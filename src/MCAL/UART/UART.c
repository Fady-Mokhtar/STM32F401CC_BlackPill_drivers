/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "RCC.h"
#include "Common_Macros.h"

/* Driver Libraries   */
#include "UART.h"
#include "UART_Reg.h"


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

/*** Information about UART
 * UART sends the least significient bit first
 */


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

static UART_ErrorStatus_t UART_Transmit_IT(UART_ManagerStruct *Ptr_uartxManger);

static UART_ErrorStatus_t UART_endTransmit_IT(UART_ManagerStruct *usartxManger);

static UART_ErrorStatus_t UART_Receive_IT(UART_ManagerStruct *Ptr_uartxManger);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

UART_ErrorStatus_t UART_Init(const UART_InitStruct* ConfigPtr)
{
    UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

    switch (ConfigPtr->channel)
    {
    case UART_Channel_1:
        /*  RCC Enable CLK  */
        RCC_enableAPB2Peripheral(RCC_APB2PERIPHERAL_USART1);
        /*  clear TE, RE, UE, PCE, PS  */
        UART1->UART_CR1 &= (~ ((UART_CR1_TE_Msk) | (UART_CR1_RE_Msk) | (UART_CR1_M_Msk) | (UART_CR1_PCE_Msk) | (UART_CR1_PS_Msk) | (UART_CR1_UE_Msk)));
        UART1->UART_CR2 &= (~ (UART_CR2_STOP_Msk));
        /*  set UE -->>  Enable   */
        switch (ConfigPtr->state)
        {
            case UART_DISABLE:   UART1->UART_CR1 &= (~(UART_CR1_UE_Msk));    break;
            case UART_ENABLE:    UART1->UART_CR1 |= (UART_CR1_UE_Msk);       break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;             break;        
        }
        /*  set mode: Transmit, Receive or Full duplex   */
        switch (ConfigPtr->mode)
        {
            case UART_Transmit:     UART1->UART_CR1 |= (UART_CR1_TE_Msk);                          break;
            case UART_Receive:      UART1->UART_CR1 |= (UART_CR1_RE_Msk);                          break;
            case UART_FullDuplex:   UART1->UART_CR1 |= ((UART_CR1_TE_Msk) | (UART_CR1_RE_Msk));    break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                   break;        
        }
        
        switch (ConfigPtr->overSampling)
        {
            case UART_OVER16:   UART1->UART_CR1 &= (~(UART_CR1_OVER16_Msk));    break;
            case UART_OVER8:    UART1->UART_CR1 |= (UART_CR1_OVER8_Msk);        break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                break;        
        }

         /*  BaudRate Ctrl   */
        switch (ConfigPtr->baudRate)    /*  baudrate depend on oversampling*/
        {
            /*  this code handle oversampling16 only we have to make changes when using oversampling8*/
            /*  and we have to change the clk manually everytime it changes and also the baudrate we have to write the exact number according to ConfigPtr point to */
            case UART_BR_9600:     UART1->UART_BRR = UART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)9600);      break;
            case UART_BR_19200:    UART1->UART_BRR = UART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)19200);     break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                                   break;        
        }

        switch (ConfigPtr->wordLength)
        {
            case UART_WORDLENGTH_8B:   UART1->UART_CR1 &= (~(UART_CR1_M_Msk));    break;
            case UART_WORDLENGTH_9B:   UART1->UART_CR1 |= (UART_CR1_M_Msk);       break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                      break;        
        }

        switch (ConfigPtr->parityControl)
        {
            case UART_ParityDisable:        UART1->UART_CR1 &= (~(UART_CR1_PCE_Msk));                     break;
            case UART_ParityEnable_Even:    
                UART1->UART_CR1 |= (UART_CR1_PCE_Msk);  
                UART1->UART_CR1 &= (~(UART_CR1_PS_Msk));     
                break;
            case UART_ParityEnable_Odd:     UART1->UART_CR1 |= ((UART_CR1_PCE_Msk) | (UART_CR1_PS_Msk));  break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                          break;        
        }

        switch (ConfigPtr->stopBitNum)
        {
            case UART_1_BIT_Stop:   UART1->UART_CR2 &= (~(UART_CR2_STOP_Msk));    break;      /* 0x00 */
            case UART_2_BIT_Stop:   UART1->UART_CR2 |= (UART_CR2_STOP_1);         break;      /* 0x02 */
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                  break;        
        }
        
        switch (ConfigPtr->dmaState)
        {

            case UART_DISABLE_DMA:    UART1->UART_CR3 &= (~((UART_CR3_DMAT_Msk) | (UART_CR3_DMAR_Msk)));    break;
            case UART_ENABLE_DMAT:    
                UART1->UART_CR3 &= (~(UART_CR3_DMAR_Msk));       
                UART1->UART_CR3 |= (UART_CR3_DMAT_Msk);   
                break;
            case UART_ENABLE_DMAR:    
                UART1->UART_CR3 &= (~(UART_CR3_DMAT_Msk));       
                UART1->UART_CR3 |= (UART_CR3_DMAR_Msk);   
                break;
            case UART_ENABLE_DMA_FD:    UART1->UART_CR3 |= ((UART_CR3_DMAT_Msk) | (UART_CR3_DMAR_Msk));     break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                            break;        
        }

        /*  Interrupt control   */
        if (ConfigPtr->TransDateRegisterEmpty_Int == 1)
            UART1->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->TransDateRegisterEmpty_Int == 0)
            UART1->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        if (ConfigPtr->ReceiveDateRegisterEmpty_Int == 1)
            UART1->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->ReceiveDateRegisterEmpty_Int == 0)
            UART1->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        if (ConfigPtr->TransDateRegisterEmpty_Int == 1)
            UART1->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->TransDateRegisterEmpty_Int == 0)
            UART1->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        break;

    case UART_Channel_2:
        RCC_enableAPB1Peripheral(RCC_APB1PERIPHERAL_USART2);
        /*  clear TE, RE, UE, PCE, PS  */
        UART2->UART_CR1 &= (~ ((UART_CR1_TE_Msk) | (UART_CR1_RE_Msk) | (UART_CR1_M_Msk) | (UART_CR1_PCE_Msk) | (UART_CR1_PS_Msk) | (UART_CR1_UE_Msk)));
        UART2->UART_CR2 &= (~ (UART_CR2_STOP_Msk));
        /*  set UE -->>  Enable   */
        switch (ConfigPtr->state)
        {
            case UART_DISABLE:   UART2->UART_CR1 &= (~(UART_CR1_UE_Msk));    break;
            case UART_ENABLE:    UART2->UART_CR1 |= (UART_CR1_UE_Msk);       break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;             break;        
        }
        /*  set mode: Transmit, Receive or Full duplex   */
        switch (ConfigPtr->mode)
        {
            case UART_Transmit:     UART2->UART_CR1 |= (UART_CR1_TE_Msk);                          break;
            case UART_Receive:      UART2->UART_CR1 |= (UART_CR1_RE_Msk);                          break;
            case UART_FullDuplex:   UART2->UART_CR1 |= ((UART_CR1_TE_Msk) | (UART_CR1_RE_Msk));    break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                   break;        
        }
        
        switch (ConfigPtr->overSampling)
        {
            case UART_OVER16:   UART2->UART_CR1 &= (~(UART_CR1_OVER16_Msk));    break;
            case UART_OVER8:    UART2->UART_CR1 |= (UART_CR1_OVER8_Msk);        break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                break;        
        }

         /*  BaudRate Ctrl   */
        switch (ConfigPtr->baudRate)    /*  baudrate depend on oversampling*/
        {
            /*  this code handle oversampling16 only we have to make changes when using oversampling8*/
            /*  and we have to change the clk manually everytime it changes */
            case UART_BR_9600:     UART2->UART_BRR = UART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)9600);      break;
            case UART_BR_19200:    UART2->UART_BRR = UART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)19200);     break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                                   break;        
        }

        switch (ConfigPtr->wordLength)
        {
            case UART_WORDLENGTH_8B:   UART2->UART_CR1 &= (~(UART_CR1_M_Msk));    break;
            case UART_WORDLENGTH_9B:   UART2->UART_CR1 |= (UART_CR1_M_Msk);       break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                      break;        
        }

        switch (ConfigPtr->parityControl)
        {
            case UART_ParityDisable:        UART2->UART_CR1 &= (~(UART_CR1_PCE_Msk));                     break;
            case UART_ParityEnable_Even:    
                UART2->UART_CR1 |= (UART_CR1_PCE_Msk);  
                UART2->UART_CR1 &= (~(UART_CR1_PS_Msk));     
                break;
            case UART_ParityEnable_Odd:     UART2->UART_CR1 |= ((UART_CR1_PCE_Msk) | (UART_CR1_PS_Msk));  break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                          break;        
        }

        switch (ConfigPtr->stopBitNum)
        {
            case UART_1_BIT_Stop:   UART2->UART_CR2 &= (~(UART_CR2_STOP_Msk));    break;      /* 0x00 */
            case UART_2_BIT_Stop:   UART2->UART_CR2 |= (UART_CR2_STOP_1);         break;      /* 0x02 */
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                  break;        
        }
        
        switch (ConfigPtr->dmaState)
        {

            case UART_DISABLE_DMA:    UART2->UART_CR3 &= (~((UART_CR3_DMAT_Msk) | (UART_CR3_DMAR_Msk)));    break;
            case UART_ENABLE_DMAT:    
                UART2->UART_CR3 &= (~(UART_CR3_DMAR_Msk));       
                UART2->UART_CR3 |= (UART_CR3_DMAT_Msk);   
                break;
            case UART_ENABLE_DMAR:    
                UART2->UART_CR3 &= (~(UART_CR3_DMAT_Msk));       
                UART2->UART_CR3 |= (UART_CR3_DMAR_Msk);   
                break;
            case UART_ENABLE_DMA_FD:    UART2->UART_CR3 |= ((UART_CR3_DMAT_Msk) | (UART_CR3_DMAR_Msk));     break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                            break;        
        }

        /*  Interrupt control   */
        if (ConfigPtr->TransDateRegisterEmpty_Int == 1)
            UART2->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->TransDateRegisterEmpty_Int == 0)
            UART2->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        if (ConfigPtr->ReceiveDateRegisterEmpty_Int == 1)
            UART2->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->ReceiveDateRegisterEmpty_Int == 0)
            UART2->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        if (ConfigPtr->TransDateRegisterEmpty_Int == 1)
            UART2->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->TransDateRegisterEmpty_Int == 0)
            UART2->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        break;
        
    case UART_Channel_6:
        RCC_enableAPB2Peripheral(RCC_APB2PERIPHERAL_USART6);
        /*  clear TE, RE, UE, PCE, PS  */
        UART6->UART_CR1 &= (~ ((UART_CR1_TE_Msk) | (UART_CR1_RE_Msk) | (UART_CR1_M_Msk) | (UART_CR1_PCE_Msk) | (UART_CR1_PS_Msk) | (UART_CR1_UE_Msk)));
        UART6->UART_CR2 &= (~ (UART_CR2_STOP_Msk));
        /*  set UE -->>  Enable   */
        switch (ConfigPtr->state)
        {
            case UART_DISABLE:   UART6->UART_CR1 &= (~(UART_CR1_UE_Msk));    break;
            case UART_ENABLE:    UART6->UART_CR1 |= (UART_CR1_UE_Msk);       break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;             break;        
        }
        /*  set mode: Transmit, Receive or Full duplex   */
        switch (ConfigPtr->mode)
        {
            case UART_Transmit:     UART6->UART_CR1 |= (UART_CR1_TE_Msk);                          break;
            case UART_Receive:      UART6->UART_CR1 |= (UART_CR1_RE_Msk);                          break;
            case UART_FullDuplex:   UART6->UART_CR1 |= ((UART_CR1_TE_Msk) | (UART_CR1_RE_Msk));    break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                   break;        
        }
        
        switch (ConfigPtr->overSampling)
        {
            case UART_OVER16:   UART6->UART_CR1 &= (~(UART_CR1_OVER16_Msk));    break;
            case UART_OVER8:    UART6->UART_CR1 |= (UART_CR1_OVER8_Msk);        break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                break;        
        }

         /*  BaudRate Ctrl   */
        switch (ConfigPtr->baudRate)    /*  baudrate depend on oversampling*/
        {
            /*  this code handle oversampling16 only we have to make changes when using oversampling8*/
            /*  and we have to change the clk manually everytime it changes */
            case UART_BR_9600:     UART6->UART_BRR = UART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)9600);      break;
            case UART_BR_19200:    UART6->UART_BRR = UART_BRR_SAMPLING16((uint64_t)16000000, (uint64_t)19200);     break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                                   break;        
        }

        switch (ConfigPtr->wordLength)
        {
            case UART_WORDLENGTH_8B:   UART6->UART_CR1 &= (~(UART_CR1_M_Msk));    break;
            case UART_WORDLENGTH_9B:   UART6->UART_CR1 |= (UART_CR1_M_Msk);       break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                      break;        
        }

        switch (ConfigPtr->parityControl)
        {
            case UART_ParityDisable:        UART6->UART_CR1 &= (~(UART_CR1_PCE_Msk));                     break;
            case UART_ParityEnable_Even:    
                UART6->UART_CR1 |= (UART_CR1_PCE_Msk);  
                UART6->UART_CR1 &= (~(UART_CR1_PS_Msk));     
                break;
            case UART_ParityEnable_Odd:     UART6->UART_CR1 |= ((UART_CR1_PCE_Msk) | (UART_CR1_PS_Msk));  break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                          break;        
        }

        switch (ConfigPtr->stopBitNum)
        {
            case UART_1_BIT_Stop:   UART6->UART_CR2 &= (~(UART_CR2_STOP_Msk));    break;      /* 0x00 */
            case UART_2_BIT_Stop:   UART6->UART_CR2 |= (UART_CR2_STOP_1);         break;      /* 0x02 */
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                  break;        
        }
        
        switch (ConfigPtr->dmaState)
        {

            case UART_DISABLE_DMA:    UART6->UART_CR3 &= (~((UART_CR3_DMAT_Msk) | (UART_CR3_DMAR_Msk)));    break;
            case UART_ENABLE_DMAT:    
                UART6->UART_CR3 &= (~(UART_CR3_DMAR_Msk));       
                UART6->UART_CR3 |= (UART_CR3_DMAT_Msk);   
                break;
            case UART_ENABLE_DMAR:    
                UART6->UART_CR3 &= (~(UART_CR3_DMAT_Msk));       
                UART6->UART_CR3 |= (UART_CR3_DMAR_Msk);   
                break;
            case UART_ENABLE_DMA_FD:    UART6->UART_CR3 |= ((UART_CR3_DMAT_Msk) | (UART_CR3_DMAR_Msk));     break;
            default:    Loc_UARTerrorState =  UART_WRONG_INPUTS;                                            break;        
        }

        /*  Interrupt control   */
        if (ConfigPtr->TransDateRegisterEmpty_Int == 1)
            UART6->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->TransDateRegisterEmpty_Int == 0)
            UART6->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        if (ConfigPtr->ReceiveDateRegisterEmpty_Int == 1)
            UART6->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->ReceiveDateRegisterEmpty_Int == 0)
            UART6->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        if (ConfigPtr->TransDateRegisterEmpty_Int == 1)
            UART6->UART_CR1 |= (UART_CR1_TCIE_Msk);
        else if (ConfigPtr->TransDateRegisterEmpty_Int == 0)
            UART6->UART_CR1 &= (~(UART_CR1_TCIE_Msk));
        else
            Loc_UARTerrorState =  UART_WRONG_INPUTS;
        /***********************************************************/
        break;
    
    default:
        Loc_UARTerrorState = UART_WRONG_INPUTS;
        break;
    }

    return Loc_UARTerrorState;
}

