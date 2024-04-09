

#ifndef LED_H_
#define LED_H_

#include "STD_TYPES.h"
#include "Errors.h"
#include "LED_cfg.h"

#define LED_DIR_FORWARD 0
#define LED_DIR_REVERSE 1
#define LED_STATE_ON 1
#define LED_STATE_OFF 0

typedef struct
{
    void *Port;
    uint32_t Pin;
    uint8_t Direction;
} LED_cfg_t;

/*
 * use this function to initialize all the leds you configured
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
HALStatus_t LED_Init();

/*
 * use this function to set the sate of a led
 * Parameters:
 * Led -> the name of the desired led
 * State -> LED_STATE_ON, LED_STATE_OFF
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
HALStatus_t LED_SetState(uint32_t Led, uint8_t State);

/*
 * use this function to toggle the state of a led
 * Parameters:
 * Led -> the name of the desired led
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
HALStatus_t LED_ToggleLed(uint32_t Led);

#endif