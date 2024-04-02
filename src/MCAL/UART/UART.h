#ifndef E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_MCAL_UART_H_
#define E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_MCAL_UART_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "../../lib/Errors.h"
#include "../../lib/STD_TYPES.h"
#include "../../lib/Common_Macros.h"

/* Driver Libraries   */
#include "UART_Reg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define __IO       volatile

#define UART_ERROR_NONE              0x00000000U   		/*!< No error            */
#define UART_ERROR_PE                UART_SR_PE_Msk	/*0x00000001U*/   /*!< Parity error        */
#define UART_ERROR_FE                UART_SR_FE_Msk	/*0x00000002U*/   /*!< Frame error         */
#define UART_ERROR_NE                UART_SR_NE_Msk	/*0x00000004U*/   /*!< Noise error         */
#define UART_ERROR_ORE               UART_SR_ORE_Msk	/*0x00000008U*/   /*!< Overrun error       */
#define UART_ERROR_DMA               0x00000010U		/*!< DMA transfer error  */

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef enum
{
    UART_Channel_1 = 0,
    UART_Channel_2,
    UART_Channel_6
} UART_Channel_t;

typedef enum
{
    UART_DISABLE,
    UART_ENABLE
} UART_State_t;

typedef enum
{
    UART_WORDLENGTH_8B,
    UART_WORDLENGTH_9B
} UART_WordLength_t;

typedef enum
{
    UART_1_BIT_Stop,
    UART_2_BIT_Stop
} UART_StopBits_t;

typedef enum
{
    UART_DISABLE_DMA,
    UART_ENABLE_DMAT,
    UART_ENABLE_DMAR,
    UART_ENABLE_DMA_FD

} UART_DMAState_t;

typedef enum
{
    UART_BR_9600,
    UART_BR_19200

} UART_BaudRate_t;

typedef enum
{
    UART_ParityDisable,
    UART_ParityEnable_Even,
    UART_ParityEnable_Odd,
} UART_ParityCtrl_t;

typedef enum
{
    UART_Transmit,
    UART_Receive,
    UART_FullDuplex

} UART_Mode_t;

typedef enum
{
    UART_TXE,
    UART_RXE,
    UART_TC
} UART_InterruptFlag_t;

typedef enum
{
    UART_OVER16 = 0,
    UART_OVER8 = 1
} UART_OverSampling_t;

/*******************************************************************************/

typedef struct
{
    UART_Mode_t mode;
    UART_State_t state;
    UART_Channel_t channel;
    UART_StopBits_t stopBitNum;
    UART_DMAState_t dmaState;
    UART_BaudRate_t baudRate;
    UART_WordLength_t wordLength;
    UART_ParityCtrl_t parityControl;
    UART_OverSampling_t overSampling;

    uint8_t TransDateRegisterEmpty_Int;
    uint8_t TransComplete_Int;
    uint8_t ReceiveDateRegisterEmpty_Int;

} UART_InitStruct;

/*UART Data storage to send*/
typedef struct
{
    pu8_t data;
    uint32_t size;
    UART_Channel_t channel;
} UART_strConfigTransmitBuffer;

/*UART Data storage to Receive*/
typedef struct
{
    pu8_t data;
    uint32_t size;
    uint32_t index;
    UART_Channel_t channel;
} UART_strConfigReceiveBuffer;

/*****************************************************************************/

/*callback function*/
typedef void (*CallBack)(void);

uint8_t bufferData[3];



typedef enum
{
  UART_STATE_RESET             = 0x00U,    /*!< Peripheral is not yet Initialized
                                                   Value is allowed for gState and RxState */
  UART_STATE_READY             = 0x20U,    /*!< Peripheral Initialized and ready for use
                                                   Value is allowed for gState and RxState */
  UART_STATE_BUSY              = 0x24U,    /*!< an internal process is ongoing
                                                   Value is allowed for gState only */
  UART_STATE_BUSY_TX           = 0x21U,    /*!< Data Transmission process is ongoing
                                                   Value is allowed for gState only */
  UART_STATE_BUSY_RX           = 0x22U,    /*!< Data Reception process is ongoing
                                                   Value is allowed for RxState only */
  UART_STATE_BUSY_TX_RX        = 0x23U,    /*!< Data Transmission and Reception process is ongoing
                                                   Not to be used for neither gState nor RxState.
                                                   Value is result of combination (Or) between gState and RxState values */
  UART_STATE_TIMEOUT           = 0xA0U,    /*!< Timeout state
                                                   Value is allowed for gState only */
  UART_STATE_ERROR             = 0xE0U     /*!< Error
                                                   Value is allowed for gState only */
} UART_STATE_t;


