/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "../../include/MCAL/NVIC.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

typedef struct 
{
/*
    volatile uint32_t NVIC_ISER0;
    volatile uint32_t NVIC_ISER1;
    volatile uint32_t NVIC_ISER2;
    volatile uint32_t NVIC_ISER3;
    volatile uint32_t NVIC_ISER4;
    volatile uint32_t NVIC_ISER5;
    volatile uint32_t NVIC_ISER6;
    volatile uint32_t NVIC_ISER7;
*/
    volatile uint32_t NVIC_ISER[8U];
    volatile uint32_t RESERVED[24U];
    volatile uint32_t NVIC_ICER[8U];
    volatile uint32_t RESERVED[24U];
    volatile uint32_t NVIC_ISPR[8U];
    volatile uint32_t RESERVED[24U];
    volatile uint32_t NVIC_ICPR[8U];
    volatile uint32_t RESERVED[24U];
    volatile uint32_t NVIC_IABR[8U];
    volatile uint32_t RESERVED[56U];
    volatile uint8_t  NVIC_IPR[240U];           //60*4=240 uint8_t
    volatile uint32_t RESERVED[644U];
    volatile uint32_t NVIC_STIR;

}MNVIC;

#define NVIC_BASE_ADDR      (0xE000E100UL) 
#define NVIC    ((volatile MNVIC* const)(NVIC_BASE_ADDR))
/*
    #define NVIC_STIR_OFFSET    0xE00UL
    #define NVIC_STIR   (volatile uint32_t NVIC_STIR)(NVIC_BASE_ADDR + NVIC_STIR_OFFSET)
*/
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


//void NVIC_SetPriorityGrouping(uint32_t priority_grouping);                        
void NVIC_EnableIRQ(IRQn_t IRQn)                                                 
{
    if(IRQn >=0)
        NVIC->NVIC_ISER[IRQn/32] = 1 << (IRQn % 32); 
}
/**
  \brief   Enable Interrupt
  \details Enables a device specific interrupt in the NVIC interrupt controller.
  \param [in]      IRQn  Device specific interrupt number.
  \note    IRQn must not be negative.
 */
/*
__STATIC_INLINE void __NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    __COMPILER_BARRIER();
    NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    __COMPILER_BARRIER();
  }
}
*/

void NVIC_DisableIRQ(IRQn_t IRQn)
{
    if(IRQn >=0)
    {
        NVIC->NVIC_ICER[IRQn/32] = 1 << (IRQn % 32); 
    }  
}       

void NVIC_SetPendingIRQ (IRQn_t IRQn)
{
    if(IRQn >=0)
    {
        NVIC->NVIC_ISPR[IRQn/32] = 1 << (IRQn % 32);
    }
}       

void NVIC_ClearPendingIRQ (IRQn_t IRQn)
{
    if(IRQn >=0)
    {
        NVIC->NVIC_ICPR[IRQn/32] = 1 << (IRQn % 32); 
    } 
}         

uint32_t NVIC_GetPendingIRQ (IRQn_t IRQn)
{
    if(IRQn >=0)
    {
        return (NVIC->NVIC_ISPR[IRQn/32] << (IRQn % 32));
    }
    else
    {
        return 0U;
    }
}          

uint32_t NVIC_GetActive (IRQn_t IRQn)
{
    if(IRQn >=0)
    {
        return (NVIC->NVIC_IABR[IRQn/32] << (IRQn % 32)); 
    }
    else
    {
        return 0U;
    } 
}         

void NVIC_SetPriority (IRQn_t IRQn, uint32_t priority)
{
    if(IRQn >=0)
        NVIC->NVIC_IPR[IRQn] = (uint8_t)priority; 

}           
/**
  \brief   Set Interrupt Priority
  \details Sets the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]      IRQn  Interrupt number.
  \param [in]  priority  Priority to set.
  \note    The priority cannot be set for every processor exception.
 */
/*
__STATIC_INLINE void __NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->IP[((uint32_t)IRQn)]               = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
  else
  {
    SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
}
*/

uint8_t NVIC_GetPriority (IRQn_t IRQn)
{
    if(IRQn >=0)
    {
        return NVIC->NVIC_IPR[IRQn]; 
    }
    else
    {
        return 0U;
    }  
}            
/**
  \brief   Get Interrupt Priority
  \details Reads the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  \param [in]   IRQn  Interrupt number.
  \return             Interrupt Priority.
                      Value is aligned automatically to the implemented priority bits of the microcontroller.
 */
/*
__STATIC_INLINE uint32_t __NVIC_GetPriority(IRQn_Type IRQn)
{

  if ((int32_t)(IRQn) >= 0)
  {
    return(((uint32_t)NVIC->IP[((uint32_t)IRQn)]               >> (8U - __NVIC_PRIO_BITS)));
  }
  else
  {
    return(((uint32_t)SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] >> (8U - __NVIC_PRIO_BITS)));
  }
}
*/

void NVIC_SystemReset (void)
{

}                                                     