/*    another init
void UART_Init(uint8_t Copy_uart_channel, UART_InitStruct *configPtr)
{
	UART_RegStruct *UART_basePtr = NULL_t_t;
	switch (Copy_uart_channel)
	{
	case UART1:
		UART_basePtr = (UART_RegStruct *)UART1_BASE;
		RCC_Enable(RCC_UART1);
		break;
	case UART2:
		UART_basePtr = (UART_RegStruct *)UART2_BASE;
		RCC_Enable(RCC_UART2);
		break;
	case UART6:
		UART_basePtr = (UART_RegStruct *)UART6_BASE;
		RCC_Enable(RCC_UART6);
		break;

	default:
		break;
	}


	// Configure stop bits
	// UART_basePtr->CR2 = (UART_basePtr->CR2 & ~(UART_CR2_STOP_ClrMsk)) | StopBits;
	REG_CLEARANDSET_BYMASKS(UART_basePtr->CR2, UART_CR2_STOP_clrMsk, configPtr->StopBits);

	// configure Wordlength, parity, TxorRX mode
	REG_CLEARANDSET_BYMASKS(UART_basePtr->UART_CR1,
							((uint32)(UART_CR1_M_clrMsk | UART_CR1_PCE_clrMsk | UART_CR1_PS_clrMsk | UART_CR1_TE_clrMsk | UART_CR1_RE_clrMsk)),
							((uint32)configPtr->wordLength | configPtr->Parity | configPtr->Mode));

	// configure baudrate
	UART_basePtr->BRR = UART_BRR_SAMPLING16(16000000, configPtr->BaudRate);

	// enable the uart
	(UART_basePtr->UART_CR1 |= UART_CR1_UE);

	return;
}
*/


