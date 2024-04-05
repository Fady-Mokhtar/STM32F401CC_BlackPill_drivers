/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "RCC.h"
#include "Common_Macros.h"

/* Driver Libraries   */
#include "USART.h"
#include "USART_Reg.h"
#include "NVIC.h"


/*   setting
modes:
Synchronous
Asynchronous
..
*
basic parameters:
	Baud rate
	word length
	parity
	stop bits
*
*/

/*** Information about USART
 * USART sends the least significient bit first
 */


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define USART1_IRQn		37
#define USART2_IRQn		38

typedef enum
{
	RESET = 0U,
	SET = !RESET
} FlagStatus;


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

static USART_ErrorStatus_t USART_Transmit_IT(USART_ManagerStruct *Ptr_uartxManger);

static USART_ErrorStatus_t USART_endTransmit_IT(USART_ManagerStruct *usartxManger);

static USART_ErrorStatus_t USART_Receive_IT(USART_ManagerStruct *Ptr_uartxManger);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

USART_ErrorStatus_t USART_Init(USART_ManagerStruct* Ptr_usartManager)
{
    USART_ErrorStatus_t Loc_USARTerrorState = USART_OK;

    switch (Ptr_usartManager->init.channel)
    {
    case USART_Channel_1:

        /*  RCC Enable CLK  */
        RCC_enableAPB2Peripheral(RCC_APB2PERIPHERAL_USART1);
        /*  clear TE, RE, UE, PCE, PS  */
        USART1->USART_CR1 &= (~ ((USART_CR1_TE_Msk) | (USART_CR1_RE_Msk) | (USART_CR1_M_Msk) | (USART_CR1_PCE_Msk) | (USART_CR1_PS_Msk) | (USART_CR1_UE_Msk)));
        USART1->USART_CR2 &= (~ (USART_CR2_STOP_Msk));
		/* BaudRate Ctrl*/
		USART1->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, Ptr_usartManager->init.baudRate);
        /*  set UE -->>  Enable   */
        switch (Ptr_usartManager->init.en_dis_state)
        {
            case USART_DISABLE:   USART1->USART_CR1 &= (~(USART_CR1_UE_Msk));    break;
            case USART_ENABLE:    USART1->USART_CR1 |= (USART_CR1_UE_Msk);       break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;             break;        
        }
        /*  set mode: Transmit, Receive or Full duplex   */
        switch (Ptr_usartManager->init.mode)
        {
            case USART_Transmit:     USART1->USART_CR1 |= (USART_CR1_TE_Msk);                          break;
            case USART_Receive:      USART1->USART_CR1 |= (USART_CR1_RE_Msk);                          break;
            case USART_FullDuplex:   USART1->USART_CR1 |= ((USART_CR1_TE_Msk) | (USART_CR1_RE_Msk));    break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                   break;        
        }
        
        switch (Ptr_usartManager->init.overSampling)
        {
            case USART_OVER16:   USART1->USART_CR1 &= (~(USART_CR1_OVER16_Msk));    break;
            case USART_OVER8:    USART1->USART_CR1 |= (USART_CR1_OVER8_Msk);        break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                break;        
        }

         /*  BaudRate Ctrl   */
        // switch (Ptr_usartManager->init.baudRate)    /*  baudrate depend on oversampling*/
        // {
        //     /*  this code handle oversampling16 only we have to make changes when using oversampling8*/
        //     /*  and we have to change the clk manually everytime it changes and also the baudrate we have to write the exact number according to ConfigPtr point to */
        //     case USART_BR_9600:      USART1->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)9600);      break;
        //     case USART_BR_115200:    USART1->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)115200);    break;
        //     default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                                      break;        
        // }

        switch (Ptr_usartManager->init.wordLength)
        {
            case USART_WORDLENGTH_8B:   USART1->USART_CR1 &= (~(USART_CR1_M_Msk));    break;
            case USART_WORDLENGTH_9B:   USART1->USART_CR1 |= (USART_CR1_M_Msk);       break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                      break;        
        }

        switch (Ptr_usartManager->init.parityControl)
        {
            case USART_ParityDisable:        USART1->USART_CR1 &= (~(USART_CR1_PCE_Msk));                     break;
            case USART_ParityEnable_Even:    
                USART1->USART_CR1 |= (USART_CR1_PCE_Msk);  
                USART1->USART_CR1 &= (~(USART_CR1_PS_Msk));     
                break;
            case USART_ParityEnable_Odd:     USART1->USART_CR1 |= ((USART_CR1_PCE_Msk) | (USART_CR1_PS_Msk));  break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                          break;        
        }

        switch (Ptr_usartManager->init.stopBitNum)
        {
            case USART_1_BIT_Stop:   USART1->USART_CR2 &= (~(USART_CR2_STOP_Msk));    break;      /* 0x00 */
            case USART_2_BIT_Stop:   USART1->USART_CR2 |= (USART_CR2_STOP_1);         break;      /* 0x02 */
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                  break;        
        }
        
        switch (Ptr_usartManager->init.dmaState)
        {

            case USART_DISABLE_DMA:    USART1->USART_CR3 &= (~((USART_CR3_DMAT_Msk) | (USART_CR3_DMAR_Msk)));    break;
            case USART_ENABLE_DMAT:    
                USART1->USART_CR3 &= (~(USART_CR3_DMAR_Msk));       
                USART1->USART_CR3 |= (USART_CR3_DMAT_Msk);   
                break;
            case USART_ENABLE_DMAR:    
                USART1->USART_CR3 &= (~(USART_CR3_DMAT_Msk));       
                USART1->USART_CR3 |= (USART_CR3_DMAR_Msk);   
                break;
            case USART_ENABLE_DMA_FD:    USART1->USART_CR3 |= ((USART_CR3_DMAT_Msk) | (USART_CR3_DMAR_Msk));     break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                            break;        
        }


		Ptr_usartManager->rxState = USART_STATE_READY;
		Ptr_usartManager->txState = USART_STATE_READY;

        /***********************************************************/
        break;

    case USART_Channel_2:
        RCC_enableAPB1Peripheral(RCC_APB1PERIPHERAL_USART2);
        /*  clear TE, RE, UE, PCE, PS  */
        USART2->USART_CR1 &= (~ ((USART_CR1_TE_Msk) | (USART_CR1_RE_Msk) | (USART_CR1_M_Msk) | (USART_CR1_PCE_Msk) | (USART_CR1_PS_Msk) | (USART_CR1_UE_Msk)));
        USART2->USART_CR2 &= (~ (USART_CR2_STOP_Msk));
		/* BaudRate Ctrl*/
		USART2->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, Ptr_usartManager->init.baudRate);
        /*  set UE -->>  Enable   */
        switch (Ptr_usartManager->init.en_dis_state)
        {
            case USART_DISABLE:   USART2->USART_CR1 &= (~(USART_CR1_UE_Msk));    break;
            case USART_ENABLE:    USART2->USART_CR1 |= (USART_CR1_UE_Msk);       break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;             break;        
        }
        /*  set mode: Transmit, Receive or Full duplex   */
        switch (Ptr_usartManager->init.mode)
        {
            case USART_Transmit:     USART2->USART_CR1 |= (USART_CR1_TE_Msk);                          break;
            case USART_Receive:      USART2->USART_CR1 |= (USART_CR1_RE_Msk);                          break;
            case USART_FullDuplex:   USART2->USART_CR1 |= ((USART_CR1_TE_Msk) | (USART_CR1_RE_Msk));    break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                   break;        
        }
        
        switch (Ptr_usartManager->init.overSampling)
        {
            case USART_OVER16:   USART2->USART_CR1 &= (~(USART_CR1_OVER16_Msk));    break;
            case USART_OVER8:    USART2->USART_CR1 |= (USART_CR1_OVER8_Msk);        break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                break;        
        }

         /*  BaudRate Ctrl   */
        // switch (Ptr_usartManager->init.baudRate)    /*  baudrate depend on oversampling*/
        // {
        //     /*  this code handle oversampling16 only we have to make changes when using oversampling8*/
        //     /*  and we have to change the clk manually everytime it changes */
        //     case USART_BR_9600:     USART2->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)9600);      break;
        //     case USART_BR_115200:    USART2->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)19200);     break;
        //     default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                                   break;        
        // }

        switch (Ptr_usartManager->init.wordLength)
        {
            case USART_WORDLENGTH_8B:   USART2->USART_CR1 &= (~(USART_CR1_M_Msk));    break;
            case USART_WORDLENGTH_9B:   USART2->USART_CR1 |= (USART_CR1_M_Msk);       break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                      break;        
        }

        switch (Ptr_usartManager->init.parityControl)
        {
            case USART_ParityDisable:        USART2->USART_CR1 &= (~(USART_CR1_PCE_Msk));                     break;
            case USART_ParityEnable_Even:    
                USART2->USART_CR1 |= (USART_CR1_PCE_Msk);  
                USART2->USART_CR1 &= (~(USART_CR1_PS_Msk));     
                break;
            case USART_ParityEnable_Odd:     USART2->USART_CR1 |= ((USART_CR1_PCE_Msk) | (USART_CR1_PS_Msk));  break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                          break;        
        }

        switch (Ptr_usartManager->init.stopBitNum)
        {
            case USART_1_BIT_Stop:   USART2->USART_CR2 &= (~(USART_CR2_STOP_Msk));    break;      /* 0x00 */
            case USART_2_BIT_Stop:   USART2->USART_CR2 |= (USART_CR2_STOP_1);         break;      /* 0x02 */
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                  break;        
        }
        
        switch (Ptr_usartManager->init.dmaState)
        {

            case USART_DISABLE_DMA:    USART2->USART_CR3 &= (~((USART_CR3_DMAT_Msk) | (USART_CR3_DMAR_Msk)));    break;
            case USART_ENABLE_DMAT:    
                USART2->USART_CR3 &= (~(USART_CR3_DMAR_Msk));       
                USART2->USART_CR3 |= (USART_CR3_DMAT_Msk);   
                break;
            case USART_ENABLE_DMAR:    
                USART2->USART_CR3 &= (~(USART_CR3_DMAT_Msk));       
                USART2->USART_CR3 |= (USART_CR3_DMAR_Msk);   
                break;
            case USART_ENABLE_DMA_FD:    USART2->USART_CR3 |= ((USART_CR3_DMAT_Msk) | (USART_CR3_DMAR_Msk));     break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                            break;        
        }

		Ptr_usartManager->rxState = USART_STATE_READY;
		Ptr_usartManager->txState = USART_STATE_READY;

        /***********************************************************/
        break;
        
    case USART_Channel_6:
        RCC_enableAPB2Peripheral(RCC_APB2PERIPHERAL_USART6);
        /*  clear TE, RE, UE, PCE, PS  */
        USART6->USART_CR1 &= (~ ((USART_CR1_TE_Msk) | (USART_CR1_RE_Msk) | (USART_CR1_M_Msk) | (USART_CR1_PCE_Msk) | (USART_CR1_PS_Msk) | (USART_CR1_UE_Msk)));
        USART6->USART_CR2 &= (~ (USART_CR2_STOP_Msk));
		/* BaudRate Ctrl*/
		USART6->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, Ptr_usartManager->init.baudRate);
        /*  set UE -->>  Enable   */
        switch (Ptr_usartManager->init.en_dis_state)
        {
            case USART_DISABLE:   USART6->USART_CR1 &= (~(USART_CR1_UE_Msk));    break;
            case USART_ENABLE:    USART6->USART_CR1 |= (USART_CR1_UE_Msk);       break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;             break;        
        }
        /*  set mode: Transmit, Receive or Full duplex   */
        switch (Ptr_usartManager->init.mode)
        {
            case USART_Transmit:     USART6->USART_CR1 |= (USART_CR1_TE_Msk);                          break;
            case USART_Receive:      USART6->USART_CR1 |= (USART_CR1_RE_Msk);                          break;
            case USART_FullDuplex:   USART6->USART_CR1 |= ((USART_CR1_TE_Msk) | (USART_CR1_RE_Msk));    break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                   break;        
        }
        
        switch (Ptr_usartManager->init.overSampling)
        {
            case USART_OVER16:   USART6->USART_CR1 &= (~(USART_CR1_OVER16_Msk));    break;
            case USART_OVER8:    USART6->USART_CR1 |= (USART_CR1_OVER8_Msk);        break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                break;        
        }

         /*  BaudRate Ctrl   */
        // switch (Ptr_usartManager->init.baudRate)    /*  baudrate depend on oversampling*/
        // {
        //     /*  this code handle oversampling16 only we have to make changes when using oversampling8*/
        //     /*  and we have to change the clk manually everytime it changes */
        //     case USART_BR_9600:     USART6->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)9600);      break;
        //     case USART_BR_115200:    USART6->USART_BRR = USART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)19200);     break;
        //     default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                                   break;        
        // }

        switch (Ptr_usartManager->init.wordLength)
        {
            case USART_WORDLENGTH_8B:   USART6->USART_CR1 &= (~(USART_CR1_M_Msk));    break;
            case USART_WORDLENGTH_9B:   USART6->USART_CR1 |= (USART_CR1_M_Msk);       break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                      break;        
        }

        switch (Ptr_usartManager->init.parityControl)
        {
            case USART_ParityDisable:        USART6->USART_CR1 &= (~(USART_CR1_PCE_Msk));                     break;
            case USART_ParityEnable_Even:    
                USART6->USART_CR1 |= (USART_CR1_PCE_Msk);  
                USART6->USART_CR1 &= (~(USART_CR1_PS_Msk));     
                break;
            case USART_ParityEnable_Odd:     USART6->USART_CR1 |= ((USART_CR1_PCE_Msk) | (USART_CR1_PS_Msk));  break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                          break;        
        }

        switch (Ptr_usartManager->init.stopBitNum)
        {
            case USART_1_BIT_Stop:   USART6->USART_CR2 &= (~(USART_CR2_STOP_Msk));    break;      /* 0x00 */
            case USART_2_BIT_Stop:   USART6->USART_CR2 |= (USART_CR2_STOP_1);         break;      /* 0x02 */
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                  break;        
        }
        
        switch (Ptr_usartManager->init.dmaState)
        {

            case USART_DISABLE_DMA:    USART6->USART_CR3 &= (~((USART_CR3_DMAT_Msk) | (USART_CR3_DMAR_Msk)));    break;
            case USART_ENABLE_DMAT:    
                USART6->USART_CR3 &= (~(USART_CR3_DMAR_Msk));       
                USART6->USART_CR3 |= (USART_CR3_DMAT_Msk);   
                break;
            case USART_ENABLE_DMAR:    
                USART6->USART_CR3 &= (~(USART_CR3_DMAT_Msk));       
                USART6->USART_CR3 |= (USART_CR3_DMAR_Msk);   
                break;
            case USART_ENABLE_DMA_FD:    USART6->USART_CR3 |= ((USART_CR3_DMAT_Msk) | (USART_CR3_DMAR_Msk));     break;
            default:    Loc_USARTerrorState =  USART_WRONG_INPUTS;                                            break;        
        }

		Ptr_usartManager->rxState = USART_STATE_READY;
		Ptr_usartManager->txState = USART_STATE_READY;
		
        /***********************************************************/
        break;
    
    default:
        Loc_USARTerrorState = USART_WRONG_INPUTS;
        break;
    }

    return Loc_USARTerrorState;
}

