#ifndef E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_ERRORS_H_
#define E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_ERRORS_H_


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
 * @brief Enumeration defining error types for MCAL.
 */
typedef enum
{
    MCAL_OK             = 0x00U,

    MCAL_NOK            = 0x01U,

    MCAL_NULL_PTR       = 0x02U,

    MCAL_WRONG_INPUTS   = 0x03U

}MCAL_ErrorStatus_t;

/**
 * @brief Enumeration defining error types for UART.
 */
typedef enum
{
	UART_OK = 0,
	UART_NULL_PTR,
	UART_VALUE_EXCEED,
	UART_BUSY, 
    UART_WRONG_INPUTS,
    UART_ERROR


}UART_ErrorStatus_t;

/**
 * @brief Enumeration defining error types for DMA.
 */
typedef enum
{
    DMA_OK             = 0x00U,

    DMA_NOK            = 0x01U,

    DMA_NULL_PTR       = 0x02U,

    DMA_WRONG_INPUTS   = 0x03U

}DMA_ErrorStatus_t;

/**
 * @brief Enumeration defining error types for HAL.
 */
typedef enum
{
    HAL_OK             = 0x00U,

    HAL_NOK            = 0x01U,

    HAL_NULL_PTR       = 0x02U,

    HAL_WRONG_INPUTS   = 0x03U

}HAL_ErrorStatus_t;


#endif // E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_ERRORS_H_