UART_ErrorStatus_t UART_startTransmit_it(UART_ManagerStruct* Ptr_uartxManger,  const uint8_t *pData, uint16_t Size)
{
	UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

	if (Ptr_uartxManger->txState != UART_STATE_READY)
	{
		return UART_BUSY;
	}

	if ((pData == NULL_t) || (Size == 0U))
    {
      	return UART_ERROR;
    }

	Ptr_uartxManger->pTxBuffPtr  = pData;
    Ptr_uartxManger->TxXferSize  = Size;
    Ptr_uartxManger->TxXferCount = Size;

	// set busy state to indiacte that the USart is in use
	Ptr_uartxManger->txState = UART_STATE_BUSY_TX;

	// enable interrupt TXE when the tx is empty and ready to write the new data
	Ptr_uartxManger->Instance->UART_CR1 |= UART_CR1_TXEIE;

	return Loc_UARTerrorState;
}

static UART_ErrorStatus_t UART_Transmit_IT(UART_ManagerStruct *Ptr_uartxManger)
{
	//UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

	const uint16_t *tmp;

	/* Check that a Tx process is ongoing */
	if (Ptr_uartxManger->txState == UART_STATE_BUSY_TX)
	{
		if ((Ptr_uartxManger->init.wordLength == UART_WORDLENGTH_9B) && (Ptr_uartxManger->init.parityControl == UART_ParityDisable))
		{
		tmp = (const uint16_t *) Ptr_uartxManger->pTxBuffPtr;
		Ptr_uartxManger->Instance->UART_DR = (uint16_t)(*tmp & (uint16_t)0x01FF);
		Ptr_uartxManger->pTxBuffPtr += 2U;
		}
		else
		{
		Ptr_uartxManger->Instance->UART_DR = (uint8_t)(*Ptr_uartxManger->pTxBuffPtr++ & (uint8_t)0x00FF);
		}

		if (--(Ptr_uartxManger->TxXferCount) == 0U)
		{
		/* Disable the UART Transmit Data Register Empty Interrupt */
		Ptr_uartxManger->Instance->UART_CR1 &= ~UART_CR1_TXEIE_clrMsk;

		/* Enable the UART Transmit Complete Interrupt */
		Ptr_uartxManger->Instance->UART_CR1 |= UART_CR1_TCIE;
		}
		return UART_OK;
	}
	else
	{
		return UART_BUSY;
	}
}

