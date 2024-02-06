/*
 * ts_ipc_p1_cmd_hdlr.h
 *
 *  Created on: 22-Jun-2021
 *      Author: Anil C S
 */

#ifndef TS_IPC_P1_HDLR_H_
#define TS_IPC_P1_HDLR_H_

#include "ts_ipc_p1.h"

EIPC_P1_Errors ipc_p1_error_hdlr(SIPC_P1_Handle *phdle);

#ifndef IPC_P1_BOOTLOADER_ENABLE


EIPC_P1_Errors ipc_p1_voice_call_hdlr(SIPC_P1_Handle *phdle);

EIPC_P1_Errors ipc_p1_sms_hdlr(SIPC_P1_Handle *phdle);

EIPC_P1_Errors ipc_p1_app_data_hdlr(SIPC_P1_Handle *phdle);

#endif

#endif /* TS_IPC_P1_HDLR_H_ */
