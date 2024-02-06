#ifndef EXT_LIB_CODE_LIB_IPC_IPC1_TS_IPC_P1
#define EXT_LIB_CODE_LIB_IPC_IPC1_TS_IPC_P1

/*

*/

#include <stdint.h>
#include <stdarg.h>

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define IPC_PROT_CODE       "TS_IPC_PROT_001"
#define IPC_PROT_REV_NUM    6

#define IPC_P1_FRAME_SIZE               1030
#define IPC_P1_FRAME_HEAD_START_INDEX   0
#define IPC_P1_FRAME_HEAD_SIZE          3
#define IPC_P1_PAYLOAD_START_INDEX      3
#define IPC_P1_MAX_PAYLOAD_SIZE         1024
#define IPC_P1_PAYLOAD_END_INDEX        (1030-4)
#define IPC_P1_FRAME_TAIL_START_INDEX   (1030-3)

#define IPC_P1_START_BYTE                   (const uint8_t)     0xAA
#define IPC_P1_STOP_BYTE                    (const uint8_t)     0x55


#define IPC_P1_MAX_HW_CODE_SIZE     16
#define IPC_P1_MAX_FW_CODE_SIZE     16
#define IPC_P1_MAX_PROT_CODE_SIZE   16
#define NUM_OF_IDFS                 0x01


#ifndef IPC_P1_CAN_FILTER_NUM
#define IPC_P1_CAN_FILTER_NUM       8
#endif


#define IPC_P1_SET_CMD  1
#define IPC_P1_GET_CMD  2

typedef enum
{
    EIPC_P1_ERR_NO_ERROR = 0,
    EIPC_P1_ERR_HDLE_NOT_INIT,
    EIPC_P1_ERR_NULL_PTR,
    EIPC_P1_ERR_PROT_REV_MISMATCH,
    EIPC_P1_ERR_SLAVE_PROT_REV_MISMATCH,
    EIPC_P1_ERR_CMD_LOCK_FAIL,
	EIPC_P1_ERR_EVENT_WAIT_FAIL,
    EIPC_P1_ERR_MALLOC_FAIL,
	EIPC_P1_ERR_SEND_PKT_FAIL,
    EIPC_P1_ERR_BUFFER_SIZE,
    EIPC_P1_ERR_INVALID_MODE,
	EIPC_P1_ERR_START_BYTE,
	EIPC_P1_ERR_PAYLOAD_LEN_STATE1,
	EIPC_P1_ERR_PAYLOAD_LEN_STATE2,
	EIPC_P1_ERR_PAYLOAD_STATE,
	EIPC_P1_ERR_CRC_STATE1,
	EIPC_P1_ERR_CRC_STATE2,
	EIPC_P1_ERR_STOP_STATE,
	EIPC_P1_ERR_INVALID_CRC,
	EIPC_P1_ERR_INVALID_STATE,
	EIPC_P1_ERR_INVALID_INDEX,
	EIPC_P1_ERR_RXT_ERROR,
	EIPC_P1_ERR_RXD_EVENT_ERROR,
	EIPC_P1_ERR_RXD_DATA_SIZE_ERROR,
	EIPC_P1_ERR_FN_ARG_ERROR,
	EIPC_P1_ERR_INVALID_CMD,
	EIPC_P1_ERR_INVALID_WDG_NUM,
	EIPC_P1_ERR_RXD_INDEX_COUNT_ERROR,
	EIPC_P1_ERR_RXD_INDEX_ERROR,
	EIPC_P1_ERR_RXD_PKT_HAS_ERROR,
	EIPC_P1_ERR_PAYLOAD_SIZE_OVER_FLOW,
	EIPC_P1_ERR_INVALID_WDT_INDEX = 0x100,
	EIPC_P1_ERR_CAN_CFG_FAIL,
	EIPC_P1_ERR_MAX_PAYLOAD_EXCEEDED,
	EIPC_P1_ERR_MAX_IDF_EXCEEDED,
	EIPC_P1_ERR_MAX_INVALID_IDF,
	EIPC_P1_ERR_FW_ADDR_OUT_OF_RANGE,
	EIPC_P1_ERR_FW_ERASE_FAIL,
	EIPC_P1_ERR_FW_FLASH_FAIL,
	EIPC_P1_ERR_FW_CRC_FAIL,
	EIPC_P1_ERR_FW_CRC_VERIFY_FAIL,
	EIPC_P1_ERR_PWR_CTRL_FAIL,
	EIPC_P1_ERR_OW_READ_FAIL,
}EIPC_P1_Errors;

