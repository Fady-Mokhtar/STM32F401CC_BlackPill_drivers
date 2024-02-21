/*===============================================================================================================
*
* @date 10/2/2024
*
* Author: Fady
*
* Layer: MCAL
*
* Module: RCC
*
* File Name: RCC_cfg.h
*
*===============================================================================================================*/

/* Include Libraries  */
#include "../../STD_TYPES/STD_TYPES.h"

/* Driver Libraries   */
#include "RCC.h"

/*==============================================================================================================*/

/*==============================================================================================================*/

/* Register Definitions (1st method using macros) */
/*
#define RCC_CR            *((volatile u32*)0x40023800)
#define RCC_PLLCFGR       *((volatile u32*)0x40023804)
#define RCC_CFGR          *((volatile u32*)0x40023804)
#define RCC_CIR           *((volatile u32*)0x40023808)
   //     and so on
*/
/*==============================================================================================================*/

/* Register Definitions (2nd method using struct) */

typedef struct
{
    volatile u32 CR;
    volatile u32 PLLCFGR;
    volatile u32 CFGR;
    volatile u32 CIR;
    volatile u32 AHB1RSTR;
    volatile u32 AHB2RSTR;
    volatile u32 RESERVED1[2];;
    volatile u32 APB1RSTR;
    volatile u32 APB2RSTR;
    volatile u32 RESERVED2[2];
    volatile u32 AHB1ENR;
    volatile u32 AHB2ENR;
    volatile u32 RESERVED3[2];
    volatile u32 APB1ENR;
    volatile u32 APB2ENR;
    volatile u32 RESERVED4[2];
    volatile u32 AHB1LPENR;
    volatile u32 AHB2LPENR;
    volatile u32 RESERVED5[2];
    volatile u32 APB1LPENR;
    volatile u32 APB2LPENR;
    volatile u32 RESERVED6[2];
    volatile u32 BDCR;
    volatile u32 CSR;
    volatile u32 RESERVED7[2];
    volatile u32 SSCGR;
    volatile u32 PLLI2SCFGR;
    volatile u32 RESERVED8;
    volatile u32 DCKCFGR;
}MRCC;

#define RCC_BASE_ADDRESS (0x40023800UL)
#define RCC ((volatile MRCC* const)(RCC_BASE_ADDRESS))

/*==============================================================================================================*/
/*                                                    Defines                                                   */
/*==============================================================================================================*/
#define RCC_CFGR_SW_MASK  ((u32)0x3 << 0)
#define RCC_CFGR_SWS_MASK ((u32)0x3 << 2)

#define RCC_PLLCFGR_PLLSRC_OFFSET (22)
#define RCC_PLLCFGR_PLLSRC_MASK ((u32)1 << RCC_PLLCFGR_PLLSRC_OFFSET)

#define RCC_PLLCFGR_PLLM_OFFSET (0)
#define RCC_PLLCFGR_PLLM_MASK ((u32)0x3F << RCC_PLLCFGR_PLLM_OFFSET)

#define RCC_PLLCFGR_PLLN_OFFSET (6)
#define RCC_PLLCFGR_PLLN_MASK ((u32)0x1FF << RCC_PLLCFGR_PLLN_OFFSET)

#define RCC_PLLCFGR_PLLP_OFFSET (16)
#define RCC_PLLCFGR_PLLP_MASK ((u32)0x3 << RCC_PLLCFGR_PLLP_OFFSET)

#define RCC_PLLCFGR_PLLQ_OFFSET (24)
#define RCC_PLLCFGR_PLLQ_MASK ((u32)0xF << RCC_PLLCFGR_PLLQ_OFFSET)

#define RCC_CFGR_HPRE_OFFSET (4)
#define RCC_CFGR_HPRE_MASK ((u32)0xF << RCC_CFGR_HPRE_OFFSET)

#define RCC_CFGR_PPRE1_OFFSET (10)
#define RCC_CFGR_PPRE1_MASK ((u32)0x7 << RCC_CFGR_PPRE1_OFFSET)

#define RCC_CFGR_PPRE2_OFFSET (13)
#define RCC_CFGR_PPRE2_MASK ((u32)0x7 << RCC_CFGR_PPRE2_OFFSET)

#define RCC_TIMEOUT_VAL     (600)




/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

MCAL_ErrorStatus_t RCC_enableClock(RCC_SystemClock_t Clock)
{
    /* Enable the clock*/
    RCC->CR |= Clock;
    u32 clockStatusMask = (Clock << 1);

    u32 timeout = RCC_TIMEOUT_VAL;
    while(!(RCC->CR & clockStatusMask) && timeout){timeout--;}

    return (RCC->CR & clockStatusMask)? MCAL_OK : MCAL_NOK;
}

MCAL_ErrorStatus_t RCC_disableClock(RCC_SystemClock_t Clock)
{
    RCC->CR &= ~Clock;
    return MCAL_OK;
}


