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


#define PORTA_BASE_ADDR     (void*)(0x40020000UL)
#define PORTB_BASE_ADDR     (void*)(0x40020400UL)
#define PORTC_BASE_ADDR     (void*)(0x40020800UL)
#define PORTD_BASE_ADDR     (void*)(0x40020C00UL)
#define PORTE_BASE_ADDR     (void*)(0x40021000UL)
#define PORTH_BASE_ADDR     (void*)(0x40021C00UL)

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

}GPIO_strCfg_t;

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
}GPIO_PINSTATE_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

MCAL_ErrorStatus_t GPIO_InitPin(GPIO_strCfg_t *Copy_strCfg_ptr);

MCAL_ErrorStatus_t GPIO_SetPinState(void* Port, GPIO_PINS_t Copy_PinNum, GPIO_PINSTATE_t Copy_PinState);

MCAL_ErrorStatus_t GPIO_GetPinState(void* Port, GPIO_PINS_t Copy_PinNum, uint8_t* Copy_PinState);








#endif // MCAL_GPIO_GPIO_H_