/**
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 * @file       ts_vlt_app_0022.h
 * @brief      header file for app_0022.c 
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



#ifndef CODE_LIB_APPLICATIONS_TS_VLT_APP_0022_TS_VLT_APP_0022_H_
#define CODE_LIB_APPLICATIONS_TS_VLT_APP_0022_TS_VLT_APP_0022_H_

#define APP_0022_CODE		"TSVLTAPP0022"


#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#define BIT_BYTE_MASK(x)		1<<(x)

#define APP_0022_NRML_PKT_TMR 	BIT_BYTE_MASK(0)
#define APP_0022_HLT_PKT_TMR	BIT_BYTE_MASK(1)

typedef enum
{
	APP_0003	=	3,
	APP_0011	=	11,
	APP_0008	=	8,
	APP_0022    =   22,
}EAppGenName;

typedef enum{
	ERR_APP_0022_NO_ERROR  = 0x00,
	ERR_APP_0022_MALLOC_FAIL,
	ERR_APP_0022_GPS_READ_FAIL,
	ERR_APP_0022_PKT_IPC_SEND_FAILED,

}EApp0022Errors;

typedef enum
{
	APP_0022_PKT_NRL	=	0,
	APP_0022_PKT_EPB,
	APP_0022_PKT_HLT,
	APP_0022_PKT_LGN,
	APP_0022_PKT_UNKNOWN,
}EApp0022PktType;

typedef enum
{
    APP_PKT_LIVE    =   0,
    APP_PKT_HISTORY,
}EAppPktStatus;

typedef enum{
	APP_0022_EVT_NRL_PKT_TMR,
	APP_0022_EVT_HLT_PKT_TMR,
}EApp0022Events;

typedef enum
{
	APP_0008_ALT_UNUSED = 0,
	APP_0008_ALT_LOC_UPDATE = 1,
}EApp0022Alerts;

typedef enum
{
	OPTR_UNKNOWN = 0,
	OPTR_IDEA,
	OPTR_BSNL,
	OPTR_AIRTEL,
	OPTR_VODAFONE,
	OPTR_JIO,
}ECellOptrCode;


typedef struct
{
	uint32_t sgl_strgth		:7;
	uint32_t cellid			:16;
	uint32_t lac			:16;
}SCellNWInfo;

typedef struct
{
	uint32_t	Elevation		:	7;
	uint32_t	SNR				:	7;
	uint32_t	Reserved		:	8;
	uint32_t	Azimuth			:	10;
	uint32_t	ID				:	16;
}SSatelliteInfo;

typedef struct
{
	uint32_t	ValidData		:	1;
	uint32_t	TalkerID		:	3;
	uint32_t	MsgCnt			:	4;
	uint32_t	SeqNum			:	4;
	uint32_t	SatCnt			:	6;
	uint32_t	Reserved1		:	6;
	uint8_t 	CheckSum		:	8;
	SSatelliteInfo OSatInfo[4];
}SGSV;

#pragma pack(1)
typedef struct
{
	uint32_t	IntBattVoltage 	:13;
	uint32_t	n3_sgl_strgth	:7;
	uint32_t	n4_sgl_strgth	:7;
	uint32_t	Numsatellites	:5;		//GGA

	uint32_t	FrameNum		:20;
	uint32_t	mcc				:10;
	uint32_t	EmergencyStat	:1;
	uint32_t	Tamper			:1;

	uint32_t	SpeedOverGnd	:20;		//VTG
	uint32_t	mnc				:10;
	uint32_t	Ignition		:1;
	uint32_t	MainPwrStat		:1;

	uint32_t	CourseOverGnd	:20;		//VTG
	uint32_t	n2_sgl_strgth	:7;
	uint32_t	AlertID			:5;

	uint32_t 	Altitude		:24; 		//Altitude * meters
	uint32_t	sgl_strgth		:7;
	uint32_t	fix_status		:1;

	uint32_t	Latitude		:29;		//format decimal degrees * 10^6
	uint32_t	din1  			:1;		//Digital Input 1
	uint32_t	din2  			:1;		//Digital Input 2
	uint32_t	din3  			:1;		//Digital Input 3

	uint32_t	Longittude		:29;		//format decimal degrees * 10^6
	uint32_t	din4  			:1;		//Digital Input 4
	uint32_t	dout1  			:1;		//Digital Output 0
	uint32_t	dout2  			:1;		//Digital Output 1

	uint32_t 	SupplyVoltage	:16;
	uint32_t	n1_sgl_strgth	:7;
	uint32_t	OPTR			:4;		//GGA
	uint32_t	PacketType		:2;
	uint32_t 	PacketStatus	:1;
	uint32_t	NS_indication	:1;
	uint32_t	EW_indication	:1;

	uint32_t 	cellid			:16;
	uint32_t 	n1_cellid		:16;

	uint32_t 	n2_cellid		:16;
	uint32_t 	n3_cellid		:16;

	uint32_t 	n4_cellid		:16;
	uint32_t	lac				:16;

	uint32_t	n1_lac			:16;
	uint32_t	n2_lac			:16;

	uint32_t	n3_lac			:16;
	uint32_t	n4_lac			:16;

	uint32_t	HDOP			:16;		//GGA
	uint32_t	PDOP			:16;		//GSA

	time_t 		utc_time;

	SGSV OGSV;

}SApp0022NRLPkt; 


typedef struct
{
	uint32_t	fix_status		:	1;		//GGA
	uint32_t	NS_indication	:	1;		//GGA
	uint32_t	EW_indication	:	1;		//GGA
	uint32_t	Latitude		:	29;		//format decimal degrees * 10^6
	uint32_t	Longittude		:	29;		//format decimal degrees * 10^6
	uint32_t	Numsatellites	:	5;		//GGA
	uint32_t	SpeedOverGnd	:	20;		//VTG
	uint32_t	CourseOverGnd	:	20;		//VTG
	uint32_t	HDOP			:	16;		//GGA
	uint32_t	PDOP			:	16;		//GSA
	uint32_t	GPSQual			:	4;
	int32_t 	Altitude; 	//Altitude * meters
	time_t 		utc_time;
	uint32_t	dummy_Lat		;		
	uint32_t	dummy_Long      ;	
}SLocationData;

typedef struct
{
	// char VendorID[12];
	// char VehicleRegNo[16];

	// char AdminControlNumber[4][15];
	// char EmrgCtrlCtrNo[2][15];

	// uint32_t dummy_bit_feild_aligner3;

	// uint32_t en_alt_on_off_ctrl	:1;
	// uint32_t en_alt_panic		:1;
	// uint32_t en_alt_sply_rmvl	:1;
	// uint32_t en_alt_tamper		:1;

	// uint32_t en_alt_low_batt	:1;
	// uint32_t en_alt_ign			:1;
	// uint32_t en_alt_tilt		:1;
	// uint32_t en_alt_ovr_spd		:1;
	
	// uint32_t en_alt_crash		:1;
	// uint32_t en_alt_hbrake		:1;
	// uint32_t en_alt_haccel		:1;
	// uint32_t en_alt_rturn		:1;
	
	// uint32_t en_alt_rtviol		:1;
	// uint32_t en_alt_geo_fence	:1;
	// uint32_t en_alt_flash_mem	:1;
	// uint32_t en_alt_ota			:1;

	// uint32_t en_imob			:1;
	// uint32_t en_auto_nwsw		:1;
	// uint32_t en_loc_out_mode	:1;
	// uint32_t en_updt_chk		:1;
	
	// uint32_t en_sms_fb				:1;
	// uint32_t en_alert_dest			:1;
	// uint32_t en_mem_bkp_test_mode	:1;
	// uint32_t master_mode			:1;

	// uint32_t ea10_pend			:1;
	// uint32_t ea10_saved_pend	:1;
	// uint32_t ea11_pend			:1;
	// uint32_t ea11_saved_pend	:1;

	// uint32_t debug_print_enabled		:1;
	// uint32_t dummy_bit_feild_aligner	:3;

	// // uint32_t dummy_bit_feild_aligner2;

	uint8_t app_enabled;
	// uint8_t emg_svr_enabled;

	// uint16_t rate_cfg_slp_mod;
	// uint16_t rate_cfg_ign_off;
	// uint16_t rate_cfg_ign_on;
	// uint16_t rate_cfg_vhcl_turn;
	uint16_t rate_cfg_dev_hlth;
	// uint16_t rate_cfg_panic_alt;
	// uint16_t rate_cfg_sms_fb;

	// uint16_t tout_slp;
	// uint16_t tout_nw_sw;
	// uint16_t tout_panic_state;
	// uint16_t tout_sms_fb;
	// uint16_t tout_panic_wc_sense;

	// uint16_t thresh_hbrake_raw;
	// uint16_t thresh_hbrake_mg;
	// uint16_t thresh_haccel_raw;
	// uint16_t thresh_haccel_mg;
	// uint16_t thresh_rturn_raw;
	// uint16_t thresh_rturn_deg;
	// uint16_t thresh_sply_rmvl;
	// uint16_t thresh_low_batt;
	// uint16_t thresh_flash_mem;
	// uint16_t thresh_over_sp;
	// uint16_t thresh_tilt_ref;
	// uint16_t thresh_asw_sgl_level;

	// float pitch_ref;
	// float roll_ref;
	// float yaw_ref;

	// EApp0008ProtocolVer ProtocolVersion;

	// SLocationData LstVldLoc;

	// SApp0008NRLPkt OEA10Packet;
	// SApp0008NRLPkt OEA11Packet;
	// uint8_t FsMode;		//FileSystem Mode. 0 - FIFO, 1 - LIFO
	
}SApp0022Cfg;

typedef struct
{
// 	float		FlashMemPercent;
// 	float		VehicleSpeed;
// 	uint32_t	flash_mem_rec_cnt;
// 	uint32_t	fmem_evt_rec_cnt;
// 	uint32_t 	AnalogInput1;
// 	uint32_t 	AnalogInput2;
// 	uint32_t	FrameNumber;
// 	char		PktTriggerType;
// 	uint8_t 	TrckCfgDatValid;
// 	uint8_t 	TrckCfgDat_bkpValid;
// 	uint32_t	SlpModToutCntr;
// 	uint32_t	active_trck_updt_rate;
// 	uint8_t    	set_tilt_ref;
// 	uint8_t 	chk_update_now;
// 	uint8_t		sms_enabled;
// 	uint8_t		identity_msg_sent;
// 	uint8_t		login_pkt_sent_cnt[2];

// 	uint8_t 	ps_es_sent;		//Primary server emergency stop pkt sent
// 	uint8_t 	es_sem_sent;	//Emergency server sem_sent

// 	uint16_t	tmr_ign_off;
// /*	uint16_t	tmr_slp_mode_high;
// 	uint16_t	tmr_slp_mode_low;*/
// 	uint16_t 	tmr_panic_st;
// 	uint16_t 	tmr_panic_high;
// 	uint16_t 	tmr_tamper_st;
// 	uint16_t 	tmr_sply_rmvl_st;
// 	uint16_t 	tmr_low_batt_st;
// 	uint16_t 	tmr_low_mem_st;