typedef enum
{
    EIPC_P1_ES_ERR_NO_ERRORS = 0,
    EIPC_P1_ES_ERR_APP_FW_CRC_CHECK_FAIL,
}EIPC_P1_ErrorStates;

typedef enum
{
    EIPC_P1_EVT_IO_CHG = 10,             //Digital IO
    EIPC_P1_EVT_UART_DATA_RXD,
    EIPC_P1_EVT_CAN_MSG_RXD,
	EIPC_P1_EVT_SAVE_ALL_PARAMETERS,
	EIPC_P1_EVT_RESET_ALL_PARAMETERS,
	EIPC_P1_EVT_START_EVENT_ALERTS,
	EIPC_P1_EVT_ADC_GET_RSP_RXD,
	EIPC_P1_EVT_WDG_CFG_GET_RSP_RXD,
	EIPC_P1_EVT_WDG_CFG_SET_RSP_RXD,
	EIPC_P1_EVT_PWR_CTRL_SET_RSP_RXD,
	EIPC_P1_EVT_SYS_INFO_GET_RSP_RXD,
	EIPC_P1_EVT_SAVE_PARAMS_SET_RSP_RXD,
	EIPC_P1_EVT_RESET_PARAMS_SET_RSP_RXD,
	EIPC_P1_EVT_EVENT_CONTROL_SET_RSP_RXD,
	EIPC_P1_EVT_BAT_CHG_CTRL_SET_RSP_RXD,
	EIPC_P1_EVT_BAT_CHG_CTRL_GET_RSP_RXD,
	EIPC_P1_EVT_BAT_PWR_CTRL_SET_RSP_RXD,
	EIPC_P1_EVT_BAT_PWR_CTRL_GET_RSP_RXD,
	EIPC_P1_EVT_BOOTLOADER_CTRL_SET_RSP_RXD,
	EIPC_P1_EVT_SIM_SEL_CTRL_SET_RSP_RXD,
    EIPC_P1_EVT_SIM_SEL_CTRL_GET_RSP_RXD,
	EIPC_P1_EVT_FIRMWARE_ERASE_SET_RSP_RXD,
	EIPC_P1_EVT_FIRMWARE_CRC_SET_RSP_RXD,
	EIPC_P1_EVT_FIRMWARE_FLASH_SET_RSP_RXD,
	EIPC_P1_EVT_IO_CONFIG_GET_RSP_RXD,
	EIPC_P1_EVT_IO_CONFIG_SET_RSP_RXD,
	EIPC_P1_EVT_IO_GET_RSP_RXD,
	EIPC_P1_EVT_IO_SET_RSP_RXD,
	EIPC_P1_EVT_UART_CONFIG_GET_RSP_RXD,
	EIPC_P1_EVT_UART_CONFIG_SET_RSP_RXD,
	EIPC_P1_EVT_SEND_UART_DATA_SET_RSP_RXD,
	EIPC_P1_EVT_CAN_CONFIG_SET_RSP_RXD,
	EIPC_P1_EVT_CAN_CONFIG_GET_RSP_RXD,
	EIPC_P1_EVT_SEND_CAN_DATA_SET_RSP_RXD,
	EIPC_P1_EVT_ONE_WIRE_DATA_GET_RSP_RXD,
    EIPC_P1_EVT_VOICE_CALL_RXD,
    EIPC_P1_EVT_VOICE_CALL_RSP,
    EIPC_P1_EVT_APP_DATA,

}EIPC_P1_Event;

typedef enum
{
    /*
    EIPC_P1_MOD_MCU,
    EIPC_P1_MOD_MPU,
    
    */
    EIPC_P1_MOD_MSTR =0,
    EIPC_P1_MOD_SLAVE,
}EIPC_P1_Mode;

