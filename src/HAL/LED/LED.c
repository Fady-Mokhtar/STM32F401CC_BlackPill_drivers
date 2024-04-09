

#include "LED.h"
#include "GPIO.h"

extern const LED_cfg_t Leds[__LED_Num];

HALStatus_t LED_Init()
{
    HALStatus_t LOC_Status = HAL_NOK;
    GPIO_StrCfg_t Led[__LED_Num];
    uint8_t index;

    for (index = 0; index < __LED_Num; index++)
    {
        Led[index].pin = Leds[index].Pin;
        Led[index].port = Leds[index].Port;
        Led[index].speed = GPIO_SPEED_High;
        Led[index].mode = GPIO_MODE_Output;
        Led[index].out_type = GPIO_OUTPUT_PushPull;
        Led[index].pupd = GPIO_NO_PUPD;
    }
    LOC_Status = GPIO_Init(Led);
    return LOC_Status;
}

HALStatus_t LED_SetState(uint32_t Led, uint8_t State)
{
    HALStatus_t LOC_Status = HAL_NOK;
    LOC_Status = GPIO_SetPinState(Leds[Led].Port, Leds[Led].Pin, Leds[Led].Direction ^ State);
    return LOC_Status;
}

HALStatus_t LED_ToggleLed(uint32_t Led)
{
    HALStatus_t LOC_Status = HAL_NOK;
    uint8_t LOC_State;
    LOC_Status = GPIO_GetPinState(Leds[Led].Port, Leds[Led].Pin, &LOC_State);
    LOC_Status = GPIO_SetPinState(Leds[Led].Port, Leds[Led].Pin, LOC_State ^ 1);

    return LOC_Status;
}