/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */

/* Driver Libraries   */
#include "../../include/MCAL/STK.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

typedef struct
{
    volatile uint32_t STK_CTRL;               /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */                                                              
    volatile uint32_t STK_LOAD;               /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
    volatile uint32_t STK_VAL;                /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
    volatile const uint32_t STK_CALIB;        /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
}SYSTICK;

#define STK_BASE_ADDR   0xE000E010UL
#define STK     ((volatile SYSTICK* const)(STK_BASE_ADDR))



/* SysTick Control/Status Register Definitions */
#define STK_MASK_CTRL_COUNTFLAG_Pos         16U                                            /*!< SysTick CTRL: COUNTFLAG Position */
#define STK_MASK_CTRL_COUNTFLAG_Msk         (1UL << SysTick_CTRL_COUNTFLAG_Pos)            /*!< SysTick CTRL: COUNTFLAG Mask */
#define STK_MASK_CTRL_CLKSOURCE_Pos          2U                                            /*!< SysTick CTRL: CLKSOURCE Position */
#define STK_MASK_CTRL_CLKSOURCE_Msk         (1UL << SysTick_CTRL_CLKSOURCE_Pos)            /*!< SysTick CTRL: CLKSOURCE Mask */
#define STK_MASK_CTRL_TICKINT_Pos            1U                                            /*!< SysTick CTRL: TICKINT Position */
#define STK_MASK_CTRL_TICKINT_Msk           (1UL << SysTick_CTRL_TICKINT_Pos)              /*!< SysTick CTRL: TICKINT Mask */
#define STK_MASK_CTRL_ENABLE_Pos             0U                                            /*!< SysTick CTRL: ENABLE Position */
#define STK_MASK_CTRL_ENABLE_Msk            (1UL /*<< SysTick_CTRL_ENABLE_Pos*/)           /*!< SysTick CTRL: ENABLE Mask */
/* SysTick load Register Definitions */
#define STK_MASK_LOAD_RELOAD_Pos             0U                                            /*!< SysTick LOAD: RELOAD Position */
#define STK_MASK_LOAD_RELOAD_Msk            (0xFFFFFFUL /*<< SysTick_LOAD_RELOAD_Pos*/)    /*!< SysTick LOAD: RELOAD Mask */
/* SysTick current Register Definitions */
#define STK_MASK_VAL_CURRENT_Pos             0U                                            /*!< SysTick VAL: CURRENT Position */
#define STK_MASK_VAL_CURRENT_Msk            (0xFFFFFFUL /*<< SysTick_VAL_CURRENT_Pos*/)    /*!< SysTick VAL: CURRENT Mask */
/* SysTick calibration Register Definitions */
#define STK_MASK_CALIB_NOREF_Pos            31U                                            /*!< SysTick CALIB: NOREF Position */
#define STK_MASK_CALIB_NOREF_Msk            (1UL << SysTick_CALIB_NOREF_Pos)               /*!< SysTick CALIB: NOREF Mask */
#define STK_MASK_CALIB_SKEW_Pos             30U                                            /*!< SysTick CALIB: SKEW Position */
#define STK_MASK_CALIB_SKEW_Msk             (1UL << SysTick_CALIB_SKEW_Pos)                /*!< SysTick CALIB: SKEW Mask */
#define STK_MASK_CALIB_TENMS_Pos             0U                                            /*!< SysTick CALIB: TENMS Position */
#define STK_MASK_CALIB_TENMS_Msk            (0xFFFFFFUL /*<< SysTick_CALIB_TENMS_Pos*/)    /*!< SysTick CALIB: TENMS Mask */


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

MCAL_ErrorStatus_t STK_SetTime_ms(uint32_t Copy_PreloadVal)
{
    MCAL_ErrorStatus_t Loc_STKErrorState = MCAL_OK;

    if (Copy_PreloadVal > 0x00FFFFFF || Copy_PreloadVal < 0x01)
    {
        Loc_STKErrorState = MCAL_WRONG_INPUTS;
    }
    else    
    {
        STK->STK_LOAD = Copy_PreloadVal;
    }
    
    return Loc_STKErrorState;
}
MCAL_ErrorStatus_t STK_Start()
{
    MCAL_ErrorStatus_t Loc_STKErrorState = MCAL_OK;

    STK->STK_CTRL |= 0x01;

    return Loc_STKErrorState;
}
MCAL_ErrorStatus_t STK_Stop()
{
    MCAL_ErrorStatus_t Loc_STKErrorState = MCAL_OK;

    STK->STK_CTRL |= 0x00;

    return Loc_STKErrorState;
}
MCAL_ErrorStatus_t STK_IsExpire(uint32_t* Loc_ExpireStatus)
{
    MCAL_ErrorStatus_t Loc_STKErrorState = MCAL_OK;

    return Loc_STKErrorState;
}
MCAL_ErrorStatus_t STK_SetCallBack(uint32_t Copy_PreloadVal)
{
    MCAL_ErrorStatus_t Loc_STKErrorState = MCAL_OK;

    return Loc_STKErrorState;
}