// 	uint16_t 	cntr_tcp_send_ok;
// 	uint16_t 	cntr_tcp_send_fail;

// 	uint32_t	tmr_deep_sleep_high;

// 	uint32_t 	lst_epb_emr_sms_time;
// 	uint32_t 	lst_epb_ser_sms_time;
// 	uint32_t 	lst_autp_prof_sw_time;

// 	SApp0008Flag OFlags;

// 	uint8_t dig_out1_en;	//If set to one dig out will serve as digout 1 else it serve as panic led.

// 	char	OTAInfo[APP_0008_OTA_INFO_SIZE];

// 	SAppGenIMUThresh OIMUThresh;

// 	SApp0008TiltState 	OTillState;
// 	SApp0008MsenseState	OMSenseState;

// 	uint32_t frame_number;

 	uint32_t tmr_actv_ur_rate;

// 	uint32_t svr_ping_cnt[2];

// 	uint32_t last_tcp_send_ok_time[2];

// 	uint32_t tmp_pad;

	uint32_t ActvUpdateRate;
// 	uint32_t	last_ur_pkt_cog;
// 	uint32_t	update_gf_stat		:1;

// 	SApp008GfStat  prev_gf_stat[APP_0008_MAX_NUM_GEOFENCE];

// 	SApp008GfStat  crnt_gf_stat[APP_0008_MAX_NUM_GEOFENCE];

