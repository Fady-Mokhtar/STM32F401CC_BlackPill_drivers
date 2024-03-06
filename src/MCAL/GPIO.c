/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Driver Libraries   */
#include "../../include/MCAL/GPIO.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/**
  * @brief  GPIO peripheral register map.
  * @note   This structure defines the memory-mapped registers of a GPIO peripheral.
  *         Each register corresponds to a specific function of the GPIO peripheral,
  *         such as pin mode, output type, output speed, pull-up/pull-down configuration,
  *         input data, output data, etc.
  *         The structure is designed to be used with direct memory access (DMA) to
  *         interact with the GPIO peripheral registers efficiently.
  */
typedef struct 
{
    volatile uint32_t GPIOx_MODER;    /*!< GPIO port mode register */
    volatile uint32_t GPIOx_OTYPER;   /*!< GPIO port output type register */
    volatile uint32_t GPIOx_OSPEEDR;  /*!< GPIO port output speed register */
    volatile uint32_t GPIOx_PUPDR;    /*!< GPIO port pull-up/pull-down register */
    volatile uint32_t GPIOx_IDR;      /*!< GPIO port input data register */
    volatile uint32_t GPIOx_ODR;      /*!< GPIO port output data register */
    volatile uint32_t GPIOx_BSRR;     /*!< GPIO port bit set/reset register */
    volatile uint32_t GPIOx_LCKR;     /*!< GPIO port configuration lock register */
    volatile uint64_t GPIOx_AFR;      /*!< GPIO port alternate function register */
    /*
    volatile uint32_t GPIOx_AFRL;   // GPIO port alternate function low register
    volatile uint32_t GPIOx_AFRH;   // GPIO port alternate function high register
    */
} MGPIO;


/* Init_Masks */

/*
 * RCC port mask for GPIO initialization.
 * This mask is used to enable the clock for the GPIO port being initialized.
 * The RCC port mask is calculated based on the base address of the GPIO port.
 */
#define GPIO_MASK_RCC_PORT    (1 << (((Copy_strCfg_ptr[i].port) - GPIO_PORTA) /0x400))

/*
 * GPIO register access macro.
 * This macro is used to access the GPIO registers for a specific port.
 * It casts the base address of the GPIO port to a pointer to the MGPIO structure.
 */
#define GPIO    ((volatile MGPIO* const)(Copy_strCfg_ptr[i].port))


/* SetPin_GetPin_Masks */

/*
 * GPIO pin register access macro.
 * This macro is used to access the GPIO registers for a specific pin.
 * It casts the base address of the GPIO port to a pointer to the MGPIO structure.
 */
#define GPIOPINx    ((volatile MGPIO* const)(port))

/*
 * Mask for bit manipulation.
 * This mask is used for setting or clearing individual bits in a register.
 */
#define MASK1           0x01

/*
 * Bit set/reset register clear mask.
 * This mask is used to clear a specific bit in the BSRR (bit set/reset) register.
 */
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

    /*     another method (old one)         */
 /*   
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
*/    
    return Loc_GPIOErrorState;
}

MCAL_ErrorStatus_t GPIO_GetPinState(void *port, GPIO_PINS_t Copy_PinNum, uint8_t* Copy_PinState)
{
    MCAL_ErrorStatus_t Loc_GPIOErrorState = MCAL_OK;

    *Copy_PinState = ((GPIOPINx->GPIOx_IDR >> Copy_PinNum) & MASK1);

    return Loc_GPIOErrorState;
}
