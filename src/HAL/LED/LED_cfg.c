

#include "GPIO.h"
#include "LED.h"

const LED_cfg_t Leds[__LED_Num] =
{
    [LED_Toggle] = 
    {
        .Port = GPIO_PORTA,
        .Pin = GPIO_PIN0,
        .Direction = LED_DIR_FORWARD
    }

};