/*    another init
void USART_Init(uint8_t Copy_uart_channel, USART_InitStruct *configPtr)
{
	USART_RegStruct *USART_basePtr = NULL_t_t;
	switch (Copy_uart_channel)
	{
	case USART1:
		USART_basePtr = (USART_RegStruct *)USART1_BASE;
		RCC_Enable(RCC_USART1);
		break;
	case USART2:
		USART_basePtr = (USART_RegStruct *)USART2_BASE;
		RCC_Enable(RCC_USART2);
		break;
	case USART6:
		USART_basePtr = (USART_RegStruct *)USART6_BASE;
		RCC_Enable(RCC_USART6);
		break;

	default:
		break;
	}


	// Configure stop bits
	// USART_basePtr->CR2 = (USART_basePtr->CR2 & ~(USART_CR2_STOP_ClrMsk)) | StopBits;
	REG_CLEARANDSET_BYMASKS(USART_basePtr->CR2, USART_CR2_STOP_clrMsk, configPtr->StopBits);

	// configure Wordlength, parity, TxorRX mode
	REG_CLEARANDSET_BYMASKS(USART_basePtr->USART_CR1,
							((uint32)(USART_CR1_M_clrMsk | USART_CR1_PCE_clrMsk | USART_CR1_PS_clrMsk | USART_CR1_TE_clrMsk | USART_CR1_RE_clrMsk)),
							((uint32)configPtr->wordLength | configPtr->Parity | configPtr->Mode));

	// configure baudrate
	USART_basePtr->BRR = USART_BRR_SAMPLING16(16000000, configPtr->BaudRate);

	// enable the uart
	(USART_basePtr->USART_CR1 |= USART_CR1_UE);

	return;
}
*/


