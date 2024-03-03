/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Driver Libraries   */
#include "../../include/MCAL/GPIO.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

typedef struct 
{
    volatile uint32_t GPIOx_MODER;
    volatile uint32_t GPIOx_OTYPER;
    volatile uint32_t GPIOx_OSPEEDR;
    volatile uint32_t GPIOx_PUPDR;
    volatile uint32_t GPIOx_IDR;
    volatile uint32_t GPIOx_ODR;
    volatile uint32_t GPIOx_BSRR;
    volatile uint32_t GPIOx_LCKR;
    volatile uint32_t GPIOx_AFRL;
    volatile uint32_t GPIOx_AFRH;
    
}GPIO;


/*                      Masks                       */
#define GPIO_MASK_RCC_PORT     (((Copy_strCfg_ptr->port) - GPIO_PORTA) /0x400)


/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

MCAL_ErrorStatus_t GPIO_InitPin(GPIO_StrCfg_t *Copy_strCfg_ptr)
{ 
    MCAL_ErrorStatus_t Loc_GPIOErrorState = MCAL_OK;
    MCAL_ErrorStatus_t Loc_RCCErrorState = MCAL_OK;

    if(Copy_strCfg_ptr == NULL_t)
    {
        Loc_GPIOErrorState = MCAL_NULL_PTR;
    }
    else if (Copy_strCfg_ptr->port == NULL_t)
    {
        Loc_GPIOErrorState = MCAL_NULL_PTR;
    }
    else if (Copy_strCfg_ptr->port < GPIO_PORTA || Copy_strCfg_ptr->port > GPIO_PORTH)
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    else if (Copy_strCfg_ptr->pin < GPIO_PIN0 || Copy_strCfg_ptr->pin > GPIO_PIN15)
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    else if (Copy_strCfg_ptr->mode < GPIO_MODE_Input || Copy_strCfg_ptr->mode > GPIO_MODE_Analog)
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    else if (Copy_strCfg_ptr->speed < GPIO_SPEED_Low || Copy_strCfg_ptr->speed > GPIO_SPEED_VeryHigh)
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    else
    {
       // ((GPIO *)(Copy_strCfg_ptr->port))->
        RCC_enableAHB1Peripheral(GPIO_MASK_RCC_PORT);               //1st Method
    }
    

    return Loc_GPIOErrorState;
}

MCAL_ErrorStatus_t GPIO_SetPinState(void* port, uint32_t Copy_PinNum, uint32_t Copy_PinState)
{
    MCAL_ErrorStatus_t Loc_GPIOErrorState = MCAL_OK;

    return Loc_GPIOErrorState;
}

MCAL_ErrorStatus_t GPIO_GetPinState(void* port, uint32_t Copy_PinNum, uint8_t* Copy_PinState)
{
    MCAL_ErrorStatus_t Loc_GPIOErrorState = MCAL_OK;


    return Loc_GPIOErrorState;
}