// /*	SApp0008NRLPkt ea_10_pkt;
// 	SApp0008NRLPkt ea_11_pkt;*/

}SApp0022State;	


typedef struct
{
	//Flash memory interface functions
	// EApp0008Errors (*pinit)(void);			//APP 003 initialization
	// EApp0008Errors (*pdeinit)(void);		//De initialize

	// //Dynamic memory allocation APIs
	void* (*pmalloc)(uint32_t Size);
	void (*pfree)(void * ptr);

	// //File system Related APIs Start
	// //---------------------------------------------------------------------
	// EApp0008Errors (*ppkt_nrl_strg_push)(uint8_t *pBuff,uint32_t BuffSize);
	// EApp0008Errors (*ppkt_nrl_strg_pop)(uint8_t *pBuffer, uint32_t *pRdCnt);
	// EApp0008Errors (*ppkt_nrl_strg_format)();
	// EApp0008Errors (*ppkt_nrl_strg_scan)();
	// uint32_t (*ppkt_nrl_strg_file_cnt)();

	// EApp0008Errors (*ppkt_emg_strg_push)(uint8_t *pBuff,uint32_t BuffSize);
	// EApp0008Errors (*ppkt_emg_strg_pop)(uint8_t *pBuffer, uint32_t *pRdCnt);
	// EApp0008Errors (*ppkt_emg_strg_format)();
	// EApp0008Errors (*ppkt_emg_strg_scan)();
	// uint32_t (*ppkt_emg_strg_file_cnt)();

	// // EApp0008Errors (*ppkt_hlt_strg_push)(uint8_t *pBuff,uint32_t BuffSize);
	// // EApp0008Errors (*ppkt_hlt_strg_pop)(uint8_t *pBuffer, uint32_t *pRdCnt);
	// // EApp0008Errors (*ppkt_hlt_strg_format)();
	// // EApp0008Errors (*ppkt_hlt_strg_scan)();
	// // uint32_t (*ppkt_hlt_strg_file_cnt)();

	// EApp0008Errors (*ppkt_send_que_push)(uint8_t *pBuff,uint32_t BuffSize);
	EApp0022Errors (*ppacketSendIPC)(uint8_t *pBuff,uint32_t BuffSize);
	// uint32_t (*ppkt_send_que_cnt)();

	// EApp0008Errors (*psms_send_que_push)(SCellSMSContent *pSMS);
	// uint32_t (*psms_que_cnt)();

	// //File system Related APIs END
	// //---------------------------------------------------------------------


	// //Cellular Module Related APIs Start
	// //---------------------------------------------------------------------
	ECellOptrCode (*pGetCellActvOptr)();
	// uint32_t (*pGetCellSglStrength)();
	SCellNWInfo* (*pGetCellNWINfo)();
	uint32_t (*pGetCellMNC)();
	uint32_t (*pGetCellMCC)();
	// uint32_t (*pGetCellGSMNWRegStat)();
	// uint32_t (*pGetCellGPRSNWRegStat)();
	// char const* (*pGetCellIMEI)();
	// char const* (*pGetCellICCID)();
	// // char const* (*pGetCellApn)();
	// // EApp0008Errors (*pcell_set_apn)(char *pApnStr);
	// SCellApnInfo const* (*pCellApnCfg)(SCellApnInfo const *pApnInfo);
	// EApp0008Errors (*pCellSwitchProfile)();
	// EApp0008Errors (*pcell_restart)();

	// //Cellular Module TCP communication related APIs Start
	// //---------------------------------------------------------------------

	// 	EApp0008Errors (*pCellSetTcpIpCfg)(EApp0008SvrName SvrName,
	// 										EApp0008IPType Ipype,
	// 										char *pIpStr,
	// 										char *pPortNumStr,
	// 										char *pProtocolStr);

	// 	EApp0008Errors (*pCellGetTcpIpCfg)(EApp0008SvrName SvrNam,
	// 										EApp0008IPType Ipype,
	// 										char *pIpStr,
	// 										char *pPortNumStr,
	// 										char *pProtocolStr);

	// 	uint32_t (*pCellGetTcpConnState)(EApp0008SvrName SvrNam);

	// 	EApp0008Errors (*pCellTcpSendData)(EApp0008SvrName Svr,
	// 												char *pTxBuffer,
	// 												uint32_t TxLength);

	// //Cellular Module TCP communication related APIs end
	// //---------------------------------------------------------------------

	// //Cellular Module Related APIs End
	// //---------------------------------------------------------------------

	// //Location Module related APIs START
	// //---------------------------------------------------------------------
	EApp0022Errors (*pget_location)(SLocationData *pLocData);
	EApp0022Errors (*pget_irnss_data)(SGSV *pGSV);
	// EApp0008Errors (*pget_last_valid_location)(SLocationData *pLocData);
	// //Location Module related APIs END
	// //---------------------------------------------------------------------

	// //Panic Management Functions
	// void (*pPanicTurnOff)();
	uint32_t (*pGetPanicState)();
	// uint32_t (*pGetPanicWireCutState)();
	// void (*set_panic_buzzer)(uint32_t on_off);			//Required if the application is configured in master mode.
	// void (*set_panic_blink)(uint32_t on_off);			//Required if the application is configured in master mode.

	// //IO management functions
	uint32_t (*pGetIGNState)();
	uint32_t (*pGetMainPwrState)();
	uint32_t (*pGetTamperState)();
	uint32_t (*pGetDIState)(uint32_t DINo);
	uint32_t (*pGetDOState)(uint32_t DONo);
	// void (*pSetDOState)(uint32_t DONo,uint32_t State);
	// uint32_t (*pGetAIState)(uint32_t DINo);
	uint32_t (*pGetMainPwrVoltage)();
	uint32_t (*pGetIBattVoltage)();
	// uint32_t (*pGetIBattLevel)();

	// /*
	//  * This function will be called whenever the application
	//  * enters and exits sleep mode
	//  * While entering sleep mode variable sleep will be  1
	//  * and while exiting the value of variable sleep will be 0
	//  */
	// void (*pSleepCallBack)(uint32_t sleep);
    // EApp0008Errors (*pcfg_save)(void);
	// EApp0008Errors (*pgeo_fence_save)(SApp0008Geofence *pGeofence);	/*Caution: Implementation of this function should not use gefence lock. Lock/release will be handled by the application.*/
	// EApp0008Errors (*pget_gf_lock)(void);
	// void (*prelease_gf_lock)(void);

	// //Can functions
	// EApp0008Errors (*pSenCanMsg)(SCanMsg *pCanMsg);

	// uint32_t	(*pget_sys_uptime)();
	// EApp0008Errors (*pMCURestart)();

	uint32_t	(*pGetRTCFrameNumber)();
	void (*pSetRTCFrameNumber)(uint32_t FrameNo);

	// uint32_t (*pBattChrgCtrl)(uint32_t ctrl);

	// void (*pPrcsFOTACmd)(char *pCmd);

	// SFWInfo1 const* (*pGetAppFwInfo)();
	// SFWInfo1 const* (*pGetBlrFwInfo)();

	// void (*psend_event)(EApp0008Events event);
	// void (*psend_event_isr)(EApp0008Events event);

	// void  (*pdebug_print)(uint32_t DbgLevel,
	// 							const char *pStartStr,
	// 							const char *pStr,
	// 							va_list args);

}SApp0022Interface;



