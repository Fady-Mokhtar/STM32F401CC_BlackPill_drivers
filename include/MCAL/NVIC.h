/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "../../lib/Errors.h"
#include "../../lib/STD_TYPES.h"
#include "../STM32F401CC.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/


//void NVIC_SetPriorityGrouping(uint32_t priority_grouping);                        // Set the priority grouping
void NVIC_EnableIRQ(IRQn_t IRQn);                                                 // Enable IRQn
void NVIC_DisableIRQ(IRQn_t IRQn);                                                // Disable IRQn
uint32_t NVIC_GetPendingIRQ (IRQn_t IRQn);                                        // Return true (IRQ-Number) if IRQn is pending
void NVIC_SetPendingIRQ (IRQn_t IRQn);                                            // Set IRQn pending
void NVIC_ClearPendingIRQ (IRQn_t IRQn);                                          // Clear IRQn pending status
uint32_t NVIC_GetActive (IRQn_t IRQn);                                            // Return the IRQ number of the active interrupt
void NVIC_SetPriority (IRQn_t IRQn, uint32_t priority);                           // Set priority for IRQn
uint8_t NVIC_GetPriority (IRQn_t IRQn);                                          // Read priority of IRQn
void NVIC_SystemReset (void);                                                     // Reset the system
