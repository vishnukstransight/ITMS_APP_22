#include "ts_ipc_p1.h"
#include "ts_ipc_p1_cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ts_ipc_p1_hdlr.h"
#include <pthread.h>
#include "cellular_task.h"
#include "ts_vlt_app_0022.h"


SIPC_P1_Handle OIPC_P1_Hdle;

char IPCP1_MStr_DebugBuffer[2048];

static SIPC_P1_UartCfg	mstr_uart_Cfg[4];
static SIPC_P1_WdgCfg	mstr_WDG_Cfg;
static SIPC_P1_IOCtrl	mstr_IO_Ctrl;
static SIPC_P1_IOCfg	mstr_IO_Cfg;
static uint16_t IO_Status = 0;
SIPC_P1_CallData OIPC_P1_CallData;


FILE *pLogFileUartRxdData;

extern int fd;

extern pthread_mutex_t cell_evt_mtx;
extern pthread_cond_t  cellular_evnt_group;
extern ECell_Events celular_event;

void* ipc1_master_wrap_malloc(uint32_t Size)
{
//    return pvPortMalloc(Size);
	return malloc(Size);
}

void ipc1_master_wrap_free(void * ptr)
{
//	vPortFree(ptr);
	free(ptr);
}

void ipc1_master_wrap_EventCallback(EIPC_P1_Event Event,void *pParameter)
{
	SIPC_P1_UartData *pUartData = NULL;
	SIPC_P1_CanData *pCanData = NULL;
	uint32_t i=0;
	//SYSTEMTIME str_t;

	printf("\n\rEvent received = %d",Event);

	switch (Event)
	{
		case EIPC_P1_EVT_UART_DATA_RXD:

			pUartData = (SIPC_P1_UartData*)pParameter;
			printf("\r\nUART data Rxd, Index = %d, Size = %d\r\n", pUartData->Index, pUartData->DataSize);
			for(i=0;i<pUartData->DataSize;i++)
			{
				printf("%.2X ", pUartData->pDataBuffer[i]);
			}
/*
			GetLocalTime(&str_t);
			fprintf(pLogFileUartRxdData,"\r\n[%02d-%02d-%04d %02d:%02d:%02d:%03d]\tEvent\t= EIPC_P1_EVT_UART_DATA_RXD\tIdx\t= %d\tSize\t= %d\tData\t= ",
					str_t.wDay,str_t.wMonth,str_t.wYear,str_t.wHour,str_t.wMinute,str_t.wSecond,str_t.wMilliseconds,
					pUartData->Index,pUartData->DataSize);

			for (i = 0; i < pUartData->DataSize; ++i)
			{
				fprintf(pLogFileUartRxdData,"%02X ",pUartData->pDataBuffer[i]);
			}

			fflush(pLogFileUartRxdData);
*/
			break;

		case EIPC_P1_EVT_IO_CHG:

			printf("\r\nIO Change event"
					"\r\nPrevious State: %04X"
					"\r\nCurrent State : %04X",
					IO_Status,
					(*((uint16_t*)pParameter)));
/*
			GetLocalTime(&str_t);
			fprintf(pLogFileUartRxdData,"\r\n[%02d-%02d-%04d %02d:%02d:%02d:%03d]\tEvent\t= EIPC_P1_EVT_IO_CHG\tIdx\t= %d\tSize\t= %d\tData\t= ",
					str_t.wDay,str_t.wMonth,str_t.wYear,str_t.wHour,str_t.wMinute,str_t.wSecond,str_t.wMilliseconds,
					pUartData->Index,pUartData->DataSize);

			fflush(pLogFileUartRxdData);
*/
			IO_Status = (*((uint16_t*)pParameter));

			break;

		case EIPC_P1_EVT_CAN_MSG_RXD:

			pCanData = (SIPC_P1_CanData *)pParameter;
			printf("\r\nCAN Message Rxd");

			printf("\r\npCanData->Index = %.2X", pCanData->Index);
			printf("\r\npCanData->Frame.ID.id = %.2X", pCanData->Frame.ID.id);
			printf("\r\npCanData->Frame.ID.ide = %.2X", pCanData->Frame.ID.ide);
			printf("\r\npCanData->Frame.ID.rtr = %.2X", pCanData->Frame.ID.rtr);
			printf("\r\npCanData->Frame.DLC = %.2X", pCanData->Frame.DLC);
			printf("\r\npCanData->Frame.DATA = ");
			for(i=0;i<pCanData->Frame.DLC;i++)
			{
				printf("%.2X ", pCanData->Frame.DATA[i]);
			}

			break;
		
		case EIPC_P1_EVT_VOICE_CALL_RXD:

			OIPC_P1_CallData = *((SIPC_P1_CallData*)pParameter);
			pthread_mutex_lock(&cell_evt_mtx);
    		celular_event = CELLULAR_EVNT_INCOMING_CALL;
    		pthread_cond_signal(&cellular_evnt_group);
    		pthread_mutex_unlock(&cell_evt_mtx);

			break;
		case EIPC_P1_EVT_APP_DATA:;
			uint32_t buff_length;
			SAppGenFrameInfo oframe;
			SApp0022NRLPkt opkt;

			memcpy(&buff_length,pParameter,4);
			memcpy(&oframe,(pParameter+4),sizeof(SAppGenFrameInfo));
			memcpy(&opkt,(pParameter+4+sizeof(SAppGenFrameInfo)),sizeof(SApp0022NRLPkt));

			 fprintf(stderr," recieved latitude = %03d.%06d\n\r",opkt.Latitude);
			  fprintf(stderr," recieved longitude = %03d.%06d\n\r",opkt.Longittude);
			   fprintf(stderr," info = %d\n\r",oframe.AppName);

			
			
			// err_code = ((*(phdle->pInterface->ppkt_send_que_push))((pParameter+4),buff_length));
			
			break;
		default:
			break;
	}
}