typedef enum
{
    EIPC_P1_ID_ADC = 0x100,
    EIPC_P1_ID_MASTER_WATCHDOG_CONFIG,
    EIPC_P1_ID_POWER_CONTROL,
    EIPC_P1_ID_SYSTEM_INFO,
    EIPC_P1_ID_SAVE_PARAMETERS,
    EIPC_P1_ID_RESET_PARAMETERS,
    EIPC_P1_ID_EVENT_CONTROL,
    EIPC_P1_ID_BATTERY_CHARGE_CONTROL,
    EIPC_P1_ID_BATTERY_POWER_CONTROL,
    EIPC_P1_ID_BOOTLOADER_CONTROL,
    EIPC_P1_ID_SIM_SELECT_CONTROL,
    EIPC_P1_ID_FIRMWARE_ERASE = 0x150,
    EIPC_P1_ID_FIRMWARE_CRC_VERIFY,
    EIPC_P1_ID_FIRMWARE_FLASH,
    EIPC_P1_ID_PIN_CONFIG = 0x200,
    EIPC_P1_ID_PIN_CONTROL,
    EIPC_P1_ID_PIN_STATUS,
    EIPC_P1_ID_UART_CONFIG = 0x250,
    EIPC_P1_ID_UART_DATA,
    EIPC_P1_ID_CAN_CONFIG = 0x300,
    EIPC_P1_ID_CAN_DATA,
    EIPC_P1_ID_CAN_CONFIGURATOR_CFG,
    EIPC_P1_ID_CAN_CONFIGURATOR_TX,
    EIPC_P1_ID_CAN_CONFIGURATOR_RX,
    EIPC_P1_ID_ONE_WIRE_DATA = 0x350,
    EIPC_P1_ID_ONE_WIRE_CONFIG,
    EIPC_P1_ID_SIMPLE_EV_DATA = 0x400,
    EIPC_P1_ID_VOICE_CALL_DATA = 0x450,
    EIPC_P1_ID_SMS_DATA ,
    EIPC_P1_ID_APP_PKT_DATA = 0x500,
}EIPC_P1_IDENTIFIER;


typedef enum
{
    EIPC_P1_CMD_NACK = 0,
    EIPC_P1_CMD_ACK,
}EIPC_P1_CmdAckMode;

typedef enum
{
    EIPC_P1_EVENT = 0x00,
	EIPC_P1_GET_CMD,
	EIPC_P1_GET_RESP,
	EIPC_P1_SET_CMD,
	EIPC_P1_SET_RESP,
	EIPC_P1_SET_CMD_NACK
}EIPC_P1_CMD_RESPONSE;


typedef enum
{
    EIPC_P1_IDLE_STATE = 0,
    EIPC_P1_PAYLOAD_LEN_STATE1,
	EIPC_P1_PAYLOAD_LEN_STATE2,
    EIPC_P1_PAYLOAD_STATE,
    EIPC_P1_CRC_STATE1,
	EIPC_P1_CRC_STATE2,
    EIPC_P1_STOP_STATE
}EIPC_P1_State;

typedef enum
{

    EIPC_P1_UART_BAUD_4800 = 4800,
	EIPC_P1_UART_BAUD_9600 = 9600,
	EIPC_P1_UART_BAUD_921600 = 921600,

}EIPC_P1_UART_Baudrate;

typedef enum
{
    EIPC_P1_UART_PARITY_NONE = 0,
    EIPC_P1_UART_PARITY_ODD,
    EIPC_P1_UART_PARITY_EVEN,
}EIPC_P1_UART_Parity;

typedef enum
{
    EIPC_P1_UART_STOPBITS_ONE = 1,
    EIPC_P1_UART_STOPBITS_TWO,
}EIPC_P1_UART_StopBits;

typedef enum
{
    EIPC_P1_UART_BYTE_SIZE_7 = 7,
    EIPC_P1_UART_BYTE_SIZE_8,
    EIPC_P1_UART_BYTE_SIZE_9,
}EIPC_P1_UART_BYTE_SIZE;

typedef enum
{
    EIPC_P1_IO_MOD_INPUT = 0,
    EIPC_P1_IO_MOD_OUTPUT,
    EIPC_P1_IO_MOD_ADC,
}EIPC_P1_IO_Mode;

