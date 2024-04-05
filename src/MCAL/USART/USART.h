#ifndef E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_MCAL_USART_H_
#define E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_MCAL_USART_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "Errors.h"
#include "STD_TYPES.h"
#include "Common_Macros.h"

/* Driver Libraries   */
#include "USART_Reg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define __IO       volatile

#define USART_ERROR_NONE              0x00000000U   		/*!< No error            */
#define USART_ERROR_PE                USART_SR_PE_Msk	/*0x00000001U*/   /*!< Parity error        */
#define USART_ERROR_FE                USART_SR_FE_Msk	/*0x00000002U*/   /*!< Frame error         */
#define USART_ERROR_NE                USART_SR_NE_Msk	/*0x00000004U*/   /*!< Noise error         */
#define USART_ERROR_ORE               USART_SR_ORE_Msk	/*0x00000008U*/   /*!< Overrun error       */
#define USART_ERROR_DMA               0x00000010U		/*!< DMA transfer error  */

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef enum
{
    USART_Channel_1 = 0,
    USART_Channel_2,
    USART_Channel_6
} USART_Channel_t;

typedef enum
{
    USART_DISABLE,
    USART_ENABLE
} USART_EN_DIS_State_t;

typedef enum
{
    USART_WORDLENGTH_8B,
    USART_WORDLENGTH_9B
} USART_WordLength_t;

typedef enum
{
    USART_1_BIT_Stop,
    USART_2_BIT_Stop
} USART_StopBits_t;

typedef enum
{
    USART_DISABLE_DMA,
    USART_ENABLE_DMAT,
    USART_ENABLE_DMAR,
    USART_ENABLE_DMA_FD

} USART_DMAState_t;

typedef enum
{
    USART_BR_9600,
    USART_BR_115200

} USART_BaudRate_t;

typedef enum
{
    USART_ParityDisable,
    USART_ParityEnable_Even,
    USART_ParityEnable_Odd,
} USART_ParityCtrl_t;

typedef enum
{
    USART_Transmit,
    USART_Receive,
    USART_FullDuplex

} USART_Mode_t;

typedef enum
{
    USART_TXE,
    USART_RXE,
    USART_TC
} USART_InterruptFlag_t;

typedef enum
{
    USART_OVER16 = 0,
    USART_OVER8 = 1
} USART_OverSampling_t;

typedef enum
{
  USART_STATE_RESET             = 0x00U,    /*!< Peripheral is not yet Initialized
                                                   Value is allowed for gState and RxState */
  USART_STATE_READY             = 0x20U,    /*!< Peripheral Initialized and ready for use
                                                   Value is allowed for gState and RxState */
  USART_STATE_BUSY              = 0x24U,    /*!< an internal process is ongoing
                                                   Value is allowed for gState only */
  USART_STATE_BUSY_TX           = 0x21U,    /*!< Data Transmission process is ongoing
                                                   Value is allowed for gState only */
  USART_STATE_BUSY_RX           = 0x22U,    /*!< Data Reception process is ongoing
                                                   Value is allowed for RxState only */
  USART_STATE_BUSY_TX_RX        = 0x23U,    /*!< Data Transmission and Reception process is ongoing
                                                   Not to be used for neither gState nor RxState.
                                                   Value is result of combination (Or) between gState and RxState values */
  USART_STATE_TIMEOUT           = 0xA0U,    /*!< Timeout state
                                                   Value is allowed for gState only */
  USART_STATE_ERROR             = 0xE0U     /*!< Error
                                                   Value is allowed for gState only */
} USART_STATE_t;

/*******************************************************************************/

typedef struct
{
    uint64_t baudRate;
    USART_Mode_t mode;
    USART_STATE_t state;
    USART_EN_DIS_State_t en_dis_state;
    USART_Channel_t channel;
    USART_StopBits_t stopBitNum;
    USART_DMAState_t dmaState;
    USART_WordLength_t wordLength;
    USART_ParityCtrl_t parityControl;
    USART_OverSampling_t overSampling;

} USART_InitStruct;


// /*USART Data storage to send*/
// typedef struct
// {
//     pu8_t data;
//     uint32_t size;
//     USART_Channel_t channel;
// } USART_strConfigTransmitBuffer;

