/**
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 * @file       ts_vlt_app_0022.c
 * @brief      itms tracking data handling functions
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Jan-23-2024     Rajinas                         rajinasm.transight@gmail.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 */



#include "ts_vlt_app_0022.h"
#include <stdio.h>
#include <pthread.h>


extern pthread_mutex_t app_evt_mtx ;
extern pthread_cond_t  app_evnt_group ;
extern uint32_t app_0022_event;


EApp0022Errors app_0022_gen_nrl_pkt_struct(SApp0022Handle *phdle,
											SApp0022NRLPkt *pNrl,
											SLocationData *pLoc,
											EApp0022Alerts AltID);

/**
 * @brief  function to handle packet send timings. should be called each second from hardware timer isr         
 * @param[in,out]phdle: pointer to app handler
 * @retval         
 */
void app_0022_1sec_tick_isr(SApp0022Handle *phdle)
{
	static volatile uint32_t counter;
    uint32_t event = 0;

	if(	(phdle->ModInit == 0) || (phdle->pCfg->app_enabled == 0) )
		return;


    if(phdle->State.tmr_actv_ur_rate == phdle->State.ActvUpdateRate)
    {	
		fprintf(stderr, "\n\napp_22 normal packet ------------\r\n");
        event |= APP_0022_NRML_PKT_TMR;
        phdle->State.tmr_actv_ur_rate=0;
    }
	
  	if(counter >= phdle->pCfg->rate_cfg_dev_hlth)
	{   
        event |= APP_0022_HLT_PKT_TMR;
		counter = 0;
	}
    if (event !=0){
        pthread_mutex_lock(&app_evt_mtx);
        app_0022_event = event;
        pthread_cond_signal(&app_evnt_group);
        pthread_mutex_unlock(&app_evt_mtx);
    }
    (phdle->State.tmr_actv_ur_rate)++;
	counter++;
}


/**
 * @brief           
 * @param[in,out]phdle: pointer to app handler
 * @param[in]Event: app event
 * @return          EApp0022Errors
 * @retval         
 */
EApp0022Errors app_0022_event_hdlr(SApp0022Handle *phdle,EApp0022Events Event){
	
	EApp0022Errors err_code = ERR_APP_0022_NO_ERROR;
	if(	(phdle->ModInit == 0) || (phdle->pCfg->app_enabled == 0) )
		return ERR_APP_0022_NO_ERROR;
	
	switch(Event){
		case APP_0022_EVT_NRL_PKT_TMR:
			err_code = app_0022_push_nrl_pkt(phdle,APP_0008_ALT_LOC_UPDATE,NULL,NULL);
			break;
		case APP_0022_EVT_HLT_PKT_TMR:
			break;
	}

	return err_code;
}


/**
 * @brief           
 * @param[in,out]phdle: app handler
 * @param[in]Alert: App_0022 alert ID
 * @param[in,out]pLoc: pointer to store location information
 * @param[in,out]pOTAInfo: pointer to store OTA data
 * @return          EApp0022Errors
 * @retval         
 */