USART_ErrorStatus_t USART_startTransmit_IT(USART_ManagerStruct* Ptr_uartxManger,  const uint8_t *pData, uint16_t Size)
{
	USART_ErrorStatus_t Loc_USARTerrorState = USART_OK;

	if (Ptr_uartxManger->txState != USART_STATE_READY)
	{
		return USART_BUSY;
	}

	if ((pData == NULL_t) || (Size == 0U))
    {
      	return USART_ERROR;
    }

	Ptr_uartxManger->pTxBuffPtr  = pData;
    Ptr_uartxManger->TxXferSize  = Size;
    Ptr_uartxManger->TxXferCount = Size;

	// set busy state to indiacte that the USart is in use
	Ptr_uartxManger->txState = USART_STATE_BUSY_TX;

	// enable interrupt TXE when the tx is empty and ready to write the new data
	Ptr_uartxManger->Instance->USART_CR1 |= USART_CR1_TXEIE;


	Enable_Interrupts();
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_EnableIRQ(USART6_IRQn);

	return Loc_USARTerrorState;
}

static USART_ErrorStatus_t USART_Transmit_IT(USART_ManagerStruct *Ptr_uartxManger)
{
	//USART_ErrorStatus_t Loc_USARTerrorState = USART_OK;

	const uint16_t *tmp;

	/* Check that a Tx process is ongoing */
	if (Ptr_uartxManger->txState == USART_STATE_BUSY_TX)
	{
		if ((Ptr_uartxManger->init.wordLength == USART_WORDLENGTH_9B) && (Ptr_uartxManger->init.parityControl == USART_ParityDisable))
		{
			tmp = (const uint16_t *) Ptr_uartxManger->pTxBuffPtr;
			Ptr_uartxManger->Instance->USART_DR = (uint16_t)(*tmp & (uint16_t)0x01FF);
			Ptr_uartxManger->pTxBuffPtr += 2U;
		}
		else
		{
			Ptr_uartxManger->Instance->USART_DR = (uint8_t)(*Ptr_uartxManger->pTxBuffPtr++ & (uint8_t)0x00FF);
		}

		if (--(Ptr_uartxManger->TxXferCount) == 0U)
		{
		/* Disable the USART Transmit Data Register Empty Interrupt */
		Ptr_uartxManger->Instance->USART_CR1 &= ~USART_CR1_TXEIE_clrMsk;

		/* Enable the USART Transmit Complete Interrupt */
		Ptr_uartxManger->Instance->USART_CR1 |= USART_CR1_TCIE;
		}
		return USART_OK;
	}
	else
	{
		return USART_BUSY;
	}
}