// /*USART Data storage to Receive*/
// typedef struct
// {
//     pu8_t data;
//     uint32_t size;
//     uint32_t index;
//     USART_Channel_t channel;
// } USART_strConfigReceiveBuffer;

/*****************************************************************************/


typedef struct USART_ManagerStruct
{
  USART_RegStruct				*Instance;      /*!< USART registers base address        */

  USART_InitStruct   	    	init;             /*!< USART communication parameters      */

  const uint8_t                 *pTxBuffPtr;      /*!< Pointer to USART Tx transfer Buffer */

  uint16_t                      TxXferSize;       /*!< USART Tx Transfer size              */

  __IO uint16_t                 TxXferCount;      /*!< USART Tx Transfer Counter           */

  uint8_t                       *pRxBuffPtr;      /*!< Pointer to USART Rx transfer Buffer */

  uint16_t                      RxXferSize;       /*!< USART Rx Transfer size              */

  __IO uint16_t                 RxXferCount;      /*!< USART Rx Transfer Counter           */

//   __IO HAL_USART_RxTypeTypeDef ReceptionType;      /*!< Type of ongoing reception          */
// 
//   DMA_HandleTypeDef             *hdmatx;          /*!< USART Tx DMA Handle parameters      */
// 
//   DMA_HandleTypeDef             *hdmarx;          /*!< USART Rx DMA Handle parameters      */
// 
//   HAL_LockTypeDef               Lock;             /*!< Locking object                     */

  __IO USART_STATE_t		    	txState;           /*!< USART state information related to global Handle management
                                                       and also related to Tx operations.
                                                       This parameter can be a value of @ref HAL_USART_StateTypeDef */

  __IO USART_STATE_t		    	rxState;          /*!< USART state information related to Rx operations.
                                                       This parameter can be a value of @ref HAL_USART_StateTypeDef */

  __IO uint32_t                 ErrorCode;        /*!< USART Error code                    */

#if (USE_HAL_USART_REGISTER_CALLBACKS == 1)
  void (* TxHalfCpltCallback)(struct __USART_HandleTypeDef *huart);        /*!< USART Tx Half Complete Callback        */
  void (* TxCpltCallback)(struct __USART_HandleTypeDef *huart);            /*!< USART Tx Complete Callback             */
  void (* RxHalfCpltCallback)(struct __USART_HandleTypeDef *huart);        /*!< USART Rx Half Complete Callback        */
  void (* RxCpltCallback)(struct __USART_HandleTypeDef *huart);            /*!< USART Rx Complete Callback             */
  void (* ErrorCallback)(struct __USART_HandleTypeDef *huart);             /*!< USART Error Callback                   */
  void (* AbortCpltCallback)(struct __USART_HandleTypeDef *huart);         /*!< USART Abort Complete Callback          */
  void (* AbortTransmitCpltCallback)(struct __USART_HandleTypeDef *huart); /*!< USART Abort Transmit Complete Callback */
  void (* AbortReceiveCpltCallback)(struct __USART_HandleTypeDef *huart);  /*!< USART Abort Receive Complete Callback  */
  void (* WakeupCallback)(struct __USART_HandleTypeDef *huart);            /*!< USART Wakeup Callback                  */
  void (* RxEventCallback)(struct __USART_HandleTypeDef *huart, uint16_t Pos); /*!< USART Reception Event Callback     */

  void (* MspInitCallback)(struct __USART_HandleTypeDef *huart);           /*!< USART Msp Init callback                */
  void (* MspDeInitCallback)(struct __USART_HandleTypeDef *huart);         /*!< USART Msp DeInit callback              */
#endif  /* USE_HAL_USART_REGISTER_CALLBACKS */

} USART_ManagerStruct;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

extern USART_ErrorStatus_t USART_Init(USART_ManagerStruct* Ptr_usartManager);

USART_ErrorStatus_t USART_startTransmit_IT(USART_ManagerStruct* Ptr_uartxManger,  const uint8_t *pData, uint16_t Size);

USART_ErrorStatus_t USART_startRecieve_IT(USART_ManagerStruct *Ptr_uartxManger, uint8_t *pData, uint16_t Size);

void USART_sendByte_polling(USART_ManagerStruct *usartxManger, const uint8_t data);

uint8_t USART_recieveByte_polling(USART_ManagerStruct *usartxManger);

void USART_IRQHandler(USART_ManagerStruct *usartxManger);



#endif // E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_MCAL_USART_H_