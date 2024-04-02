// /********************************************************************************************************/
// /************************************************Includes************************************************/
// /********************************************************************************************************/

// /* Include Libraries  */

// /* Driver Libraries   */
// #include "DMA_private.h"
// #include "DMA_interface.h"
// #include "DMA_cfg.h"




// /********************************************************************************************************/
// /*********************************************APIs Implementation****************************************/
// /********************************************************************************************************/


// void ( *DMA_CallBackFunc )( void );

// /*
// void DMA_Init(DMA_InitTypeDef * Ptr_strInit)
// {
//     Ptr_strInit->Instance
// }
// */
// DMA_ErrorStatus_t DMA_setConfig(DMA_Id Dma, const Dma_config_t* ConfigPtr)
// {
// 	DMA_ErrorStatus_t Loc_enuErrorStatus = DMA_OK;

// 	uint32_t Loc_u32TempRegister = 0;

// 	if(ConfigPtr)
// 	{
// 		if(Dma == DMA1)
// 		{
// 			Loc_u32TempRegister = ConfigPtr->Channel << 25 | ConfigPtr->Memory_Burst << 23 | ConfigPtr->Peripheral_Burst << 21 |
// 									ConfigPtr->Double_Buffer << 18 | ConfigPtr->Priority_Cfg << 16 | ConfigPtr->Memory_Size << 13 |
// 									ConfigPtr->Peripheral_Size << 11 | ConfigPtr->Mem_Auto_Increment << 10 | ConfigPtr->Per_Auto_Increment << 9 |
// 									ConfigPtr->Circular_Mode << 8 |  ConfigPtr->Data_Direction << 6 | ConfigPtr->Interrupt.Transfer_Complete << 4 |
// 									ConfigPtr->Interrupt.HalfTransfer << 3 | ConfigPtr->Interrupt.Transfer_Err << 2 |
// 									ConfigPtr->Interrupt.Direct_Mode << 1;

// 			/*Store the configuration in the register*/
// 			DMA1_REGISTER->Cfg_Reg[ConfigPtr->Stream].SxCR = Loc_u32TempRegister;
// 		}


// 		else if(Dma == DMA2)
// 		{
// 			Loc_u32TempRegister = ConfigPtr->Channel << 25 | ConfigPtr->Memory_Burst << 23 | ConfigPtr->Peripheral_Burst << 21 |
// 									ConfigPtr->Double_Buffer << 18 | ConfigPtr->Priority_Cfg << 16 | ConfigPtr->Memory_Size << 13 |
// 									ConfigPtr->Peripheral_Size << 11 | ConfigPtr->Mem_Auto_Increment << 10 | ConfigPtr->Per_Auto_Increment << 9 |
// 									ConfigPtr->Circular_Mode << 8 |  ConfigPtr->Data_Direction << 6 | ConfigPtr->Interrupt.Transfer_Complete << 4 |
// 									ConfigPtr->Interrupt.HalfTransfer << 3 | ConfigPtr->Interrupt.Transfer_Err << 2 |
// 									ConfigPtr->Interrupt.Direct_Mode << 1;

// 			/*Store the configuration in the register*/
// 			DMA2_REGISTER->Cfg_Reg[ConfigPtr->Stream].SxCR = Loc_u32TempRegister;
// 		}

// 		else
// 		{
// 			Loc_enuErrorStatus = DMA_WRONG_INPUTS;
// 		}
// 	}

// 	else
// 	{
// 		Loc_enuErrorStatus = DMA_NULL_PTR;
// 	}

// 	return Loc_enuErrorStatus;

// }



// DMA_ErrorStatus_t Dma_enuCritariaChannel(DMA_Id Dma, const Criteria_t* CriteriaPtr)
// {
// 	DMA_ErrorStatus_t Loc_enuErrorStatus = DMA_OK;

// 	if(CriteriaPtr)
// 	{
// 		if(Dma == DMA1)
// 		{
// 			DMA1_REGISTER->Cfg_Reg[CriteriaPtr->Stream].SxPAR = CriteriaPtr->Peripheral_Address;
// 			DMA1_REGISTER->Cfg_Reg[CriteriaPtr->Stream].SxM0AR = CriteriaPtr->Memory_0_Address;
// 			DMA1_REGISTER->Cfg_Reg[CriteriaPtr->Stream].SxM1AR = CriteriaPtr->Memory_1_Address;
// 			DMA1_REGISTER->Cfg_Reg[CriteriaPtr->Stream].SxNDTR = CriteriaPtr->Block_Length;
// 		}

