



#include <stdint.h>
#include "../include/MCAL/RCC.h"
#include "../include/MCAL/GPIO.h"
#include "../include/MCAL/STK.h"


int main(void)
{
    GPIO_StrCfg_t Loc_arrStrGpios[NUM_OF_PINS] = {
        [0] = 
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN0,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Low
        },

        [1] = 
        {
            .port = GPIO_PORTB,
            .pin = GPIO_PIN0,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Low
        }
    };
    
    

    GPIO_InitPin(Loc_arrStrGpios);
/*
    GPIO_SetPinState(Loc_arrStrGpios[0].port, Loc_arrStrGpios[0].pin, GPIO_PINSTATE_HIGH);
    GPIO_TogglePinState(Loc_arrStrGpios[0].port, Loc_arrStrGpios[0].pin);
*/
    //GPIO_TogglePinState(Loc_arrStrGpios[0].port, Loc_arrStrGpios[0].pin);

    STK_SetTime_ms(7000);
    STK_Start();

    uint32_t expire;

    while (1)
    {
        STK_IsExpire(&expire);
        if (expire)
        {
            GPIO_TogglePinState(Loc_arrStrGpios[0].port, Loc_arrStrGpios[0].pin);
        }
        
    }

    /*
    uint8_t i;
    for (i = 0; i < NUM_OF_PINS; i++)
    {
        GPIO_SetPinState(Loc_arrStrGpios[i].port, Loc_arrStrGpios[i].pin, GPIO_PINSTATE_HIGH);
    }
*/
    /*
    GPIO_SetPinState(GPIO_PORTA, GPIO_PIN0, GPIO_PINSTATE_HIGH);
    GPIO_SetPinState(GPIO_PORTA, GPIO_PIN4, GPIO_PINSTATE_HIGH);
    */
    return 0;
}