typedef enum
{
    EIPC_P1_IO_ADC_MOD_VL = 0,  //Voltage Loop
    EIPC_P1_IO_ADC_MOD_CL,      //Current Loop      
}EIPC_P1_ADC_Mode;

typedef enum
{
    EIPC_P1_IO_PATTERN_DISABLE,
    EIPC_P1_IO_PATTERN_ENABLE,
}EIPC_P1_IO_Pattern_Ctrl;

typedef enum
{
    EIPC_P1_SYS_RESTART = 1,
    EIPC_P1_MSTR_RESTART_NORMAL,
    EIPC_P1_MSTR_RESTART_FAST,
}EIPC_P1_PowerControl_Type;


typedef enum
{
    EIPC_P1_CAN_ID_STANDARD = 0,
    EIPC_P1_CAN_ID_EXTENDED,
    EIPC_P1_CAN_ID_MIXED,
}EIPC_P1_CanIdType;

typedef enum
{
    EIPC_P1_CAN_DATA_FRAME = 0,
    EIPC_P1_CAN_REMOTE_FRAME
}EIPC_P1_CanFrameType;

typedef enum
{
    EIPC_P1_CAN_BAUD_10K  = 10000,
	EIPC_P1_CAN_BAUD_20K  = 20000,
	EIPC_P1_CAN_BAUD_50K  = 50000,
	EIPC_P1_CAN_BAUD_100K = 100000,
	EIPC_P1_CAN_BAUD_125K = 125000,
	EIPC_P1_CAN_BAUD_200K = 200000,
	EIPC_P1_CAN_BAUD_250K = 250000,
	EIPC_P1_CAN_BAUD_400K = 400000,
	EIPC_P1_CAN_BAUD_5M   = 5000000,
	EIPC_P1_CAN_BAUD_8M   = 8000000,
	EIPC_P1_CAN_BAUD_10M  = 10000000
}EIPC_P1_CanBaudrate;

typedef enum
{
	EIPC_P1_BLR_FW,
	EIPC_P1_APP_FW,

}EIPC_P1_FwType;


typedef struct
{
    uint16_t ADC;
}SIPC_P1_Adc;


typedef struct
{
    EIPC_P1_UART_Baudrate Baudrate;
    EIPC_P1_UART_Parity Parity;
    EIPC_P1_UART_StopBits Stopbits;
    EIPC_P1_UART_BYTE_SIZE ByteSize;
}SIPC_P1_UartCfg;

typedef struct
{
    uint8_t  Index;
    uint8_t *pDataBuffer;
    uint32_t DataSize;
}SIPC_P1_UartData;

typedef struct
{
    EIPC_P1_IO_Mode PinMode;
    EIPC_P1_ADC_Mode ADCMode;
    EIPC_P1_IO_Pattern_Ctrl PinPatternCtrl;
    uint32_t PinPattern;
}SIPC_P1_IOCfg;

typedef struct
{
    uint16_t CtrlByte;
    uint16_t MaskByte;
}SIPC_P1_IOCtrl;

typedef struct
{
    EIPC_P1_CanIdType ide;
    EIPC_P1_CanFrameType rtr;
    uint32_t id;
}SIPC_P1_CanID;

typedef struct
{
    SIPC_P1_CanID ID;
    uint32_t Mask;
}SIPC_P1_CanFilter;

typedef struct
{
    SIPC_P1_CanID ID;
    uint8_t DLC;
    uint8_t DATA[8];
}SIPC_P1_CanFrame;

typedef struct
{
    EIPC_P1_CanBaudrate Baudrate;
    EIPC_P1_CanIdType IDMode;
    SIPC_P1_CanFilter Filter[IPC_P1_CAN_FILTER_NUM];
}SIPC_P1_CanCfg;

typedef struct
{
  uint8_t  Index;
  SIPC_P1_CanFrame Frame;
}SIPC_P1_CanData;

typedef struct 
{
    uint8_t Enable; //1 - Enabled, 0 Disabled
    uint32_t Timeout_ms; //Watchdog Timeout in millisecs
}SIPC_P1_WdgCfg;

