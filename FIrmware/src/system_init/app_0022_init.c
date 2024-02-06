/**
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 * @file       app_022_init.c
 * @brief      
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Jan-25-2024     Rajinas                         rajinasm.transight@gmail.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 */


#include <stdio.h>
#include <gps.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "app_0022_init.h"
#include "ts_vlt_app_0022.h"
#include "ts_ipc_p1_cmd.h"

uint32_t app_0008_frame_number = 0;

SApp0022Handle OApp0022Handle;
SApp0022Cfg  OApp0022Cfg;
SCellNWInfo OCellNWInfo;

pthread_mutex_t gpsd_mutex = PTHREAD_MUTEX_INITIALIZER;

extern struct gps_data_t OGps_data;
extern SIPC_P1_Handle OIPC_P1_Hdle;

void* wrap_app_0022_malloc(uint32_t size)
{
    return malloc(size);
}

void wrap_app_0022_free(void * ptr)
{
	free(ptr);
}

EApp0022Errors wrap_app_0022_get_location(SLocationData *pLocData)
{   
    uint32_t status;
    pthread_mutex_lock(&gpsd_mutex);
    status = gps_read(&OGps_data,NULL,0);
    if(status == -1){
        return ERR_APP_0022_GPS_READ_FAIL;
    }
    pLocData->fix_status = OGps_data.fix.status;
;
    fprintf(stderr," fix status = %d\n\r",pLocData->fix_status);
    //pLocData->NS_indications = 
    // pLocData->EW_indication =
    pLocData->Latitude = OGps_data.fix.latitude;
    fprintf(stderr," latitude = %03d.%06d\n\r",pLocData->Latitude);
    pLocData->Longittude = OGps_data.fix.longitude;
    fprintf(stderr," longitude = %03d.%06d\n\r",pLocData->Longittude);
    pLocData->Numsatellites = OGps_data.satellites_used;
    pLocData->SpeedOverGnd = OGps_data.fix.speed;
    pLocData->CourseOverGnd= OGps_data.fix.track;
    // pLocData->HDOP =
    // pLocData->PDOP=
    // pLocData->GPSQual=
    pLocData->Altitude= OGps_data.fix.altitude;
    // pLocData->utc_time=
    // pLocData->dummy_Lat=
    // pLocData->dummy_Lat=


    pthread_mutex_unlock(&gpsd_mutex);
	return ERR_APP_0022_NO_ERROR;
}

ECellOptrCode wrap_app_0022_GetCellActvOptr()
{
	return OPTR_JIO;
    //return OCellHdle.OCellState.OCoreState.active_optr_code;
}

SCellNWInfo* wrap_app_0022_GetCellNWINfo()
{   
    memset(&OCellNWInfo,0,sizeof(SCellNWInfo));
    return (&OCellNWInfo);
	// return OCellHdle.OCellState.OCoreState.OCellNWInfo;
}

uint32_t wrap_app_0022_GetCellMCC()
{   
    return 0;
	// return OCellHdle.OCellState.OCoreState.mcc;
}

uint32_t wrap_app_0022_GetCellMNC()
{
    return 0;
	// return OCellHdle.OCellState.OCoreState.mnc;
}

EApp0022Errors wrap_app_0022_get_irnss_data(SGSV *pGSV)
{   

    memset(pGSV,0,sizeof(SGSV));
	// loc_task_get_irnss_info(pGSV);

    return ERR_APP_0022_NO_ERROR;
}


uint32_t wrap_app_0022_GetPanicState()
{   
    return 0;
	// return OSysCfg.OAppGenCfg.panic_state;
}

uint32_t wrap_app_0022_GetIGNState()
{   
    return 1;
	// return app_gen_GetIGNState();
}

uint32_t wrap_app_0022_GetMainPwrState()
{   
    return 1;
	// return app_gen_GetMainPwrState();
}

uint32_t wrap_app_0022_GetTamperState()
{   
    return 0;
	// return app_gen_GetTamperState();
}