static UART_ErrorStatus_t UART_endTransmit_IT(UART_ManagerStruct *usartxManger)
{
	/* Disable the UART Transmit Complete Interrupt */
	usartxManger->Instance->UART_CR1 &= ~UART_CR1_TCIE_clrMsk;

	/* Tx process is ended, restore huart->gState to Ready */
	usartxManger->txState = UART_STATE_READY;

	return UART_OK;
}

UART_ErrorStatus_t UART_startRecieve_it(UART_ManagerStruct *Ptr_uartxManger, uint8_t *pData, uint16_t Size)
{
	/* Check that a Rx process is not already ongoing */
	if (Ptr_uartxManger->rxState == UART_STATE_READY)
	{
		if ((pData == NULL_t) || (Size == 0U))
		{
			return UART_ERROR;
		}

		/* Set Reception type to Standard reception */
		// Ptr_uartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

		Ptr_uartxManger->pRxBuffPtr = pData;
		Ptr_uartxManger->RxXferSize = Size;
		Ptr_uartxManger->RxXferCount = Size;

		// to reset the error state if you handle it in the manager
		//   Ptr_uartxManger->ErrorCode = UART_ERROR_NONE;
		Ptr_uartxManger->rxState = UART_STATE_BUSY_RX;

		// 111111111111111 enable ERROR interrupts
		if (Ptr_uartxManger->init.parityControl != UART_ParityDisable)
		{
			/* Enable the UART Parity Error Interrupt */
			Ptr_uartxManger->Instance->UART_CR1 |= (UART_CR1_PEIE_Msk);
		}

		/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
		Ptr_uartxManger->Instance->UART_CR1 |= (UART_CR3_EIE_Msk);
		// 111111111111111

		/* Enable the UART Data Register not empty Interrupt */
		Ptr_uartxManger->Instance->UART_CR1 |= (UART_CR1_RXNEIE_Msk);

		return UART_OK;
	}
	else
	{
		return UART_BUSY;
	}
}

