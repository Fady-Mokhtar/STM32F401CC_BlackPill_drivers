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

/*      BASE_ADDRs for each PORT          */
#define GPIO_PORTA     (void*)(0x40020000UL)
#define GPIO_PORTB     (void*)(0x40020400UL)
#define GPIO_PORTC     (void*)(0x40020800UL)
#define GPIO_PORTD     (void*)(0x40020C00UL)
#define GPIO_PORTE     (void*)(0x40021000UL)
#define GPIO_PORTH     (void*)(0x40021C00UL)

/*         AFRLy selection Masks          */
#define      GPIO_MASK_AF0         0000
#define      GPIO_MASK_AF1         0001
#define      GPIO_MASK_AF2         0010
#define      GPIO_MASK_AF3         0011
#define      GPIO_MASK_AF4         0100
#define      GPIO_MASK_AF5         0101
#define      GPIO_MASK_AF6         0110
#define      GPIO_MASK_AF7         0111
#define      GPIO_MASK_AF8         1000
#define      GPIO_MASK_AF9         1001
#define      GPIO_MASK_AF10        1010
#define      GPIO_MASK_AF11        1011
#define      GPIO_MASK_AF12        1100
#define      GPIO_MASK_AF13        1101
#define      GPIO_MASK_AF14        1110
#define      GPIO_MASK_AF15        1111

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
    uint32_t mode;
    uint32_t speed;

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
    GPIO_MODE_Input,                                              //(reset state)
    GPIO_MODE_Output,
    GPIO_MODE_Alternatefunction,
    GPIO_MODE_Analog
}GPIO_ModeState_t;

typedef enum  
{
    GPIO_OUTPUT_PushPull,                                          //(reset state)
    GPIO_OUTPUT_OpenDrain
}GPIO_OutputType_t;

typedef enum  
{
    GPIO_SPEED_Low,
    GPIO_SPEED_Medium,
    GPIO_SPEED_High,
    GPIO_SPEED_VeryHigh
}GPIO_SpeedState_t;

typedef enum  
{
    GPIO_NO_PUPD,
    GPIO_PU,
    GPIO_PD,
    GPIO_Reserved
}GPIO_PUPDRState_t;

typedef enum  
{
    GPIO_NO_PUPD,
    GPIO_PU,
    GPIO_PD,
    GPIO_Reserved
}GPIO_PUPDRState_t;



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

MCAL_ErrorStatus_t GPIO_InitPin(GPIO_StrCfg_t *Copy_strCfg_ptr);

MCAL_ErrorStatus_t GPIO_SetPinState(void* Port, GPIO_PINS_t Copy_PinNum, GPIO_PinState_t Copy_PinState);

MCAL_ErrorStatus_t GPIO_GetPinState(void* Port, GPIO_PINS_t Copy_PinNum, uint8_t* Copy_PinState);








#endif // MCAL_GPIO_GPIO_H_