uint32_t wrap_app_0022_GetDIState(uint32_t DINo)
{   
    return 1;
	// return app_gen_GetDIState(DINo);
}

uint32_t wrap_app_0022_GetDOState(uint32_t DONo)
{   
    return 1;
    // return app_gen_GetDOState(DONo);
}

uint32_t wrap_app_0022_GetMainPwrVoltage()
{   
    return 90;
	// return app_gen_GetMainPwrVoltage();
}

uint32_t wrap_app_0022_GetIBattVoltage()
{   
    return 90;
	// return app_gen_GetIBattVoltage();
}

uint32_t wrap_app_0022_GetRTCFrameNumber()
{
	return app_0008_frame_number;
	//return OAppGenState.OPersistantState.Register[0]; 
}

void wrap_app_0022_SetRTCFrameNumber(uint32_t FrameNo)
{
	app_0008_frame_number = FrameNo;
	//OAppGenState.OPersistantState.Register[0] = FrameNo; 
}

EApp0022Errors wrap_app_0022_packet_Send_IPC(uint8_t *pBuff,uint32_t BuffSize)
{   
    EIPC_P1_Errors err_code;
    err_code = ipc_p1_event_app_pkt(&OIPC_P1_Hdle,BuffSize,pBuff);
    if (EIPC_P1_ERR_NO_ERROR !=err_code )
	    return ERR_APP_0022_PKT_IPC_SEND_FAILED;
    return ERR_APP_0022_NO_ERROR;
}