static UART_ErrorStatus_t UART_Receive_IT(UART_ManagerStruct *Ptr_uartxManger)
{
	uint8_t *pdata8bits;
	uint16_t *pdata16bits;

	/* Check that a Rx process is ongoing */
	if (Ptr_uartxManger->rxState == UART_STATE_BUSY_RX)
	{
		if ((Ptr_uartxManger->init.wordLength == UART_WORDLENGTH_9B) && (Ptr_uartxManger->init.parityControl == UART_ParityDisable))
		{
			pdata8bits = NULL_t;
			pdata16bits = (uint16_t *)Ptr_uartxManger->pRxBuffPtr;
			*pdata16bits = (uint16_t)(Ptr_uartxManger->Instance->UART_DR & (uint16_t)0x01FF);
			Ptr_uartxManger->pRxBuffPtr += 2U;
		}
		else
		{
			pdata8bits = (uint8_t *)Ptr_uartxManger->pRxBuffPtr;
			pdata16bits = NULL_t;

			if ((Ptr_uartxManger->init.wordLength == UART_WORDLENGTH_9B) || ((Ptr_uartxManger->init.wordLength == UART_WORDLENGTH_8B) && (Ptr_uartxManger->init.parityControl == UART_ParityDisable)))
			{
				*pdata8bits = (uint8_t)(Ptr_uartxManger->Instance->UART_DR & (uint8_t)0x00FF);
			}
			else
			{
				*pdata8bits = (uint8_t)(Ptr_uartxManger->Instance->UART_DR & (uint8_t)0x007F);
			}
			Ptr_uartxManger->pRxBuffPtr += 1U;
		}

		if (--(Ptr_uartxManger->RxXferCount) == 0U)
		{
			/* Disable the UART Data Register not empty Interrupt */
			Ptr_uartxManger->Instance->UART_CR1 &= ~(UART_CR1_RXNEIE_clrMsk);

		//11111111111111111 Disable Error Interrupts
			/* Disable the UART Parity Error Interrupt */
			Ptr_uartxManger->Instance->UART_CR1 &= ~(UART_CR1_PEIE_clrMsk);

			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
			Ptr_uartxManger->Instance->UART_CR1 &= ~(UART_CR3_EIE_clrMsk);
		//11111111111111111

			/* Rx process is completed, restore Ptr_uartxManger->RxState to Ready */
			Ptr_uartxManger->rxState = UART_STATE_READY;

			//   /* Initialize type of RxEvent to Transfer Complete */
			//   Ptr_uartxManger->RxEventType = HAL_UART_RXEVENT_TC;

			/* Check current reception Mode :
				If Reception till IDLE event has been selected : */
			// if (Ptr_uartxManger->ReceptionType == HAL_UART_RECEPTION_TOIDLE)
			// {
			// 	/* Set reception type to Standard */
			// 	Ptr_uartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

			// 	/* Disable IDLE interrupt */
			// 	ATOMIC_CLEAR_BIT(Ptr_uartxManger->Instance->UART_CR1, UART_CR1_IDLEIE);

			// 	/* Check if IDLE flag is set */
			// 	if (__HAL_UART_GET_FLAG(Ptr_uartxManger, UART_FLAG_IDLE))
			// 	{
			// 		/* Clear IDLE flag in ISR */
			// 		__HAL_UART_CLEAR_IDLEFLAG(Ptr_uartxManger);
			// 	}
			// }

			// else
			// {

			// }

			return UART_OK;
		}
		return UART_OK;
	}
	else
	{
		return UART_BUSY;
	}
}



