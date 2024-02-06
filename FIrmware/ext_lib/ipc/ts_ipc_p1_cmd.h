/*
 * ts_ipc_p1_cmd.h
 *
 *  Created on: 22-Jun-2021
 *      Author: Anil C S
 */

#ifndef TS_IPC_P1_CMD_H_
#define TS_IPC_P1_CMD_H_

#include "ts_ipc_p1.h"


#ifndef IPC_P1_BOOTLOADER_ENABLE

EIPC_P1_Errors ipc_p1_event_app_pkt(SIPC_P1_Handle *phdle,uint32_t length, uint8_t *buffer );

EIPC_P1_Errors ipc_p1_event_voice_call_response(SIPC_P1_Handle *phdle,uint8_t response );

EIPC_P1_Errors ipc_p1_event_voice_call(SIPC_P1_Handle *phdle,uint8_t *CTX,uint8_t *time );
#endif

#endif /* TS_IPC_P1_CMD_H_ */