const SApp0022Interface OApp0022Interface={
    // .pinit                      =   wrap_app_0022_init,
    // .pdeinit                    =   wrap_app_0022_deinit,
    .pmalloc                    =   wrap_app_0022_malloc,
    .pfree                      =   wrap_app_0022_free,
    // .ppkt_nrl_strg_push         =   wrap_app_0022_NrlPktStrgPush,
    // .ppkt_nrl_strg_pop          =   wrap_app_0022_NrlPktStrgPop,
    // .ppkt_nrl_strg_format       =   wrap_app_0022_NrlPktStrgFrmt,
    // .ppkt_nrl_strg_scan         =   wrap_app_0022_NrlPktStrgScan,
    // .ppkt_nrl_strg_file_cnt     =   wrap_app_0022_NrlPktStrgFileCnt, 
    // .ppkt_emg_strg_push         =   wrap_app_0022_EmgPktStrgPush,
    // .ppkt_emg_strg_pop          =   wrap_app_0022_EmgPktStrgPop,
    // .ppkt_emg_strg_format       =   wrap_app_0022_EmgPktStrgFrmt,
    // .ppkt_emg_strg_scan         =   wrap_app_0022_EmgPktStrgScan,
    // .ppkt_emg_strg_file_cnt     =   wrap_app_0022_EmgPktStrgFileCnt, 
    // .ppkt_send_que_push         =   wrap_app_0022_PktSendQuePush,
    // .ppkt_send_que_cnt          =   wrap_app_0022_PacketSendQueCnt,
    // .psms_send_que_push         =   wrap_app_0022_SMSSendQuePush,
    // .psms_que_cnt               =   wrap_app_0022_SMSSendQueCnt,
    .pGetCellActvOptr           =   wrap_app_0022_GetCellActvOptr,
    // .pGetCellSglStrength        =   wrap_app_0022_GetCellSglStrength,
    .pGetCellNWINfo             =   wrap_app_0022_GetCellNWINfo,
    .pGetCellMNC                =   wrap_app_0022_GetCellMNC,
    .pGetCellMCC                =   wrap_app_0022_GetCellMCC,
    // .pGetCellGSMNWRegStat       =   wrap_app_0022_GetCellGSMNWRegStat,
    // .pGetCellGPRSNWRegStat      =   wrap_app_0022_GetCellGPRSNWRegStat,
    // .pGetCellIMEI               =   wrap_app_0022_GetCellIMEI,
    // .pGetCellICCID              =   wrap_app_0022_GetCellICCID,
    // .pCellApnCfg                =   wrap_app_0022_CellApnCfg,
    // .pCellSwitchProfile         =   wrap_app_0022_cell_prfosw,
    // .pcell_restart              =   wrap_app_0022_cell_restart,
    // .pCellSetTcpIpCfg           =   wrap_app_0022_CellSetTcpIpCfg,
    // .pCellGetTcpIpCfg           =   wrap_app_0022_CellGetTcpIpCfg,
    // .pCellGetTcpConnState       =   wrap_app_0022_CellGetTcpConnState,
    // .pCellTcpSendData           =   wrap_app_0022_CellTcpSendData,
    .pget_location              =   wrap_app_0022_get_location,
    .pget_irnss_data            =   wrap_app_0022_get_irnss_data,
    // .pget_last_valid_location   =   wrap_app_0022_get_last_valid_location,

    // .pPanicTurnOff              =   wrap_app_0022_PanicTurnOff,
    .pGetPanicState             =   wrap_app_0022_GetPanicState,
    // .pGetPanicWireCutState      =   wrap_app_0022_GetPanicWireCutState,
    // .set_panic_buzzer           =   wrap_app_0022_set_panic_buzzer,
    // .set_panic_blink            =   wrap_app_0022_set_panic_blink,

    .pGetIGNState               =   wrap_app_0022_GetIGNState,
    .pGetMainPwrState           =   wrap_app_0022_GetMainPwrState,
    .pGetTamperState            =   wrap_app_0022_GetTamperState,
    .pGetDIState                =   wrap_app_0022_GetDIState,
    .pGetDOState                =   wrap_app_0022_GetDOState,
    // .pSetDOState                =   wrap_app_0022_SetDOState,
    // .pGetAIState                =   wrap_app_0022_GetAIState,
    .pGetMainPwrVoltage         =   wrap_app_0022_GetMainPwrVoltage,
    .pGetIBattVoltage           =   wrap_app_0022_GetIBattVoltage,
    // .pGetIBattLevel             =   wrap_app_0022_GetIBattLevel,

    // .pSleepCallBack             =   wrap_app_0022_SleepCallBack,

    // .pSenCanMsg                 =   wrap_app_0022_SendCanMsg,

    // .pget_sys_uptime            =   wrap_app_0022_get_sys_uptime,
    // .pcfg_save                  =   wrap_app_0022_cfg_save,
    // .pMCURestart                =   wrap_app_0022_MCURestart,

    .pGetRTCFrameNumber         =   wrap_app_0022_GetRTCFrameNumber,
    .pSetRTCFrameNumber         =   wrap_app_0022_SetRTCFrameNumber,

    // .pBattChrgCtrl				=	wrap_app_0022_batt_chrg_ctrl,
    // .pPrcsFOTACmd               =   wrap_app_0022_PrcsFotaCMD,

    // .pGetAppFwInfo              =   wrap_app_0022_GetAppFwInfo,
    // .pGetBlrFwInfo              =   wrap_app_0022_GetBlrFwInfo,

    // .psend_event                =   wrap_app_0022_send_event,
    // .psend_event_isr            =   wrap_app_0022_send_event_isr,

    // .pdebug_print               =   wrap_app_0022_debug_print,

    // .pgeo_fence_save			=	wrap_app_0022_geo_fence_save,
    // .pget_gf_lock				=	wrap_app_0022_get_gf_lock,
    // .prelease_gf_lock			=	wrap_app_0022_release_gf_lock,

    .ppacketSendIPC             =  wrap_app_0022_packet_Send_IPC,
};
void app_0022_mod_init(void){


OApp0022Handle.pCfg=&OApp0022Cfg;
OApp0022Handle.pInterface =&OApp0022Interface;

OApp0022Handle.ModInit =1;
OApp0022Handle.pCfg->app_enabled=1;
OApp0022Handle.State.ActvUpdateRate=5;
OApp0022Handle.State.tmr_actv_ur_rate = 0;
OApp0022Handle.pCfg->rate_cfg_dev_hlth = 15;


}