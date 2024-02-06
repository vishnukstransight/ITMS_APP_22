#include "ts_ipc_p1.h"
#include "ts_ipc_p1_crc.h"
#include "ts_ipc_p1_hdlr.h"
#include "ts_ipc_p1_cmd.h"
#include <stddef.h>
#include <stdarg.h>
#include<stdio.h>
#include "string.h"


EIPC_P1_Errors ipc_p1_payload_handler(SIPC_P1_Handle *phdle);


EIPC_P1_Errors ipc_p1_send_frame(SIPC_P1_Handle *phdle, uint8_t *pBuffer, uint16_t Length)
{
    EIPC_P1_Errors err_code =  EIPC_P1_ERR_NO_ERROR;
    uint16_t crc = 0;

    if(phdle->Init != 1)
    {
        return EIPC_P1_ERR_HDLE_NOT_INIT;
    }

    if(Length < 3)
    {
        return EIPC_P1_ERR_BUFFER_SIZE;
    }

    crc = ipc_p1_crc( (pBuffer + IPC_P1_FRAME_HEAD_SIZE) , (uint16_t)(Length - IPC_P1_FRAME_HEAD_SIZE));

    //Header
    pBuffer[0] = 0xAA;
    pBuffer[1] = (uint8_t) (Length - 3) & 0xFF;
    pBuffer[2] = (uint8_t) ((Length - 3) >> 8) & 0xFF;

    //Tail
    pBuffer[Length++] = (uint8_t) crc & 0xFF;
    pBuffer[Length++] = (uint8_t) (crc >> 8) & 0xFF;
    pBuffer[Length++] = 0x55;
    pBuffer[Length] = 0;
    printf("\r\n--------------inside send frame------ ");
    err_code = (*(phdle->pInterface->pSendPacket))(pBuffer,(uint32_t)Length);

    return err_code;

}