EIPC_P1_Errors ipc1_master_wrap_SendPacket(uint8_t *pData,uint32_t Length)
{
	int ret = 0, i;
	EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
	uint8_t *ptr = pData;

	printf("\r\nFrame: ");
	for(i=0;i<Length;i++)
	{
		printf("%.2X ", *(ptr++));
	}
	printf("\r\nFrame End\r\n");

	//ret = write(&OIPC_P1_Mstr_SerialPortCfg, pData, Length, 1000);
	ret = write(fd, pData,Length);

	if(ret < 0)
	{
		err_code = EIPC_P1_ERR_SEND_PKT_FAIL;
	}
	else
	{
		if(ret == (int)Length)
		{
			err_code = EIPC_P1_ERR_NO_ERROR;
		}
		else
		{
			err_code = EIPC_P1_ERR_SEND_PKT_FAIL;
		}
	}

	return err_code;
}



void  ipc1_master_wrap_DebugPrint(uint32_t DbgLevel,
                            const char *pStartStr,
                            const char *pStr,
                            va_list args)
{
	vsprintf(IPCP1_MStr_DebugBuffer,pStr,args);
	printf("\r\n%s%s",pStartStr,IPCP1_MStr_DebugBuffer);
}




const SIPC_P1_Interface OMstrInterface =
{
	.pmalloc 		= 	ipc1_master_wrap_malloc,
	.pfree			=	ipc1_master_wrap_free,
	.pEventCallback	=	ipc1_master_wrap_EventCallback,
//	.pUartCfg		=	ipc1_master_wrap_UartCfg,
	//.pUartDeInit	=	ipc1_master_wrap_UartDeInit,
//	.pUartSend		=	ipc1_master_wrap_UartSend,
//	.pCanCfg		=	ipc1_master_wrap_CanCfg,
//	.pCanSend		=	ipc1_master_wrap_CanSend,
	//.pCanDeInit		=	ipc1_master_wrap_CanDeInit,
//	.pIOCfg			=	ipc1_master_wrap_IOCfg,
//	.pSys_ADCRead   =	ipc1_master_wrap_Sys_ADCRead,
//	.pGetIOStatus	=	ipc1_master_wrap_GetIOStatus,
//	.pSetIO			=	ipc1_master_wrap_SetIO,
	// .pGetCmdLock	=	ipc1_master_wrap_GetCmdLoc,
	// .pRelaseCmdLock	=	ipc1_master_wrap_RelaseCmdLock,
	.pSendPacket	=	ipc1_master_wrap_SendPacket,
	// .pSendEvent		=	ipc1_master_wrap_SendEvent,
    // .pWaitForEvent	=	ipc1_master_wrap_WaitForEvent,
	.pDebugPrint	=	ipc1_master_wrap_DebugPrint,
//	.pWDGCfg        =   ipc1_master_wrap_WDGCfg,
//	.pMstrPwrRstrt  = 	ipc1_master_wrap_Mstr_Restart,
//	.pSaveAllParams = 	ipc1_master_wrap_Save_All_Para,
//	.pResetParams   = 	ipc1_master_wrap_ResetPara,
//    .pStartEvent	= 	ipc1_master_wrap_StartEvent,
//	.pPin_ADCRead	=   ipc1_master_wrap_Pin_ADCRead,
};

SIPC_P1_Settings OMstrSettings =
{
    .Mode = EIPC_P1_MOD_MSTR,
	.DebugPrintEnabled = 1,
//	.max_payload_size = 1024,  // memory can't be dynamically allocated beyond these bytes
//	.OSysInfo =
//		{
//			.FwType = EIPC_P1_APP_FW
//			.HwCode = "HW_CODE_MSTR",
//			.AppFwCode = "TSFWAPP0035",
//			.AppFwBuildNumber = 50,
//			.ProtCode = "TS_IPC_PROT_001",
//			.ProtRevNumber = 4,
//			.NumUart = 4,
//			.NumCan = 1,
//			.NumIO = 6,
//			.NumADC = 4,
//			.NumCanFilter = 8,
//		},
};


int ipc1_mpu_mod_init()
{

	

	pLogFileUartRxdData =  fopen("ipc_mster_uart_rxd_data_log.txt", "w");

	if(pLogFileUartRxdData == NULL)
	{
		printf("\r\nfopen(ipc_mster_uart_rxd_data_log.txt) failed");

		return -1;
	}

	memset(&OIPC_P1_Hdle,0,sizeof(SIPC_P1_Handle));
//	Mtx_IPC1_Mstr_Cmd_Lock = xSemaphoreCreateMutex();
//	if(Mtx_IPC1_Mstr_Cmd_Lock == NULL)
//	{
//		return -1;
//	}
//	Que_IPC1_Mstr_Event = xQueueCreate( 16, sizeof( uint32_t ) );
//
//	if(Que_IPC1_Mstr_Event == NULL)
//	{
//		return -1;
//	}

	OIPC_P1_Hdle.pInterface	=	&OMstrInterface;
	OIPC_P1_Hdle.pSettings	=	&OMstrSettings;
	printf("\n settings value = %d\n",OIPC_P1_Hdle.pSettings->Mode );
	ipc_p1_init(&OIPC_P1_Hdle);
	//printf("\n exe104 \n");
	return 0;
}

