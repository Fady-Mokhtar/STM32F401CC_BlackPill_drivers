// #ifndef MCAL_DMA_DMA_INTERFACE_H_
// #define MCAL_DMA_DMA_INTERFACE_H_

// /********************************************************************************************************/
// /************************************************Includes************************************************/
// /********************************************************************************************************/

// /* Include Libraries  */
// #include "../../../lib/Errors.h"
// #include "../../../lib/STD_TYPES.h"
// #include "../../../lib/BIT_MATH.h"

// /* Driver Libraries   */
// //#include "DMA_cfg.h"

// /********************************************************************************************************/
// /************************************************Defines*************************************************/
// /********************************************************************************************************/

// /*Register Address*/
// typedef struct
// {
// 	volatile uint32_t LISR;
// 	volatile uint32_t HISR;
// 	volatile uint32_t LIFCR;
// 	volatile uint32_t HIFCR;

// 	struct
// 	{
// 		volatile uint32_t SxCR;
// 		volatile uint32_t SxNDTR;
// 		volatile uint32_t SxPAR;
// 		volatile uint32_t SxM0AR;
// 		volatile uint32_t SxM1AR;
// 		volatile uint32_t SxFCR;

// 	}Cfg_Reg[8];

// }DMA_Typedef;


// #define DMA1_REGISTER	((DMA_Typedef*)0x40026000)

// #define DMA2_REGISTER	((DMA_Typedef*)0x40026400)



// /**************Private Value******************/
// static DMA_CallBackFunc CallFunction[2][8];


// /********************************************************************************************************/
// /************************************************Types***************************************************/
// /********************************************************************************************************/

// typedef enum
// {
//     DMA1,
//     DMA2,

//     __NUM_OF_DMA

// }DMA_Id;

// typedef enum
// {
// 	DMA_Stream0 = 0,
// 	DMA_Stream1 = 1,
// 	DMA_Stream2 = 2,
// 	DMA_Stream3 = 3,
// 	DMA_Stream4 = 4,
// 	DMA_Stream5 = 5,
// 	DMA_Stream6 = 6,
// 	DMA_Stream7 = 7,


//     __NUM_OF_STREAMS

// }DMA_StreamId;

// typedef enum
// {
// 	DMA_Channel0 = 0,
// 	DMA_Channel1 = 1,
// 	DMA_Channel2 = 2,
// 	DMA_Channel3 = 3,
// 	DMA_Channel4 = 4,
// 	DMA_Channel5 = 5,
// 	DMA_Channel6 = 6,
// 	DMA_Channel7 = 7,


//     __NUM_OF_CHANNELS

// }DMA_ChannelId;


// typedef enum
// {
// 	DMA_Low = 0,
// 	DMA_Medium,
// 	DMA_High,
// 	DMA_VeryHigh

// }DMA_Priority_t;

// typedef enum
// {
// 	Byte = 0,
// 	Half_Word,
// 	Word,

// }DMA_Size_t;

// typedef enum
// {
// 	Per_to_Mem = 0,
// 	Mem_to_Per,
// 	Mem_to_Mem

// }DMA_Direction_t;

// typedef enum{

// 	DMA_DISABLE = 0,
// 	DMA_ENABLE

// }DMA_State;

// typedef enum
// {
// 	Single = 0,
// 	Inc_4_Beats,
// 	Inc_8_Beats,
// 	Inc_16_Beats

// }DMA_Burst_t;

// /*
//  * The struct is made to configure the DMA Channel
//  *
//  * !! Check all user define types (enum) above for each Mode as use from it
//  *
//  * !! TO USE IN FUCNTION :: DMA_enuConfiguration
//  *
//  * */
// typedef struct
// {
// 	DMA_StreamId    Stream;
// 	DMA_ChannelId   Channel;
//     DMA_Priority_t  Priority_Cfg;
//     DMA_Direction_t Data_Direction;

// 	DMA_Burst_t Memory_Burst;
// 	DMA_Burst_t Peripheral_Burst;

// 	DMA_Size_t	Memory_Size;
// 	DMA_Size_t	Peripheral_Size;

// 	DMA_State   Mem_Auto_Increment;
// 	DMA_State   Per_Auto_Increment;
//     DMA_State   Double_Buffer;
// 	DMA_State   Circular_Mode;
	
// 	struct
// 	{
// 		DMA_State Direct_Mode;
// 		DMA_State Transfer_Err;
// 		DMA_State HalfTransfer;
// 		DMA_State Transfer_Complete;

// 	}DMA_Interrupt_t;

// }DMA_Config_t;


// typedef struct
// {
// 	DMA_StreamId Stream;
// 	uint32_t Peripheral_Address;
// 	uint32_t Memory_0_Address;
// 	uint32_t Memory_1_Address;
// 	uint16_t Block_Length;

// }Criteria_t;


// /********************************************************************************************************/
// /************************************************APIs****************************************************/
// /********************************************************************************************************/


// void DMA_Init(DMA_InitTypeDef * Ptr_strInit);

// extern DMA_ErrorStatus_t DMA_setConfiguration(DMA_Id DMA, const DMA_config_t* ConfigPtr);

// extern DMA_ErrorStatus_t DMA_setCritariaChannel(DMA_Id DMA, const Criteria_t* CriteriaPtr);

// extern DMA_ErrorStatus_t DMA_setRegisterCallFunction(DMA_Id DMA, DMA_StreamId Stream, DMA_CallBackFunc Cbf);

// extern DMA_ErrorStatus_t DMA_setStreamState(DMA_Id DMA, DMA_StreamId Stream, DMA_State Condition);



// void DMA_voidDMAEnable( uint8_t Copy_uint8_tChannel );

// void DMA_voidDMADisable( uint8_t Copy_uint8_tChannel );

// void DMA_voidInerruptEnable( uint8_t Copy_uint8_tChannel , uint8_t Copy_uint8_tINTSource );

// void DMA_voidSetAddress( uint8_t Copy_uint8_tChannel , uint32_t * PeripheralAddress , uint32_t * MemoryAddress , uint16_t Copy_u16NDT );

// void DMA_voidClearFlag( uint8_t Copy_uint8_tChannel , uint8_t Copy_uint8_tFlag );

// uint8_t   DMA_uint8_tGetFlag( uint8_t Copy_uint8_tChannel , uint8_t Copy_uint8_tFlag );

// void DMA_voidSetCallBackChannel1( void ( *Ptr ) ( void ) );



// #endif // MCAL_DMA_DMA_INTERFACE_H_