EApp0022Errors app_0022_push_nrl_pkt(SApp0022Handle *phdle,EApp0022Alerts Alert,SLocationData *pLoc,char *pOTAInfo){
	EApp0022Errors err_code = ERR_APP_0022_NO_ERROR;
	uint8_t *pFrameStructBuff = 0;
	uint32_t LocNull = 0;
	uint32_t OTAlen = 0;
	uint32_t FrameLen = 0;

	pFrameStructBuff = (*(phdle->pInterface->pmalloc))(sizeof(SAppGenFrameInfo) + sizeof(SApp0022NRLPkt) + 256);
	if(pFrameStructBuff == NULL)
	return ERR_APP_0022_MALLOC_FAIL;

	if(pLoc == NULL)
	{
		LocNull = 1;

		pLoc = (*(phdle->pInterface->pmalloc))(sizeof(SLocationData));

		if(pLoc == NULL)
		{
			(*(phdle->pInterface->pfree))(pFrameStructBuff);
			return ERR_APP_0022_MALLOC_FAIL;
		}

		(*(phdle->pInterface->pget_location))(pLoc);
	}

		// if(Alert == APP_0022_ALT_OTA)
		// {
		// 	OTAlen = strlen(pOTAInfo);

		// 	FrameLen = sizeof(SApp0022NRLPkt) + OTAlen;

		// 	memcpy((pFrameStructBuff + sizeof(SAppGenFrameInfo) + sizeof(SApp0022NRLPkt)),pOTAInfo,OTAlen);
		// }
		// else
		// {
		 	FrameLen = sizeof(SApp0022NRLPkt);
		// }

	((SAppGenFrameInfo*)pFrameStructBuff)->FrameLength	= FrameLen;
	((SAppGenFrameInfo*)pFrameStructBuff)->BatchPkt		= 0;
	((SAppGenFrameInfo*)pFrameStructBuff)->BatchCnt		= 1;
	((SAppGenFrameInfo*)pFrameStructBuff)->AppName		= APP_0003;
	
	err_code = app_0022_gen_nrl_pkt_struct( phdle,
				(SApp0022NRLPkt*)(pFrameStructBuff + sizeof(SAppGenFrameInfo)),
									pLoc,Alert);
	
	if(err_code == ERR_APP_0022_NO_ERROR)
	{

		err_code = ((*(phdle->pInterface->ppacketSendIPC))(pFrameStructBuff,sizeof(SAppGenFrameInfo) + FrameLen));
	
		// if(err_code == ERR_APP_0022_NO_ERROR)
		// {//Saving in flash success
		// 	if(Alert == APP_0008_ALT_LOC_UPDATE)
		// 	{
		// 		phdle->State.last_ur_pkt_cog = pLoc->CourseOverGnd;
		// 	}
		// }
	}

	if(LocNull == 1)
	{
		(*(phdle->pInterface->pfree))(pLoc);
	}

	(*(phdle->pInterface->pfree))(pFrameStructBuff);

	return err_code;

	
}


/**
 * @brief  function to generate app 22 packet         
 * @param[in,out]phdle: app 22 handler
 * @param[in,out]pNrl:  structure to store packet data
 * @param[in]pLoc:  pointer to the structure where location data is stored
 * @param[in]AltID:  app 22 alert
 * @return          EApp0022Errors
 * @retval         
 */
