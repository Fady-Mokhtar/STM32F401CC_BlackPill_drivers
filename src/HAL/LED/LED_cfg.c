

#include "GPIO.h"
#include "LED.h"

const LED_cfg_t Leds[__LED_Num] =
{
    [LED_1] = 
    {
        .Port = GPIO_PORTB,
        .Pin = GPIO_PIN0,
        .Direction = LED_DIR_FORWARD
    },
    [LED_2] = 
    {
        .Port = GPIO_PORTB,
        .Pin = GPIO_PIN1,
        .Direction = LED_DIR_FORWARD
    }

};
