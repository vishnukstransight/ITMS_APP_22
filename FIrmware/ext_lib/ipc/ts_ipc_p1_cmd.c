/*
 * ts_ipc_p1_cmd.c
 *
 *  Created on: 22-Jun-2021
 *      Author: Anil C S
 */

#include "ts_ipc_p1.h"
#include "ts_ipc_p1_cmd.h"
#include "ts_ipc_p1_crc.h"
#include  <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ts_ipc_p1_hdlr.h"
#include <stdio.h>

#ifndef IPC_P1_BOOTLOADER_ENABLE


EIPC_P1_Errors ipc_p1_event_voice_call(SIPC_P1_Handle *phdle,uint8_t *CTX,uint8_t *time )
{
    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
    uint8_t *pFrame;
    uint16_t FrameLen = 0;

    pFrame = (uint8_t*)(*(phdle->pInterface->pmalloc))(IPC_P1_FRAME_SIZE);
   
    if(pFrame==NULL){
       return EIPC_P1_ERR_MALLOC_FAIL;
    }


    FrameLen =  IPC_P1_PAYLOAD_START_INDEX;  //Payload Start

    pFrame[FrameLen++] = (uint8_t) (EIPC_P1_ID_VOICE_CALL_DATA & 0xFF);
    pFrame[FrameLen++] = (uint8_t) ( ((EIPC_P1_ID_VOICE_CALL_DATA >> 8) & 0x0F) | ((EIPC_P1_EVENT << 4) & 0xF0) ) ;

    pFrame[FrameLen++] = 0;

    memcpy(&(pFrame[FrameLen]),CTX, 12);
    FrameLen+=12;
    memcpy(&(pFrame[FrameLen]),time, 32);
    FrameLen+=32;

    err_code = ipc_p1_send_frame(phdle, pFrame, FrameLen);

    (*(phdle->pInterface->pfree))(pFrame);

    return err_code;

}

EIPC_P1_Errors ipc_p1_event_voice_call_response(SIPC_P1_Handle *phdle,uint8_t response )
{
    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
    uint8_t *pFrame;
    uint16_t FrameLen = 0;

    pFrame = (uint8_t*)(*(phdle->pInterface->pmalloc))(IPC_P1_FRAME_SIZE);
   
    if(pFrame==NULL){
       return EIPC_P1_ERR_MALLOC_FAIL;
    }


    FrameLen =  IPC_P1_PAYLOAD_START_INDEX;  //Payload Start

    pFrame[FrameLen++] = (uint8_t) (EIPC_P1_ID_VOICE_CALL_DATA & 0xFF);
    pFrame[FrameLen++] = (uint8_t) ( ((EIPC_P1_ID_VOICE_CALL_DATA >> 8) & 0x0F) | ((EIPC_P1_EVENT << 4) & 0xF0) ) ;

    pFrame[FrameLen++] = 1;
    pFrame[FrameLen++] = response;

    err_code = ipc_p1_send_frame(phdle, pFrame, FrameLen);

    (*(phdle->pInterface->pfree))(pFrame);

    return err_code;
}


EIPC_P1_Errors ipc_p1_event_app_pkt(SIPC_P1_Handle *phdle,uint32_t length, uint8_t *buffer ){

    EIPC_P1_Errors err_code = EIPC_P1_ERR_NO_ERROR;
    uint8_t *pFrame;
    uint16_t FrameLen = 0;

    pFrame = (uint8_t*)(*(phdle->pInterface->pmalloc))(IPC_P1_FRAME_SIZE);
   
    if(pFrame==NULL){
       return EIPC_P1_ERR_MALLOC_FAIL;
    }
   
    FrameLen =  IPC_P1_PAYLOAD_START_INDEX;  //Payload Start

    pFrame[FrameLen++] = (uint8_t) (EIPC_P1_ID_APP_PKT_DATA & 0xFF);
    pFrame[FrameLen++] = (uint8_t) ( ((EIPC_P1_ID_APP_PKT_DATA >> 8) & 0x0F) | ((EIPC_P1_EVENT << 4) & 0xF0) ) ;

    pFrame[FrameLen++] = 0;
     
    memcpy(&pFrame[FrameLen],&length,4);
    FrameLen+= 4;
    memcpy(&pFrame[FrameLen],buffer,length);
    FrameLen+= length;
    err_code = ipc_p1_send_frame(phdle, pFrame, FrameLen);
    
    (*(phdle->pInterface->pfree))(pFrame);

    return err_code;
}

#endif

