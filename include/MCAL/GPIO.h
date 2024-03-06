#ifndef MCAL_GPIO_GPIO_H_
#define MCAL_GPIO_GPIO_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "../../lib/Errors.h"
#include "../../lib/STD_TYPES.h"

/* Driver Libraries   */
#include "RCC.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define NUM_OF_PINS     2

/*      BASE_ADDRs for each PORT          */
#define GPIO_PORTA     (void*)(0x40020000UL)
#define GPIO_PORTB     (void*)(0x40020400UL)
#define GPIO_PORTC     (void*)(0x40020800UL)
#define GPIO_PORTD     (void*)(0x40020C00UL)
#define GPIO_PORTE     (void*)(0x40021000UL)
#define GPIO_PORTH     (void*)(0x40021C00UL)

/*         AFRLy selection Masks          */
#define      GPIO_MASK_AF0         0b0000
#define      GPIO_MASK_AF1         0b0001
#define      GPIO_MASK_AF2         0b0010
#define      GPIO_MASK_AF3         0b0011
#define      GPIO_MASK_AF4         0b0100
#define      GPIO_MASK_AF5         0b0101
#define      GPIO_MASK_AF6         0b0110
#define      GPIO_MASK_AF7         0b0111
#define      GPIO_MASK_AF8         0b1000
#define      GPIO_MASK_AF9         0b1001
#define      GPIO_MASK_AF10        0b1010
#define      GPIO_MASK_AF11        0b1011
#define      GPIO_MASK_AF12        0b1100
#define      GPIO_MASK_AF13        0b1101
#define      GPIO_MASK_AF14        0b1110
#define      GPIO_MASK_AF15        0b1111

/*
#define GPIO_PIN0       0X00
#define GPIO_PIN1       0X01
#define GPIO_PIN2       0X02
#define GPIO_PIN3       0X03
#define GPIO_PIN4       0X04
#define GPIO_PIN5       0X05
#define GPIO_PIN6       0X06
#define GPIO_PIN7       0X07
#define GPIO_PIN8       0X08
#define GPIO_PIN9       0X09
#define GPIO_PIN10      0X0A
#define GPIO_PIN11      0X0B
#define GPIO_PIN12      0X0C
#define GPIO_PIN13      0X0D
#define GPIO_PIN14      0X0E
#define GPIO_PIN15      0X0F
*/


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct
{
    void* port;
    uint32_t pin;
    uint32_t speed;                         
    uint32_t mode;
    uint32_t AF;                            /*         AFRLy selection Masks          */
    uint32_t out_type;
    uint32_t pupd;

}GPIO_StrCfg_t;

typedef enum
{
    GPIO_PIN0, 
    GPIO_PIN1, 
    GPIO_PIN2, 
    GPIO_PIN3, 
    GPIO_PIN4, 
    GPIO_PIN5, 
    GPIO_PIN6, 
    GPIO_PIN7, 
    GPIO_PIN8, 
    GPIO_PIN9, 
    GPIO_PIN10,
    GPIO_PIN11,
    GPIO_PIN12,
    GPIO_PIN13,
    GPIO_PIN14,
    GPIO_PIN15
}GPIO_PINS_t;

typedef enum
{
    GPIO_PINSTATE_LOW,
    GPIO_PINSTATE_HIGH
}GPIO_PinState_t;

typedef enum  
{
    GPIO_MODE_Default = 0,
    GPIO_MODE_Input = 0,                                              //(reset state)
    GPIO_MODE_Output,
    GPIO_MODE_Alternatefunction,
    GPIO_MODE_Analog
}GPIO_ModeState_t;

typedef enum  
{
    GPIO_OUTPUT_Default = 0,
    GPIO_OUTPUT_PushPull = 0,                                          //(reset state)
    GPIO_OUTPUT_OpenDrain
}GPIO_OutputType_t;

typedef enum  
{
    GPIO_SPEED_Default = 0,
    GPIO_SPEED_Low = 0,
    GPIO_SPEED_Medium,
    GPIO_SPEED_High,
    GPIO_SPEED_VeryHigh
}GPIO_SpeedState_t;

typedef enum  
{
    GPIO_Default = 0,
    GPIO_NO_PUPD = 0,
    GPIO_PU,
    GPIO_PD,
    GPIO_Reserved
}GPIO_PUPDRState_t;




/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

MCAL_ErrorStatus_t GPIO_InitPin(GPIO_StrCfg_t *Copy_strCfg_ptr);

MCAL_ErrorStatus_t GPIO_SetPinState(void *Port, GPIO_PINS_t Copy_PinNum, GPIO_PinState_t Copy_PinState);

MCAL_ErrorStatus_t GPIO_GetPinState(void *Port, GPIO_PINS_t Copy_PinNum, uint8_t *Copy_PinState);








#endif // MCAL_GPIO_GPIO_H_