// 		else if(Dma == DMA2)
// 		{
// 			DMA2_REGISTER->Cfg_Reg[CriteriaPtr->Stream].SxPAR = CriteriaPtr->Peripheral_Address;
// 			DMA2_REGISTER->Cfg_Reg[CriteriaPtr->Stream].SxM0AR = CriteriaPtr->Memory_0_Address;
// 			DMA2_REGISTER->Cfg_Reg[CriteriaPtr->Stream].SxM1AR = CriteriaPtr->Memory_1_Address;
// 			DMA2_REGISTER->Cfg_Reg[CriteriaPtr->Stream].SxNDTR = CriteriaPtr->Block_Length;
// 		}

// 		else
// 		{
// 			Loc_enuErrorStatus = DMA_WRONG_INPUTS;
// 		}
// 	}

// 	else
// 	{
// 		Loc_enuErrorStatus = DMA_NULL_PTR;
// 	}

// 	return Loc_enuErrorStatus;
// }



// DMA_ErrorStatus_t Dma_enuRegisterCallFunction(DMA_Id Dma, DMA_StreamId Stream, Dma_CallFunction Cbf)
// {
// 	DMA_ErrorStatus_t Loc_enuErrorStatus = DMA_OK;

// 	if(Cbf)
// 	{

// 		CallFunction[Dma][Stream] = Cbf;
// 	}
// 	else
// 	{
// 		Loc_enuErrorStatus = DMA_NULL_PTR;
// 	}

// 	return Loc_enuErrorStatus;
// }


// DMA_ErrorStatus_t Dma_enuStreamState(DMA_Id Dma, DMA_StreamId Stream, DMA_State Condition)
// {
// 	DMA_ErrorStatus_t Loc_enuErrorStatus = DMA_OK;

// 	if(Stream > 7)
// 	{
// 		Loc_enuErrorStatus = DMA_WRONG_INPUTS;
// 	}


// 	switch (Dma)
// 	{
// 	case DMA1:
// 		DMA1_REGISTER->Cfg_Reg[Stream].SxCR |= Condition;
// 		break;

// 	case DMA2:
// 		DMA2_REGISTER->Cfg_Reg[Stream].SxCR |= Condition;
// 		break;

// 	default:
// 		Loc_enuErrorStatus = DMA_WRONG_INPUTS;
// 		break;

// 	}

// 	return Loc_enuErrorStatus;

// }



// void DMA1_Stream0_IRQHandler(void)
// {
// 	if((DMA1_REGISTER->LISR >> 5 & 1) == 1)
// 	{
// 		if(CallFunction[Dma_Stream0])
// 		{
// 			DMA1_REGISTER->LIFCR &= ~(1 << 5);
// 			CallFunction[DMA1][Dma_Stream0]();
// 		}
// 	}
// 	else
// 	{
// 		//Include Error
// 	}

// }

// void DMA1_Stream1_IRQHandler(void)
// {

// }

// void DMA1_Stream2_IRQHandler(void)
// {

// }

// void DMA1_Stream3_IRQHandler(void)
// {

// }

// void DMA1_Stream4_IRQHandler(void)
// {

// }



// void DMA1_Stream6_IRQHandler(void)
// {

// }

// void DMA1_Stream7_IRQHandler(void)
// {

// }


// /*****DMA2 Handler**************/

// void DMA2_Stream7_IRQHandler(void)
// {
// 	if((DMA2_REGISTER->HISR >> 27 & 1) == 1)
// 	{
// 		if(CallFunction[Dma_Stream7])
// 		{
// 			DMA2_REGISTER->HIFCR |= 1 << 27;
// 			CallFunction[DMA2][Dma_Stream7]();
// 		}
// 	}
// 	else
// 	{
// 		//Include Error
// 	}
// }

// void DMA2_Stream5_IRQHandler(void)
// {
// 	if((DMA2_REGISTER->HISR >> 11 & 1) == 1)
// 	{
// 		if(CallFunction[Dma_Stream5])
// 		{
// 			DMA2_REGISTER->HIFCR |= 1 << 11;
// 			CallFunction[DMA2][Dma_Stream5]();
// 		}
// 	}
// 	else
// 	{
// 		//Include Error
// 	}
// }

// /*----------------------------------------------------------------------------------------------------------*/
// void DMA_voidSetCallBackChannel1( void ( *Ptr ) ( void ) ){

// 	DMA_CallBackChannel1 = Ptr ;

// }

// void DMA1_Channel1_IRQHandler(void){

// 	DMA_CallBackChannel1();

// }