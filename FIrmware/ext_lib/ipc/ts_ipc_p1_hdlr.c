/*
 * ts_ipc_p1_cmd_hdlr.c
 *
 *  Created on: 22-Jun-2021
 *      Author: Anil C S
 */



#include "ts_ipc_p1.h"
#include "ts_ipc_p1_cmd.h"
#include "ts_ipc_p1_crc.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ts_ipc_p1_hdlr.h"

EIPC_P1_Errors ipc_p1_error_hdlr(SIPC_P1_Handle *phdle)
{
    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
    if(phdle->pSettings->Mode == EIPC_P1_MOD_SLAVE)
    {
        phdle->OState.pRespBuffer[phdle->OState.RespBuffIndex++] = (uint8_t) 0;
        phdle->OState.pRespBuffer[phdle->OState.RespBuffIndex++] = (uint8_t) 0xFF;
    }
    else
    {
        err_code = EIPC_P1_ERR_INVALID_CMD;
    }
    return err_code;
}

#ifndef IPC_P1_BOOTLOADER_ENABLE

EIPC_P1_Errors ipc_p1_voice_call_hdlr(SIPC_P1_Handle *phdle)
{
    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
    uint8_t index, error_code;
    SIPC_P1_CallData oCallData;
    
    if(phdle->OState.cmd_resp_byte == EIPC_P1_EVENT)
    {   
        while(phdle->OState.PayloadIndex < phdle->OState.PayloadLength){
        index = phdle->OState.pPayload[phdle->OState.PayloadIndex++];
        memcpy(&oCallData.CTX,&(phdle->OState.pPayload[phdle->OState.PayloadIndex]),12);
        phdle->OState.PayloadIndex +=12;
        memcpy(&oCallData.Call_Time,&(phdle->OState.pPayload[phdle->OState.PayloadIndex]),8);
        phdle->OState.PayloadIndex +=8;
        phdle->pInterface->pEventCallback(EIPC_P1_EVT_VOICE_CALL_RXD,(SIPC_P1_CallData *)&oCallData);
        }
    }
    else 
    {
        err_code = EIPC_P1_ERR_INVALID_CMD;
        // do nothing
    }
    return err_code;
}


EIPC_P1_Errors ipc_p1_sms_hdlr(SIPC_P1_Handle *phdle){

    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
    uint8_t index, error_code;

    if(phdle->OState.cmd_resp_byte == EIPC_P1_EVENT)
    {   
        while(phdle->OState.PayloadIndex < phdle->OState.PayloadLength){   
        }
    }
    else 
    {
        err_code = EIPC_P1_ERR_INVALID_CMD;
        // do nothing
    }
    return err_code;
}

EIPC_P1_Errors ipc_p1_app_data_hdlr(SIPC_P1_Handle *phdle){

    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
    uint8_t index, error_code;

    if(phdle->OState.cmd_resp_byte == EIPC_P1_EVENT)
    {   
            
        index = phdle->OState.pPayload[phdle->OState.PayloadIndex++];
        phdle->pInterface->pEventCallback(EIPC_P1_EVT_APP_DATA,&phdle->OState.pPayload[phdle->OState.PayloadIndex]);
    }
    else 
    {
        err_code = EIPC_P1_ERR_INVALID_CMD;
        // do nothing
    }
    return err_code;
}



#endif