EApp0022Errors app_0022_gen_nrl_pkt_struct(SApp0022Handle *phdle,
											SApp0022NRLPkt *pNrl,
											SLocationData *pLoc,
											EApp0022Alerts AltID){

	EApp0022Errors err_code = ERR_APP_0022_NO_ERROR;
	uint32_t LocNull = 0;

	//Allocate memory for getting location in passed location pointer is null
	if(pLoc == NULL)
	{
		LocNull = 1;

		pLoc = (*(phdle->pInterface->pmalloc))(sizeof(SLocationData));

		if(pLoc == NULL)
		{
			return ERR_APP_0022_MALLOC_FAIL;
		}

		(*(phdle->pInterface->pget_location))(pLoc);
	}

	pNrl->PacketType	=	APP_0022_PKT_NRL;
	pNrl->AlertID		=	AltID;
	pNrl->PacketStatus	=	APP_PKT_LIVE;
	pNrl->fix_status	=	pLoc->fix_status;
	pNrl->utc_time		=	pLoc->utc_time;
	pNrl->Latitude		=	pLoc->Latitude;
	pNrl->NS_indication	=	pLoc->NS_indication;
	pNrl->Longittude	=	pLoc->Longittude;
	pNrl->EW_indication	=	pLoc->EW_indication;
	pNrl->SpeedOverGnd 	=	pLoc->SpeedOverGnd;
	pNrl->CourseOverGnd =	pLoc->CourseOverGnd;
	pNrl->Numsatellites	=	pLoc->Numsatellites;
	pNrl->Altitude		=	pLoc->Altitude;
	pNrl->PDOP			=	pLoc->PDOP;
	pNrl->HDOP			=	pLoc->HDOP;
	pNrl->OPTR			=	(*(phdle->pInterface->pGetCellActvOptr))();
	pNrl->Ignition		=	(*(phdle->pInterface->pGetIGNState))();
	pNrl->MainPwrStat	=	(*(phdle->pInterface->pGetMainPwrState))();
	pNrl->SupplyVoltage	=	(*(phdle->pInterface->pGetMainPwrVoltage))();
	pNrl->IntBattVoltage=	(*(phdle->pInterface->pGetIBattVoltage))();
	pNrl->EmergencyStat	=	(*(phdle->pInterface->pGetPanicState))();
	pNrl->Tamper		=	(*(phdle->pInterface->pGetTamperState))();


	pNrl->sgl_strgth	=	(*(phdle->pInterface->pGetCellNWINfo))()[0].sgl_strgth;
	pNrl->mcc			=	(*(phdle->pInterface->pGetCellMCC))();
	pNrl->mnc			=	(*(phdle->pInterface->pGetCellMNC))();
	pNrl->lac			=	(*(phdle->pInterface->pGetCellNWINfo))()[0].lac;
	pNrl->cellid		=	(*(phdle->pInterface->pGetCellNWINfo))()[0].cellid;

	pNrl->n1_lac		= 	(*(phdle->pInterface->pGetCellNWINfo))()[1].lac;
	pNrl->n1_cellid		=	(*(phdle->pInterface->pGetCellNWINfo))()[1].cellid;
	pNrl->n1_sgl_strgth	=	(*(phdle->pInterface->pGetCellNWINfo))()[1].sgl_strgth;

	pNrl->n2_lac		= 	(*(phdle->pInterface->pGetCellNWINfo))()[2].lac;
	pNrl->n2_cellid		=	(*(phdle->pInterface->pGetCellNWINfo))()[2].cellid;
	pNrl->n2_sgl_strgth	=	(*(phdle->pInterface->pGetCellNWINfo))()[2].sgl_strgth;

	pNrl->n3_lac		= 	(*(phdle->pInterface->pGetCellNWINfo))()[3].lac;
	pNrl->n3_cellid		=	(*(phdle->pInterface->pGetCellNWINfo))()[3].cellid;
	pNrl->n3_sgl_strgth	=	(*(phdle->pInterface->pGetCellNWINfo))()[3].sgl_strgth;

	pNrl->n4_lac		= 	(*(phdle->pInterface->pGetCellNWINfo))()[4].lac;
	pNrl->n4_cellid		=	(*(phdle->pInterface->pGetCellNWINfo))()[4].cellid;
	pNrl->n4_sgl_strgth	=	(*(phdle->pInterface->pGetCellNWINfo))()[4].sgl_strgth;

	pNrl->din1			=	(*(phdle->pInterface->pGetDIState))(1);
	pNrl->din2			=	(*(phdle->pInterface->pGetDIState))(2);
	pNrl->din3			=	(*(phdle->pInterface->pGetDIState))(3);
	pNrl->din4			=	(*(phdle->pInterface->pGetDIState))(4);

	pNrl->dout1			=	(*(phdle->pInterface->pGetDOState))(1);
	pNrl->dout2			=	(*(phdle->pInterface->pGetDOState))(2);

	//if(phdle->pSettings->rtc_frame_num_available == 1)
	//{
		pNrl->FrameNum = (*(phdle->pInterface->pGetRTCFrameNumber))();

		(*(phdle->pInterface->pSetRTCFrameNumber))(((*(phdle->pInterface->pGetRTCFrameNumber))())+1);
	//}
	// else
	// {
	// 	pNrl->FrameNum		=	++(phdle->State.frame_number);
	// }

	(*(phdle->pInterface->pget_irnss_data))(&(pNrl->OGSV));

	// APP_0008_DEBUG_PRINT(phdle,1,"NRL Packet - Alert %u, Frame Number = %u",pNrl->AlertID,pNrl->FrameNum);

	if(LocNull == 1)
	{
		(*(phdle->pInterface->pfree))(pLoc);
	}

	// app_0008_print_nrl_pkt_struct(phdle, pNrl);

	return err_code;
}