typedef struct
{
	uint32_t 				HdleInit 	:1 ;		//Handle init status
	uint32_t 				ModInit 	:1 ;
	uint32_t 				InstanceNo	;	
	SApp0022Cfg				*pCfg;
	SApp0022State			State;
	// SApp0008Geofence const  *pGeoFence;	//Points to the starting of the geofence storage.
	// SApp0008Settings  const *pSettings;
	SApp0022Interface const *pInterface;
}SApp0022Handle;


typedef struct
{
	uint32_t FrameLength			:10;	//Total Size of entire frame in bytes
	uint32_t BatchCnt				:7;		//Number of packets following the header. If it is a batch packet
	uint32_t AppName				:6;		//type EAppGenName
	uint32_t RetryCnt				:6;		//Frame sent retry count.
	uint32_t BatchPkt				:1;		//1-  Batch packet, 0 - Not  Batch packet a single packet.
}SAppGenFrameInfo;

void app_0022_1sec_tick_isr(SApp0022Handle *phdle);
EApp0022Errors app_0022_event_hdlr(SApp0022Handle *phdle,EApp0022Events Event);
EApp0022Errors app_0022_push_nrl_pkt(SApp0022Handle *phdle,
										EApp0022Alerts Alert,
										SLocationData *pLoc,
										char *pOTAInfo);



#endif