static USART_ErrorStatus_t USART_endTransmit_IT(USART_ManagerStruct *usartxManger)
{
	/* Disable the USART Transmit Complete Interrupt */
	usartxManger->Instance->USART_CR1 &= ~USART_CR1_TCIE_clrMsk;

	/* Tx process is ended, restore huart->gState to Ready */
	usartxManger->txState = USART_STATE_READY;

	return USART_OK;
}

USART_ErrorStatus_t USART_startRecieve_IT(USART_ManagerStruct *Ptr_uartxManger, uint8_t *pData, uint16_t Size)
{
	/* Check that a Rx process is not already ongoing */
	if (Ptr_uartxManger->rxState == USART_STATE_READY)
	{
		if ((pData == NULL_t) || (Size == 0U))
		{
			return USART_ERROR;
		}

		/* Set Reception type to Standard reception */
		// Ptr_uartxManger->ReceptionType = HAL_USART_RECEPTION_STANDARD;

		Ptr_uartxManger->pRxBuffPtr = pData;
		Ptr_uartxManger->RxXferSize = Size;
		Ptr_uartxManger->RxXferCount = Size;

		// to reset the error state if you handle it in the manager
		//   Ptr_uartxManger->ErrorCode = USART_ERROR_NONE;
		Ptr_uartxManger->rxState = USART_STATE_BUSY_RX;

		// 111111111111111 enable ERROR interrupts
		if (Ptr_uartxManger->init.parityControl != USART_ParityDisable)
		{
			/* Enable the USART Parity Error Interrupt */
			Ptr_uartxManger->Instance->USART_CR1 |= (USART_CR1_PEIE_Msk);
		}

		/* Enable the USART Error Interrupt: (Frame error, noise error, overrun error) */
		Ptr_uartxManger->Instance->USART_CR1 |= (USART_CR3_EIE_Msk);
		// 111111111111111

		/* Enable the USART Data Register not empty Interrupt */
		Ptr_uartxManger->Instance->USART_CR1 |= (USART_CR1_RXNEIE_Msk);

		return USART_OK;
	}
	else
	{
		return USART_BUSY;
	}
}