EIPC_P1_Errors ipc_p1_frame_handler(SIPC_P1_Handle *phdle,uint8_t *pData, uint32_t Length)
{
	EIPC_P1_Errors err_code =  EIPC_P1_ERR_NO_ERROR;
    //char printbuff[1024];

	if(phdle->Init != 1)
	{
	    return EIPC_P1_ERR_HDLE_NOT_INIT;
	}

	while(Length != 0)
	{
        switch(phdle->OState.Current_State)
        {
            case EIPC_P1_IDLE_STATE:
                if(*pData == IPC_P1_START_BYTE)
                {
                    phdle->OState.PayloadLength = 0;
                    phdle->OState.Next_State = EIPC_P1_PAYLOAD_LEN_STATE1;
                    err_code = EIPC_P1_ERR_NO_ERROR;
                }
                else
                {
                    err_code = EIPC_P1_ERR_START_BYTE;
                    phdle->OState.Next_State = EIPC_P1_IDLE_STATE;
                }
                break;

            case EIPC_P1_PAYLOAD_LEN_STATE1:
                phdle->OState.PayloadLength = *pData;
                phdle->OState.Next_State = EIPC_P1_PAYLOAD_LEN_STATE2;
                err_code = EIPC_P1_ERR_NO_ERROR;
                break;

            case EIPC_P1_PAYLOAD_LEN_STATE2:
                phdle->OState.PayloadLength |= (uint16_t) ((*pData) << 8);

                TS_IPC_P1_DEBUG_PRINT(phdle, 5, "PAYLOAD LEN: %d", phdle->OState.PayloadLength);
                printf("PAYLOAD LEN:%d",phdle->OState.PayloadLength);

                if(phdle->OState.PayloadLength > IPC_P1_MAX_PAYLOAD_SIZE)
                {
                    phdle->OState.Next_State = EIPC_P1_IDLE_STATE;
                    err_code =  EIPC_P1_ERR_MAX_PAYLOAD_EXCEEDED;
                    break;
                }

                if(phdle->OState.PayloadLength > 0)
                {
                    phdle->OState.pPayload = 0;
                    phdle->OState.pPayload = (uint8_t*) (phdle->pInterface->pmalloc(phdle->OState.PayloadLength));

                    if(phdle->OState.pPayload == 0)
                    {
                        phdle->OState.Next_State = EIPC_P1_IDLE_STATE;
                        err_code = EIPC_P1_ERR_NULL_PTR ;
                        break;
                    }
                }

                phdle->OState.PayloadIndex = 0;
                phdle->OState.Next_State = EIPC_P1_PAYLOAD_STATE;
                err_code = EIPC_P1_ERR_NO_ERROR;
                break;

            case EIPC_P1_PAYLOAD_STATE :
                phdle->OState.pPayload[phdle->OState.PayloadIndex++] = *pData;

                if(phdle->OState.PayloadLength == phdle->OState.PayloadIndex)
                {
                    phdle->OState.Crc = 0;
                    phdle->OState.Next_State = EIPC_P1_CRC_STATE1;
                }
                err_code = EIPC_P1_ERR_NO_ERROR;
                break;

            case EIPC_P1_CRC_STATE1 :
                phdle->OState.Crc = *pData;
                phdle->OState.Next_State = EIPC_P1_CRC_STATE2;
                err_code = EIPC_P1_ERR_NO_ERROR;
                break;

            case EIPC_P1_CRC_STATE2 :
                phdle->OState.Crc |= (uint16_t) ((*pData) << 8);
                phdle->OState.Next_State = EIPC_P1_STOP_STATE;
                err_code = EIPC_P1_ERR_NO_ERROR;
                break;


            case EIPC_P1_STOP_STATE :
                if(*pData != IPC_P1_STOP_BYTE)
                {
                    err_code = EIPC_P1_ERR_STOP_STATE;
                    phdle->OState.Next_State = EIPC_P1_IDLE_STATE;
                }
                else
                {   
                    if(phdle->OState.Crc == ipc_p1_crc(phdle->OState.pPayload,phdle->OState.PayloadLength))
                    {
                        err_code = ipc_p1_payload_handler(phdle);
                    }
                    else
                    {
                        err_code = EIPC_P1_ERR_INVALID_CRC;
                    }
                }
                phdle->pInterface->pfree(phdle->OState.pPayload);
                phdle->OState.pPayload = 0;
                phdle->OState.Next_State = EIPC_P1_IDLE_STATE;
                break;

            default:
                err_code = EIPC_P1_ERR_INVALID_STATE;

                if(phdle->OState.pPayload != 0)
                {
                    phdle->pInterface->pfree(phdle->OState.pPayload);
                }
                phdle->OState.pPayload = 0;
                phdle->OState.Current_State = EIPC_P1_IDLE_STATE;
                break;
        }

        phdle->OState.Current_State = phdle->OState.Next_State ;

        Length--;
        pData++;

        if(err_code != EIPC_P1_ERR_NO_ERROR)

        {
        //    TS_IPC_P1_DEBUG_PRINT(phdle, 4, "IPC FRAME ERROR: %d", err_code);
        }
	}
	return err_code;
}