typedef struct UART_ManagerStruct
{
  UART_RegStruct				*Instance;      /*!< UART registers base address        */

  UART_InitStruct   	    	init;             /*!< UART communication parameters      */

  const uint8_t                 *pTxBuffPtr;      /*!< Pointer to UART Tx transfer Buffer */

  uint16_t                      TxXferSize;       /*!< UART Tx Transfer size              */

  __IO uint16_t                 TxXferCount;      /*!< UART Tx Transfer Counter           */

  uint8_t                       *pRxBuffPtr;      /*!< Pointer to UART Rx transfer Buffer */

  uint16_t                      RxXferSize;       /*!< UART Rx Transfer size              */

  __IO uint16_t                 RxXferCount;      /*!< UART Rx Transfer Counter           */

//   __IO HAL_UART_RxTypeTypeDef ReceptionType;      /*!< Type of ongoing reception          */
// 
//   DMA_HandleTypeDef             *hdmatx;          /*!< UART Tx DMA Handle parameters      */
// 
//   DMA_HandleTypeDef             *hdmarx;          /*!< UART Rx DMA Handle parameters      */
// 
//   HAL_LockTypeDef               Lock;             /*!< Locking object                     */

  __IO UART_STATE_t		    	txState;           /*!< UART state information related to global Handle management
                                                       and also related to Tx operations.
                                                       This parameter can be a value of @ref HAL_UART_StateTypeDef */

  __IO UART_STATE_t		    	rxState;          /*!< UART state information related to Rx operations.
                                                       This parameter can be a value of @ref HAL_UART_StateTypeDef */

  __IO uint32_t                 ErrorCode;        /*!< UART Error code                    */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  void (* TxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Tx Half Complete Callback        */
  void (* TxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Tx Complete Callback             */
  void (* RxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Rx Half Complete Callback        */
  void (* RxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Rx Complete Callback             */
  void (* ErrorCallback)(struct __UART_HandleTypeDef *huart);             /*!< UART Error Callback                   */
  void (* AbortCpltCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Abort Complete Callback          */
  void (* AbortTransmitCpltCallback)(struct __UART_HandleTypeDef *huart); /*!< UART Abort Transmit Complete Callback */
  void (* AbortReceiveCpltCallback)(struct __UART_HandleTypeDef *huart);  /*!< UART Abort Receive Complete Callback  */
  void (* WakeupCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Wakeup Callback                  */
  void (* RxEventCallback)(struct __UART_HandleTypeDef *huart, uint16_t Pos); /*!< UART Reception Event Callback     */

  void (* MspInitCallback)(struct __UART_HandleTypeDef *huart);           /*!< UART Msp Init callback                */
  void (* MspDeInitCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Msp DeInit callback              */
#endif  /* USE_HAL_UART_REGISTER_CALLBACKS */

} UART_ManagerStruct;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

extern UART_ErrorStatus_t UART_Init(const UART_InitStruct *ConfigPtr);

UART_ErrorStatus_t UART_startTransmit_it(UART_ManagerStruct* Ptr_uartxManger,  const uint8_t *pData, uint16_t Size);

UART_ErrorStatus_t UART_startRecieve_it(UART_ManagerStruct *Ptr_uartxManger, uint8_t *pData, uint16_t Size);

void UART_sendByte_polling(UART_ManagerStruct *usartxManger, const uint8_t data);

uint8_t UART_recieveByte_polling(UART_ManagerStruct *usartxManger);

void HAL_UART_IRQHandler(UART_ManagerStruct *usartxManger);

/***********************************************************************************************************************/
extern UART_ErrorStatus_t UART_SendByteAsynchronous(UART_Channel_t Channel, uint8_t Copy_u8Data);

extern UART_ErrorStatus_t UART_ReceiveBuffer(const UART_strConfigReceiveBuffer *ReceiveBuffer);

extern UART_ErrorStatus_t UART_SendBufferZeroCopy(const UART_strConfigTransmitBuffer *Copy_sConfigBuffer);

extern UART_ErrorStatus_t UART_RegisterCallBackFunction(UART_Channel_t Channel, UART_Mode_t Mode, CallBack Add_CallBackFunction);

/***********************************************************************************************************************************/

extern UART_ErrorStatus_t UART_ReceiveBufferDma(void);

extern UART_ErrorStatus_t UART_SendBufferDma(void);

#endif // E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_MCAL_UART_H_