/*****************************************************************************************/
/***********************************POLLING FUNCTIONS*************************************/
/*****************************************************************************************/
void UART_sendByte_polling(UART_ManagerStruct *usartxManger, const uint8_t data)
{
	/* 1. wait til the TDR is empty and ready to take-in data (wait for the TXE flag).
	 * 2. then write the data in the Data Register.
	 */

	// stop while the TX line is full  (wait while the TXE flag is clear)
	while (BIT_IS_CLEAR(usartxManger->Instance->UART_SR, UART_SR_TXE_Pos))
	{
	}

	// writing the data in the data Register DR
	usartxManger->Instance->UART_DR = data;

	// not nessecary    only nessecaery after the last byte sent by the uart to  indiacte that all is complete and its okay to disable the usart after it
	// while (!((USART_basePtr->SR)&(USART_SR_TC_Pos)))
}

uint8_t UART_recieveByte_polling(UART_ManagerStruct *usartxManger)
{
	/* 1. wait for the module to detect a byte on the line. that will raise a flag (RXNE) RX Not Empty. wait for the flag.
	 * 2. then read and return the recieved data.
	 */

	// stop while the RX line is empty  (wait while the RXNE flag is clear)
	while (BIT_IS_CLEAR(usartxManger->Instance->UART_SR, UART_SR_RXNE_Pos))
	{
	}

	// read and return the recieved data
	return usartxManger->Instance->UART_DR;
}