EIPC_P1_Errors ipc_p1_payload_handler(SIPC_P1_Handle *phdle)
{
    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;

    phdle->OState.PayloadIndex = 0;

	TS_IPC_P1_DEBUG_PRINT(phdle,4,"Payload Received");
    printf("----------Payload received-----");

	phdle->OState.identifier = 0;
    phdle->OState.identifier = (uint16_t) phdle->OState.pPayload[phdle->OState.PayloadIndex++];
    phdle->OState.identifier |= (uint16_t) ((phdle->OState.pPayload[phdle->OState.PayloadIndex] & 0x0F) << 8);

    phdle->OState.cmd_resp_byte = ((phdle->OState.pPayload[phdle->OState.PayloadIndex++]) >> 4) & 0x0F;

    if( ((phdle->OState.cmd_resp_byte == EIPC_P1_GET_RESP) || (phdle->OState.cmd_resp_byte == EIPC_P1_SET_RESP)) &&
        (phdle->OState.OAckState.AckExpected == 1) )
    {
        phdle->OState.OAckState.AckRxdDataSize = (uint32_t) (phdle->OState.PayloadLength - phdle->OState.PayloadIndex);

        if(phdle->OState.OAckState.AckBuffSize >= phdle->OState.OAckState.AckRxdDataSize)
        {
            memcpy( (void *)(phdle->OState.OAckState.pAckBuff),
                    (const void *)&(phdle->OState.pPayload[phdle->OState.PayloadIndex]),
                     phdle->OState.OAckState.AckRxdDataSize);

            phdle->OState.PayloadIndex = phdle->OState.PayloadLength;
        }
        else
        {
            err_code = EIPC_P1_ERR_BUFFER_SIZE;
        }
    }
    else if((phdle->OState.cmd_resp_byte == EIPC_P1_SET_CMD_NACK) || (phdle->OState.cmd_resp_byte == EIPC_P1_EVENT))
    {
        //Do nothing
    }
    else if((phdle->OState.cmd_resp_byte == EIPC_P1_SET_CMD) || (phdle->OState.cmd_resp_byte == EIPC_P1_GET_CMD))
    {
        phdle->OState.pRespBuffer = (uint8_t*)(*(phdle->pInterface->pmalloc))(IPC_P1_FRAME_SIZE);

        if(phdle->OState.pRespBuffer == 0)
        {
            err_code = EIPC_P1_ERR_NULL_PTR ;
        }
        else
        {
            phdle->OState.RespBuffIndex = IPC_P1_PAYLOAD_START_INDEX;
            if(phdle->OState.cmd_resp_byte == EIPC_P1_SET_CMD)
            {
                phdle->OState.pRespBuffer[phdle->OState.RespBuffIndex++] = phdle->OState.identifier & 0xFF;
                phdle->OState.pRespBuffer[phdle->OState.RespBuffIndex++] = ((phdle->OState.identifier >> 8) | (EIPC_P1_SET_RESP << 4))& 0xFF;
            }
            else
            {
                phdle->OState.pRespBuffer[phdle->OState.RespBuffIndex++] = phdle->OState.identifier & 0xFF;
                phdle->OState.pRespBuffer[phdle->OState.RespBuffIndex++] = ((phdle->OState.identifier >> 8) | (EIPC_P1_GET_RESP << 4))& 0xFF;
            }
        }
    }
    else
    {
        err_code = EIPC_P1_ERR_INVALID_CMD;
    }

    if(err_code == EIPC_P1_ERR_NO_ERROR)
    {
    	TS_IPC_P1_DEBUG_PRINT(phdle,4,	"Payload Identifier = 0x%.4X",
										phdle->OState.identifier);
        printf("Payload Identifier = 0x%.4X",phdle->OState.identifier);

		TS_IPC_P1_DEBUG_PRINT(phdle,4, 	"Payload CmdRespByte = 0x%.2X",
										phdle->OState.cmd_resp_byte);

        printf("Payload CmdRespByte = 0x%.2X",phdle->OState.cmd_resp_byte);

        switch(phdle->OState.identifier)
        {

#ifndef IPC_P1_BOOTLOADER_ENABLE

          
            
            case EIPC_P1_ID_VOICE_CALL_DATA:
                err_code = ipc_p1_voice_call_hdlr(phdle);
                break;
            
            case EIPC_P1_ID_APP_PKT_DATA:
                err_code = ipc_p1_app_data_hdlr(phdle);
                break;

#endif

            default:
            	err_code = ipc_p1_error_hdlr(phdle);
                break;
        }

    }
    
    if(err_code == EIPC_P1_ERR_NO_ERROR)
    {
        if((phdle->OState.cmd_resp_byte == EIPC_P1_SET_CMD) || (phdle->OState.cmd_resp_byte == EIPC_P1_GET_CMD))
        {
            err_code = ipc_p1_send_frame(phdle, phdle->OState.pRespBuffer, phdle->OState.RespBuffIndex);
            // TS_IPC_P1_DEBUG_PRINT(phdle,4, 	"ipc_p1_send_frame() = %d", err_code);
            printf("ipc_p1_send_frame() = %d", err_code);
        }
    }
    else
    {
    	TS_IPC_P1_DEBUG_PRINT(phdle,4, 	"switch(phdle->OState.identifier) = %d",err_code);
    }

    if(phdle->OState.pRespBuffer != 0)
    {
        (*(phdle->pInterface->pfree))(phdle->OState.pRespBuffer);
        phdle->OState.pRespBuffer = 0;
    }

    return err_code;
}