static USART_ErrorStatus_t USART_Receive_IT(USART_ManagerStruct *Ptr_uartxManger)
{
	uint8_t *pdata8bits;
	uint16_t *pdata16bits;

	/* Check that a Rx process is ongoing */
	if (Ptr_uartxManger->rxState == USART_STATE_BUSY_RX)
	{
		if ((Ptr_uartxManger->init.wordLength == USART_WORDLENGTH_9B) && (Ptr_uartxManger->init.parityControl == USART_ParityDisable))
		{
			pdata8bits = NULL_t;
			pdata16bits = (uint16_t *)Ptr_uartxManger->pRxBuffPtr;
			*pdata16bits = (uint16_t)(Ptr_uartxManger->Instance->USART_DR & (uint16_t)0x01FF);
			Ptr_uartxManger->pRxBuffPtr += 2U;
		}
		else
		{
			pdata8bits = (uint8_t *)Ptr_uartxManger->pRxBuffPtr;
			pdata16bits = NULL_t;

			if ((Ptr_uartxManger->init.wordLength == USART_WORDLENGTH_9B) || ((Ptr_uartxManger->init.wordLength == USART_WORDLENGTH_8B) && (Ptr_uartxManger->init.parityControl == USART_ParityDisable)))
			{
				*pdata8bits = (uint8_t)(Ptr_uartxManger->Instance->USART_DR & (uint8_t)0x00FF);
			}
			else
			{
				*pdata8bits = (uint8_t)(Ptr_uartxManger->Instance->USART_DR & (uint8_t)0x007F);
			}
			Ptr_uartxManger->pRxBuffPtr += 1U;
		}

		if (--(Ptr_uartxManger->RxXferCount) == 0U)
		{
			/* Disable the USART Data Register not empty Interrupt */
			Ptr_uartxManger->Instance->USART_CR1 &= ~(USART_CR1_RXNEIE_clrMsk);

		//11111111111111111 Disable Error Interrupts
			/* Disable the USART Parity Error Interrupt */
			Ptr_uartxManger->Instance->USART_CR1 &= ~(USART_CR1_PEIE_clrMsk);

			/* Disable the USART Error Interrupt: (Frame error, noise error, overrun error) */
			Ptr_uartxManger->Instance->USART_CR1 &= ~(USART_CR3_EIE_clrMsk);
		//11111111111111111

			/* Rx process is completed, restore Ptr_uartxManger->RxState to Ready */
			Ptr_uartxManger->rxState = USART_STATE_READY;

			//   /* Initialize type of RxEvent to Transfer Complete */
			//   Ptr_uartxManger->RxEventType = HAL_USART_RXEVENT_TC;

			/* Check current reception Mode :
				If Reception till IDLE event has been selected : */
			// if (Ptr_uartxManger->ReceptionType == HAL_USART_RECEPTION_TOIDLE)
			// {
			// 	/* Set reception type to Standard */
			// 	Ptr_uartxManger->ReceptionType = HAL_USART_RECEPTION_STANDARD;

			// 	/* Disable IDLE interrupt */
			// 	ATOMIC_CLEAR_BIT(Ptr_uartxManger->Instance->USART_CR1, USART_CR1_IDLEIE);

			// 	/* Check if IDLE flag is set */
			// 	if (__HAL_USART_GET_FLAG(Ptr_uartxManger, USART_FLAG_IDLE))
			// 	{
			// 		/* Clear IDLE flag in ISR */
			// 		__HAL_USART_CLEAR_IDLEFLAG(Ptr_uartxManger);
			// 	}
			// }

			// else
			// {

			// }

			return USART_OK;
		}
		return USART_OK;
	}
	else
	{
		return USART_BUSY;
	}
}