MCAL_ErrorStatus_t RCC_selectSystemClock(RCC_SystemClock_t SystemClock)
{
    u32 CFGRtmp = RCC->CFGR;
    CFGRtmp = (CFGRtmp & ~RCC_CFGR_SW_MASK) | (u32)SystemClock;

    RCC->CFGR = CFGRtmp;
    return MCAL_OK;
}


MCAL_ErrorStatus_t RCC_getSystemClock(RCC_SystemClock_t *SystemClock)
{
    *SystemClock = (((RCC->CFGR) & RCC_CFGR_SWS_MASK) >> 2);

    return MCAL_OK;
}


MCAL_ErrorStatus_t RCC_configurePLLClock(RCC_PLLConfig_t *PLLConfig)
{
    u32 PLLCFGRtmp = RCC->PLLCFGR;

    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLSRC_MASK) | (u32)PLLConfig->PLLSrc;

    /* Trim the prescaler value using its mask */
    u32 PLLM_value = ((u32)PLLConfig->PLLM & (RCC_PLLCFGR_PLLM_MASK >> RCC_PLLCFGR_PLLM_OFFSET));
    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLM_MASK) | (PLLM_value << RCC_PLLCFGR_PLLM_OFFSET);

    u32 PLLN_value = ((u32)PLLConfig->PLLN & (RCC_PLLCFGR_PLLN_MASK >> RCC_PLLCFGR_PLLN_OFFSET));
    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLN_MASK) | (PLLN_value << RCC_PLLCFGR_PLLN_OFFSET);

    u32 PLLP_value = ((u32)PLLConfig->PLLP & (RCC_PLLCFGR_PLLP_MASK >> RCC_PLLCFGR_PLLP_OFFSET));
    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLP_MASK) | (PLLP_value << RCC_PLLCFGR_PLLP_OFFSET);

    u32 PLLQ_value = ((u32)PLLConfig->PLLQ & (RCC_PLLCFGR_PLLQ_MASK >> RCC_PLLCFGR_PLLQ_OFFSET));
    PLLCFGRtmp = (PLLCFGRtmp & ~RCC_PLLCFGR_PLLQ_MASK) | (PLLQ_value << RCC_PLLCFGR_PLLQ_OFFSET);   

    RCC->PLLCFGR = PLLCFGRtmp;

    return MCAL_OK;
}


MCAL_ErrorStatus_t RCC_enableAHB1Peripheral(RCC_AHB1Peri_t AHB1Peripheral)
{
    RCC->AHB1ENR |= AHB1Peripheral;
    return MCAL_OK;
}


MCAL_ErrorStatus_t RCC_enableAHB2Peripheral(RCC_AHB2Peri_t AHB2Peripheral)
{
    RCC->AHB2ENR |= AHB2Peripheral;
    return MCAL_OK;
}

MCAL_ErrorStatus_t RCC_enableAPB1Peripheral(RCC_APB1Peri_t APB1Peripheral)
{
    RCC->APB1ENR |= APB1Peripheral;
    return MCAL_OK;
}

MCAL_ErrorStatus_t RCC_enableAPB2Peripheral(RCC_APB2Peri_t APB2Peripheral)
{
    RCC->APB2ENR |= APB2Peripheral;
    return MCAL_OK;
}

MCAL_ErrorStatus_t RCC_disableAHB1Peripheral(RCC_AHB1Peri_t AHB1Peripheral)
{
    RCC->AHB1ENR &= ~AHB1Peripheral;
    return MCAL_OK;
}

MCAL_ErrorStatus_t RCC_disableAHB2Peripheral(RCC_AHB2Peri_t AHB2Peripheral)
{
    RCC->AHB2ENR &= ~AHB2Peripheral;
    return MCAL_OK;
}

MCAL_ErrorStatus_t RCC_disableAPB1Peripheral(RCC_APB1Peri_t APB1Peripheral)
{
    RCC->APB1ENR &= ~APB1Peripheral;
    return MCAL_OK;
}

MCAL_ErrorStatus_t RCC_disableAPB2Peripheral(RCC_APB2Peri_t APB2Peripheral)
{
    RCC->APB2ENR &= ~APB2Peripheral;
    return MCAL_OK;
}

MCAL_ErrorStatus_t RCC_selectSystemClockPrescalers(RCC_AHB1Prescaler_t AHB1Prescaler, RCC_APB1Prescaler_t APB1Prescaler, RCC_APB2Prescaler_t APB2Prescaler)
{
    u32 CFGRtmp = RCC->CFGR;

    CFGRtmp = (CFGRtmp & ~RCC_CFGR_HPRE_MASK)  | AHB1Prescaler;
    CFGRtmp = (CFGRtmp & ~RCC_CFGR_PPRE1_MASK) | APB1Prescaler;
    CFGRtmp = (CFGRtmp & ~RCC_CFGR_PPRE2_MASK) | APB2Prescaler;

    RCC->CFGR = CFGRtmp;

    return MCAL_OK;
}