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
    
}MGPIO;


/*                      Init_Masks                       */
#define GPIO_MASK_RCC_PORT    (1 << (((Copy_strCfg_ptr[i].port) - GPIO_PORTA) /0x400))

#define GPIO    ((volatile MGPIO* const)(Copy_strCfg_ptr[i].port))

/*                      SetPin_GetPin_Masks                    */
#define GPIOPINx    ((volatile MGPIO* const)(port))

#define MASK1           0x01
#define BSRR_CLR_MASK   0x10


/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

MCAL_ErrorStatus_t GPIO_InitPin(GPIO_StrCfg_t *Copy_strCfg_ptr)
{ 
    MCAL_ErrorStatus_t Loc_GPIOErrorState = MCAL_OK;
    //MCAL_ErrorStatus_t Loc_RCCErrorState = MCAL_OK;

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
        
        uint8_t i;
        for (i = 0; i < NUM_OF_PINS; i++)
        {
            RCC_enableAHB1Peripheral(GPIO_MASK_RCC_PORT);               //1st Method
            GPIO->GPIOx_OSPEEDR |= (Copy_strCfg_ptr[i].speed << (Copy_strCfg_ptr[i].pin * 2));
            GPIO->GPIOx_PUPDR |= (Copy_strCfg_ptr[i].pupd << (Copy_strCfg_ptr[i].pin * 2));
            GPIO->GPIOx_MODER |= (Copy_strCfg_ptr[i].mode << (Copy_strCfg_ptr[i].pin * 2));
            GPIO->GPIOx_OTYPER |= (Copy_strCfg_ptr[i].out_type << (Copy_strCfg_ptr[i].pin * 2));
        }
        
        
    }
    

    return Loc_GPIOErrorState;
}

MCAL_ErrorStatus_t GPIO_SetPinState(void *port, GPIO_PINS_t Copy_PinNum, GPIO_PinState_t Copy_PinState)
{
    MCAL_ErrorStatus_t Loc_GPIOErrorState = MCAL_OK;
/*
    if (Copy_PinState == 0)
    {
        GPIOPINx->GPIOx_BSRR = Copy_PinState << (Copy_PinNum + BSRR_CLR_MASK);
    }
    else if (Copy_PinState == 1)   
    {
        GPIOPINx->GPIOx_BSRR = Copy_PinState << Copy_PinNum;
    }
    else    
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
*/
    /*     another method (old one)         */
    
    if (Copy_PinState == 0)
    {
        GPIOPINx->GPIOx_ODR &= ~(Copy_PinState << Copy_PinNum);
    }
    else if (Copy_PinState == 1)   
    {
         GPIOPINx->GPIOx_ODR |=  Copy_PinState << Copy_PinNum;
    }
    else    
    {
        Loc_GPIOErrorState = MCAL_WRONG_INPUTS;
    }
    
    return Loc_GPIOErrorState;
}

MCAL_ErrorStatus_t GPIO_GetPinState(void *port, GPIO_PINS_t Copy_PinNum, uint8_t* Copy_PinState)
{
    MCAL_ErrorStatus_t Loc_GPIOErrorState = MCAL_OK;

    *Copy_PinState = ((GPIOPINx->GPIOx_IDR >> Copy_PinNum) & MASK1);

    return Loc_GPIOErrorState;
}