/*****************************************************************************************/
/***********************************POLLING FUNCTIONS*************************************/
/*****************************************************************************************/
void USART_sendByte_polling(USART_ManagerStruct *usartxManger, const uint8_t data)
{
	/* 1. wait til the TDR is empty and ready to take-in data (wait for the TXE flag).
	 * 2. then write the data in the Data Register.
	 */

	// stop while the TX line is full  (wait while the TXE flag is clear)
	while (BIT_IS_CLEAR(usartxManger->Instance->USART_SR, USART_SR_TXE_Pos))
	{
	}

	// writing the data in the data Register DR
	usartxManger->Instance->USART_DR = data;

	// not nessecary    only nessecaery after the last byte sent by the uart to  indiacte that all is complete and its okay to disable the usart after it
	// while (!((USART_basePtr->SR)&(USART_SR_TC_Pos)))
}

uint8_t USART_recieveByte_polling(USART_ManagerStruct *usartxManger)
{
	/* 1. wait for the module to detect a byte on the line. that will raise a flag (RXNE) RX Not Empty. wait for the flag.
	 * 2. then read and return the recieved data.
	 */

	// stop while the RX line is empty  (wait while the RXNE flag is clear)
	while (BIT_IS_CLEAR(usartxManger->Instance->USART_SR, USART_SR_RXNE_Pos))
	{
	}

	// read and return the recieved data
	return usartxManger->Instance->USART_DR;
}