void TS_IPC_P1_DEBUG_PRINT(SIPC_P1_Handle *phdle,uint32_t DbgLevel, const char *pStr,...)
{
	va_list args;
	va_start(args, pStr);

	if(phdle->Init != 1)
    {
        return;
    }

	if(phdle->pSettings->DebugPrintEnabled == 1)
	{
		(*(phdle->pInterface->pDebugPrint))(DbgLevel,"TS_IPC_P1 : ",pStr,args);
	}
	else
	{
		if(DbgLevel == 0)
		{
			(*(phdle->pInterface->pDebugPrint))(DbgLevel,"TS_IPC_P1 : ",pStr,args);
		}
	}
}


EIPC_P1_Errors ipc_p1_init(SIPC_P1_Handle *phdle)
{
    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
    SIPC_P1_SystemInfo SysInfo;

    // if((phdle->pInterface->pDebugPrint == 0) ||
    //     (phdle->pInterface->pGetCmdLock == 0) ||
    //     (phdle->pInterface->pRelaseCmdLock == 0) ||
    //     (phdle->pInterface->pSendPacket == 0) ||
    //     (phdle->pInterface->pmalloc == 0) ||
    //     (phdle->pInterface->pfree == 0) ||
    //     (phdle->pInterface->pSendEvent == 0) ||
    //     (phdle->pInterface->pWaitForEvent == 0)
    //   )
    // {
    //     return EIPC_P1_ERR_NULL_PTR;
    // }

//     if(phdle->pSettings->Mode == EIPC_P1_MOD_SLAVE)
//     {
//         if(
// #ifndef IPC_P1_BOOTLOADER_ENABLE
// 			(phdle->pInterface->pUartCfg == 0) ||
//             (phdle->pInterface->pUartSend == 0) ||
//             (phdle->pInterface->pCanCfg == 0) ||
//             (phdle->pInterface->pCanSend == 0) ||
//             (phdle->pInterface->pIOCfg == 0) ||
//             (phdle->pInterface->pWDGCfg == 0) ||
//             (phdle->pInterface->pSaveParams == 0) ||
//             (phdle->pInterface->pResetParams == 0) ||
//             (phdle->pInterface->pEventCtrl == 0) ||
// 			(phdle->pInterface->pBatChgCtrl == 0) ||
// 			(phdle->pInterface->pBatPwrCtrl == 0) ||
// 			(phdle->pInterface->pBootloaderCtrl == 0) ||
//             (phdle->pInterface->pAdcRead == 0) ||
//             (phdle->pInterface->pGetIOStatus == 0) ||
//             (phdle->pInterface->pSetIO == 0) ||
//             (phdle->pInterface->pOwDS18b20Read == 0) ||
// #endif
//             (phdle->pInterface->pPwrCtrl == 0) ||
// 			(phdle->pInterface->pFirmwareErase == 0) ||
// 			(phdle->pInterface->pFirmwareCrcVerify == 0) ||
// 			(phdle->pInterface->pFirmwareFlash == 0)
//           )
//         {
//             return EIPC_P1_ERR_NULL_PTR;
//         }
//     }
//     else
//     {
//         if((phdle->pInterface->pEventCallback == 0))
//         {
//             return EIPC_P1_ERR_NULL_PTR;
//         }
//     }

    phdle->OState.Current_State = EIPC_P1_IDLE_STATE;
    phdle->OState.Next_State = EIPC_P1_IDLE_STATE;
    phdle->Init = 1;

// #ifndef IPC_P1_BOOTLOADER_ENABLE
//     if(phdle->pSettings->Mode == EIPC_P1_MOD_MSTR)
//     {
//         err_code = ipc_p1_get_sys_info(phdle, &SysInfo);

//         if(err_code == EIPC_P1_ERR_NO_ERROR)
//         {
// //            if(SysInfo.ProtRevNumber != IPC_PROT_REV_NUM)
// //            {
// //                phdle->Init = 0;
// //                return EIPC_P1_ERR_SLAVE_PROT_REV_MISMATCH;
// //            }

//         	memcpy(&(phdle->OState.OSysInfo), &SysInfo, sizeof(SysInfo));
//         }
//         else
//         {
//         	phdle->Init = 0;
//         	return err_code;
//         }
//     }
// #endif

    return err_code;
}




