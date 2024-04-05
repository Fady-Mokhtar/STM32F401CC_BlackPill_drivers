/******************************************************************************
 *                       			Includes			                      *
 ******************************************************************************/
#include "USART.h"
#include "USART_Reg.h"//for dev phase only
// #include "USART_cfg.h"
#include "STD_TYPES.h"


// this must be extern in the main or in the .h file
USART_ManagerStruct USART1Manager={
	.Instance	= (USART_RegStruct*)USART1_BASE,
	.init		= {
		.baudRate = 9600,
		.channel = USART_Channel_1,
		.state = USART_STATE_READY,
		.mode = USART_FullDuplex,
		.wordLength = USART_WORDLENGTH_8B,
		.parityControl = USART_ParityDisable,
		.stopBitNum = USART_1_BIT_Stop,
		.overSampling = USART_OVER16,
		.dmaState = USART_DISABLE_DMA,
		.en_dis_state = USART_ENABLE

	},
	.pTxBuffPtr	= NULL_t,
	.TxXferSize	= 0,
	.TxXferCount= 0,
	.pRxBuffPtr	= NULL_t,
	.RxXferSize	= 0,
	.RxXferCount= 0,
	.txState		= USART_STATE_RESET,
	.rxState		= USART_STATE_RESET,
	.ErrorCode	= USART_ERROR_NONE
};

// USART1Manager.ErrorCode = 0;