/*****************************************************************************************/
typedef enum
{
	RESET = 0U,
	SET = !RESET
} FlagStatus;

void HAL_UART_IRQHandler(UART_ManagerStruct *usartxManger)
{
	uint32_t isrflags = (usartxManger->Instance->UART_SR);
	uint32_t cr1its = (usartxManger->Instance->UART_CR1);
	uint32_t cr3its = (usartxManger->Instance->UART_CR3);
	uint32_t errorflags = 0x00U;
	uint32_t dmarequest = 0x00U;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(UART_SR_PE | UART_SR_FE | UART_SR_ORE | UART_SR_NE));
	if (errorflags == RESET)
	{
		/* UART in mode Receiver -------------------------------------------------*/
		if (((isrflags & UART_SR_RXNE) != RESET) && ((cr1its & UART_CR1_RXNEIE) != RESET))
		{
			UART_Receive_IT(usartxManger);
			return;
		}
	}

	/* If some errors occur Abort (cancel all recive it. and fn. and return)*/
	if ((errorflags != RESET) && (((cr3its & UART_CR3_EIE) != RESET) || ((cr1its & (UART_CR1_RXNEIE | UART_CR1_PEIE)) != RESET)))
	{
		/* UART parity error interrupt occurred ----------------------------------*/
		if (((isrflags & UART_SR_PE) != RESET) && ((cr1its & UART_CR1_PEIE) != RESET))
		{
			usartxManger->ErrorCode |= UART_ERROR_PE;
		}

		/* UART frame error interrupt occurred -----------------------------------*/
		if (((isrflags & UART_SR_FE) != RESET) && ((cr3its & UART_CR3_EIE) != RESET))
		{
			usartxManger->ErrorCode |= UART_ERROR_FE;
		}

		/* UART noise error interrupt occurred -----------------------------------*/
		if (((isrflags & UART_SR_NE) != RESET) && ((cr3its & UART_CR3_EIE) != RESET))
		{
			usartxManger->ErrorCode |= UART_ERROR_NE;
		}

		/* UART Over-Run interrupt occurred --------------------------------------*/
		if (((isrflags & UART_SR_ORE) != RESET) && (((cr1its & UART_CR1_RXNEIE) != RESET) || ((cr3its & UART_CR3_EIE) != RESET)))
		{
			usartxManger->ErrorCode |= UART_ERROR_ORE;
		}

		/* Call UART Error Call back function if need be --------------------------*/
		// if there is an error   recive if there is data in DR and then ABORT
		if (usartxManger->ErrorCode != UART_ERROR_NONE)
		{
			/* UART in mode Receiver -----------------------------------------------*/
			if (((isrflags & UART_SR_RXNE) != RESET) && ((cr1its & UART_CR1_RXNEIE) != RESET))
			{
				UART_Receive_IT(usartxManger);
			}

			// /* If Overrun error occurs, or if any error occurs in DMA mode reception,
			//    consider error as blocking */
			// dmarequest = HAL_IS_BIT_SET(usartxManger->Instance->CR3, USART_CR3_DMAR);
			// if (((usartxManger->ErrorCode & USART_ERROR_ORE) != RESET) || dmarequest)
			// {
			// 	/* Blocking error : transfer is aborted
			// 	   Set the UART state ready to be able to start again the process,
			// 	   Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
			// 	UART_EndRxTransfer(usartxManger);

			// 	/* Disable the UART DMA Rx request if enabled */
			// 	if (HAL_IS_BIT_SET(usartxManger->Instance->CR3, USART_CR3_DMAR))
			// 	{
			// 		ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_DMAR);

			// 		/* Abort the UART DMA Rx stream */
			// 		if (usartxManger->hdmarx != NULL)
			// 		{
			// 			/* Set the UART DMA Abort callback :
			// 			   will lead to call HAL_UART_ErrorCallback() at end of DMA abort procedure */
			// 			usartxManger->hdmarx->XferAbortCallback = UART_DMAAbortOnError;
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

				usartxManger->ErrorCode = UART_ERROR_NONE;
			}
		}
		return;
	} /* End if some error occurs */

	// 	/* Check current reception Mode :
	// 	   If Reception till IDLE event has been selected : */
	// 	if ((usartxManger->ReceptionType == HAL_UART_RECEPTION_TOIDLE) && ((isrflags & USART_SR_IDLE) != 0U) && ((cr1its & USART_SR_IDLE) != 0U))
	// 	{
	// 		__HAL_UART_CLEAR_IDLEFLAG(usartxManger);

	// 		/* Check if DMA mode is enabled in UART */
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

	// 				/* In Normal mode, end DMA xfer and HAL UART Rx process*/
	// 				if (usartxManger->hdmarx->Init.Mode != DMA_CIRCULAR)
	// 				{
	// 					/* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
	// 					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_PEIE);
	// 					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_EIE);

	// 					/* Disable the DMA transfer for the receiver request by resetting the DMAR bit
	// 					   in the UART CR3 register */
	// 					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_DMAR);

	// 					/* At end of Rx process, restore usartxManger->RxState to Ready */
	// 					usartxManger->rxState = USART_STATE_READY;
	// 					usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	// 					ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);

	// 					/* Last bytes received, so no need as the abort is immediate */
	// 					(void)HAL_DMA_Abort(usartxManger->hdmarx);
	// 				}

	// 				/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 				In this case, Rx Event type is Idle Event */
	// 				usartxManger->RxEventType = HAL_UART_RXEVENT_IDLE;

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
	// 				/* Disable the UART Parity Error Interrupt and RXNE interrupts */
	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));

	// 				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR3, USART_CR3_EIE);
	// //////////////////////////

	// 				/* Rx process is completed, restore usartxManger->RxState to Ready */
	// 				usartxManger->rxState = USART_STATE_READY;
	// 				usartxManger->ReceptionType = HAL_UART_RECEPTION_STANDARD;

	// 				ATOMIC_CLEAR_BIT(usartxManger->Instance->CR1, USART_CR1_IDLEIE);

	// 				/* Initialize type of RxEvent that correspond to RxEvent callback execution;
	// 				   In this case, Rx Event type is Idle Event */
	// 				usartxManger->RxEventType = HAL_UART_RXEVENT_IDLE;
	// 			}
	// 			return;
	// 		}
	// }

	/* UART in mode Transmitter ------------------------------------------------*/
	if (((isrflags & UART_SR_TXE) != RESET) && ((cr1its & UART_CR1_TXEIE) != RESET))
	{
		UART_Transmit_IT(usartxManger);
		return;
	}

	/* UART in mode Transmitter end --------------------------------------------*/
	if (((isrflags & UART_SR_TC) != RESET) && ((cr1its & UART_CR1_TCIE) != RESET))
	{
		UART_endTransmit_IT(usartxManger);
		return;
	}
}

/*****************************************************************************************/

UART_ErrorStatus_t UART_SendByteAsynchronous(UART_Channel_t Channel, uint8_t Copy_u8Data)
{
    UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

    

    return Loc_UARTerrorState;
}

UART_ErrorStatus_t UART_ReceiveBuffer(const UART_strConfigReceiveBuffer* ReceiveBuffer)
{
    UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

   

    return Loc_UARTerrorState;
}

UART_ErrorStatus_t UART_SendBufferZeroCopy(const UART_strConfigTransmitBuffer* Copy_sConfigBuffer)
{
    UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

   

    return Loc_UARTerrorState;
}

UART_ErrorStatus_t UART_RegisterCallBackFunction(UART_Channel_t Channel, UART_Mode_t Mode, CallBack Add_CallBackFunction)
{
    UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

   

    return Loc_UARTerrorState;
}

UART_ErrorStatus_t UART_ReceiveBufferDma(void)
{
    UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

   

    return Loc_UARTerrorState;
}

UART_ErrorStatus_t UART_SendBufferDma(void)
{
    UART_ErrorStatus_t Loc_UARTerrorState = UART_OK;

   

    return Loc_UARTerrorState;
}