typedef struct 
{
	uint8_t FwType;
    char HwCode[IPC_P1_MAX_HW_CODE_SIZE];
    char AppFwCode[IPC_P1_MAX_FW_CODE_SIZE];
    uint16_t AppFwBuildNumber;
    uint32_t AppFwStartAddress;
    uint32_t AppFwMaxSize;
    char BlrFwCode[IPC_P1_MAX_FW_CODE_SIZE];
    uint16_t BlrFwBuildNumber;
    uint32_t BlrFwStartAddress;
    uint32_t BlrFwMaxSize;
    uint8_t ErrorState;
    char ProtCode[IPC_P1_MAX_PROT_CODE_SIZE];
    uint16_t ProtRevNumber;
    uint8_t NumUart;
    uint8_t NumCan;
    uint8_t NumIO;
    uint8_t NumADC;
    uint8_t NumCanFilter;
}SIPC_P1_SystemInfo;

typedef struct
{
  uint8_t CTX[12];
  uint64_t Call_Time;
}SIPC_P1_CallData;


typedef struct
{

    /*
        This call back function will be invoked for the following events.
        When Event == EIPC_P1_EVT_IO_CHG, then pParameter will be a pointer to uint16_t which indicaes IO status
        When Event == EIPC_P1_EVT_CAN_MSG_RXD, then pParameter will be a pointer to SIPC_P1_CAN_Msg object
        When Event == EIPC_P1_EVT_UART_DATA_RXD, then pParameter will be a pointer to SIPC_P1_UART_data object
    */
    void    (*pEventCallback)(EIPC_P1_Event Event,void *pParameter);

    // Dynamic memory allocation functions
    void* (*pmalloc)(uint32_t Size);
    void (*pfree)(void * ptr);

    EIPC_P1_Errors (*pGetCmdLock)(uint32_t Waitms);

    void (*pRelaseCmdLock)();

    EIPC_P1_Errors (*pSendPacket)(uint8_t *pData,uint32_t Length);

    void (*pSendEvent)(EIPC_P1_Event event);

    EIPC_P1_Errors (*pWaitForEvent)(EIPC_P1_Event event,uint32_t Waitms);
	
	void  (*pDebugPrint)(uint32_t DbgLevel,
								const char *pStartStr,
								const char *pStr,
								va_list args);

}SIPC_P1_Interface;

typedef struct
{
    EIPC_P1_Mode const Mode;
    uint32_t const DebugPrintEnabled;
//    uint32_t max_payload_size;
    SIPC_P1_SystemInfo OSysInfo;
}SIPC_P1_Settings;

typedef struct
{
	uint32_t AckExpected; // 1 - Ack Expecting , 0 - Not Expecting
	uint16_t ExpectedAckID;
	uint32_t AckRxdDataSize;
	uint8_t *pAckBuff;
	uint32_t AckBuffSize;
}SIPC_P1_AckState;


typedef struct
{
	SIPC_P1_SystemInfo OSysInfo;
	//EIPC_P1_Errors AckError;
	SIPC_P1_AckState OAckState;

	EIPC_P1_State Current_State;
	EIPC_P1_State Next_State;

	uint16_t PayloadLength;
	uint16_t PayloadIndex;
	uint8_t *pPayload;
	uint16_t Crc;

	uint8_t cmd_resp_byte;
	uint16_t identifier;

	uint8_t *pRespBuffer;
	uint16_t RespBuffIndex;

//	SIPC_P1_Data Rxd_Data;
}SIPC_P1_State;

typedef struct
{
    uint8_t Init;

    SIPC_P1_State OState;
    SIPC_P1_Settings *pSettings;
    SIPC_P1_Interface const *pInterface;

}SIPC_P1_Handle;


EIPC_P1_Errors ipc_p1_init(SIPC_P1_Handle *phdle);

EIPC_P1_Errors ipc_p1_send_frame(SIPC_P1_Handle *phdle, uint8_t *pBuffer, uint16_t Length);

EIPC_P1_Errors ipc_p1_frame_handler(SIPC_P1_Handle *phdle,uint8_t *pData, uint32_t Length);

void TS_IPC_P1_DEBUG_PRINT(SIPC_P1_Handle *pHdle,uint32_t DbgLevel, const char *pStr,...);


#endif /* EXT_LIB_CODE_LIB_IPC_IPC1_TS_IPC_P1 */