/*****************************************************************************************/

void USART_IRQHandler(USART_ManagerStruct *usartxManger)
{
	uint32_t isrflags   = (usartxManger->Instance->USART_SR);
	uint32_t cr1its     = (usartxManger->Instance->USART_CR1);
	uint32_t cr3its     = (usartxManger->Instance->USART_CR3);
	uint32_t errorflags = 0x00U;
	uint32_t dmarequest = 0x00U;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
	if (errorflags == RESET)
	{
		/* USART in mode Receiver -------------------------------------------------*/
		if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
		{
			USART_Receive_IT(usartxManger);
			return;
		}
	}

	/* If some errors occur Abort (cancel all recive it. and fn. and return)*/
	if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
	{
		/* USART parity error interrupt occurred ----------------------------------*/
		if (((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
		{
			usartxManger->ErrorCode |= USART_ERROR_PE;
		}

		/* USART frame error interrupt occurred -----------------------------------*/
		if (((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			usartxManger->ErrorCode |= USART_ERROR_FE;
		}

		/* USART noise error interrupt occurred -----------------------------------*/
		if (((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
		{
			usartxManger->ErrorCode |= USART_ERROR_NE;
		}

		/* USART Over-Run interrupt occurred --------------------------------------*/
		if (((isrflags & USART_SR_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET) || ((cr3its & USART_CR3_EIE) != RESET)))
		{
			usartxManger->ErrorCode |= USART_ERROR_ORE;
		}

		/* Call USART Error Call back function if need be --------------------------*/
		// if there is an error   recive if there is data in DR and then ABORT
		if (usartxManger->ErrorCode != USART_ERROR_NONE)
		{
			/* USART in mode Receiver -----------------------------------------------*/
			if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
			{
				USART_Receive_IT(usartxManger);
			}

			// /* If Overrun error occurs, or if any error occurs in DMA mode reception,
			//    consider error as blocking */
			// dmarequest = HAL_IS_BIT_SET(usartxManger->Instance->CR3, USART_CR3_DMAR);
			// if (((usartxManger->ErrorCode & USART_ERROR_ORE) != RESET) || dmarequest)
			// {
			// 	/* Blocking error : transfer is aborted
			// 	   Set the USART state ready to be able to start again the process,
			// 	   Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
			// 	USART_EndRxTransfer(usartxManger);

			// 	/* Disable the USART DMA Rx request if enabled */
			// 	if (HAL_IS_BIT_SET(usartxManger->Instance->CR3, USART_CR3_DMAR))
			// 	{
			// 		ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_DMAR);

			// 		/* Abort the USART DMA Rx stream */
			// 		if (usartxManger->hdmarx != NULL)
			// 		{
			// 			/* Set the USART DMA Abort callback :
			// 			   will lead to call HAL_USART_ErrorCallback() at end of DMA abort procedure */
			// 			usartxManger->hdmarx->XferAbortCallback = USART_DMAAbortOnError;
			// 			if (HAL_DMA_Abort_IT(usartxManger->hdmarx) != HAL_OK)
			// 			{
			// 				/* Call Directly XferAbortCallback function in case of error */
			// 				usartxManger->hdmarx->XferAbortCallback(usartxManger->hdmarx);
			// 			}
			// 		}
			// 		else
			// 		{
			// 			/* Call user error callback */
			// 		}
			// 	}
			// 	else
			// 	{
			// 		/* Call user error callback */
			// 	}
			// }
			// else
			{
				/* Non Blocking error : transfer could go on.
				   Error is notified to user through user error callback */

				usartxManger->ErrorCode = USART_ERROR_NONE;
			}
		}
		return;
	} /* End if some error occurs */

	// 	/* Check current reception Mode :
	// 	   If Reception till IDLE event has been selected : */
	// 	if ((usartxManger->ReceptionType == HAL_USART_RECEPTION_TOIDLE) && ((isrflags & USART_SR_IDLE) != 0U) && ((cr1its & USART_SR_IDLE) != 0U))
	// 	{
	// 		__HAL_USART_CLEAR_IDLEFLAG(usartxManger);

	// 		/* Check if DMA mode is enabled in USART */
	// 		if (HAL_IS_BIT_SET(usartxManger->Instance->CR3, USART_CR3_DMAR))
	// 		{
	// 			/* DMA mode enabled */
	// 			/* Check received length : If all expected data are received, do nothing,
	// 			   (DMA cplt callback will be called).
	// 			   Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
	// 			uint16_t nb_remaining_rx_data = (uint16_t)__HAL_DMA_GET_COUNTER(usartxManger->hdmarx);
	// 			if ((nb_remaining_rx_data > 0U) && (nb_remaining_rx_data < usartxManger->RxXferSize))
	// 			{
	// 				/* Reception is not complete */
	// 				usartxManger->RxXferCount = nb_remaining_rx_data;

	// 				/* In Normal mode, end DMA xfer and HAL USART Rx process*/
	// 				if (usartxManger->hdmarx->Init.Mode != DMA_CIRCULAR)
	// 				{
	// 					/* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
	// 					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_PEIE);
	// 					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_EIE);

	// 					/* Disable the DMA transfer for the receiver request by resetting the DMAR bit
	// 					   in the USART CR3 register */
	// 					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_DMAR);

	// 					/* At end of Rx process, restore usartxManger->RxState to Ready */
	// 					usartxManger->rxState = USART_STATE_READY;
	// 					usartxManger->ReceptionType = HAL_USART_RECEPTION_STANDARD;

	// 					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);

	// 					/* Last bytes received, so no need as the abort is immediate */
	// 					(void)HAL_DMA_Abort(usartxManger->hdmarx);
	// 				}

	// 				/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 				In this case, Rx Event type is Idle Event */
	// 				usartxManger->RxEventType = HAL_USART_RXEVENT_IDLE;

	// 			///////////////if you want to add a cbf on event you can add it here
	// 			}
	// 			return;
	// 		}
	// 		else
	// 		{
	// 			/* DMA mode not enabled */
	// 			/* Check received length : If all expected data are received, do nothing.
	// 			   Otherwise, if at least one data has already been received, IDLE event is to be notified to user */
	// 			uint16_t nb_rx_data = usartxManger->RxXferSize - usartxManger->RxXferCount;
	// 			if ((usartxManger->RxXferCount > 0U) && (nb_rx_data > 0U))
	// 			{
	// //////////////////////////
	// 				/* Disable the USART Parity Error Interrupt and RXNE interrupts */
	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));

	// 				/* Disable the USART Error Interrupt: (Frame error, noise error, overrun error) */
	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_EIE);
	// //////////////////////////

	// 				/* Rx process is completed, restore usartxManger->RxState to Ready */
	// 				usartxManger->rxState = USART_STATE_READY;
	// 				usartxManger->ReceptionType = HAL_USART_RECEPTION_STANDARD;

	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);

	// 				/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 				   In this case, Rx Event type is Idle Event */
	// 				usartxManger->RxEventType = HAL_USART_RXEVENT_IDLE;
	// 			}
	// 			return;
	// 		}
	// }

	/* USART in mode Transmitter ------------------------------------------------*/
	if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
	{
		USART_Transmit_IT(usartxManger);
		return;
	}

	/* USART in mode Transmitter end --------------------------------------------*/
	if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
	{
		USART_endTransmit_IT(usartxManger);
		return;
	}
}

/*****************************************************************************************/

