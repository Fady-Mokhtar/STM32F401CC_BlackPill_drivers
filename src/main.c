



#include <stdint.h>
#include "RCC.h"
#include "GPIO.h"
#include "GPIO_cfg.h"
#include "STK.h"
#include "Schedular.h"


extern GPIO_StrCfg_t Loc_arrStrGpios[NUM_OF_PINS];


//  systick_test

/*
void nvic_test_tog_led(void)
{
    uint32_t expire;

  
    STK_IsExpire(&expire);
    if (expire)
    {
        GPIO_TogglePinState(Loc_arrStrGpios[0].port, Loc_arrStrGpios[0].pin);
    }
        
    
}

*/

#define green   0
#define yellow  1
#define red     2



void Traffic_Green(void)
{
    //GPIO_TogglePinState(Loc_arrStrGpios[red].port, Loc_arrStrGpios[red].pin);
    GPIO_TogglePinState(Loc_arrStrGpios[green].port, Loc_arrStrGpios[green].pin);
}

void Traffic_Yellow(void)
{
    //GPIO_TogglePinState(Loc_arrStrGpios[green].port, Loc_arrStrGpios[green].pin);
    GPIO_TogglePinState(Loc_arrStrGpios[yellow].port, Loc_arrStrGpios[yellow].pin);
}

void Traffic_Red(void)
{
    //GPIO_TogglePinState(Loc_arrStrGpios[yellow].port, Loc_arrStrGpios[yellow].pin);
    GPIO_TogglePinState(Loc_arrStrGpios[red].port, Loc_arrStrGpios[red].pin);
}

int main(void)
{
      
    GPIO_InitPin(Loc_arrStrGpios);

    
    sched_init();
    sched_start();

    while (1)
    {
        
    }
    
    //GPIO_SetPinState(Loc_arrStrGpios[0].port, Loc_arrStrGpios[0].pin, GPIO_PINSTATE_HIGH);

/*
    STK_SetTime_ms(2000);
    STK_SetCallBack(nvic_test_tog_led);
    STK_Start();

    while (1)
    {
        
    }
*/ 
    
/*
    uint32_t expire;

    while (1)
    {
        STK_IsExpire(&expire);
        if (expire)
        {
            GPIO_TogglePinState(Loc_arrStrGpios[0].port, Loc_arrStrGpios[0].pin);
        }
        
    }

 */   /*
    uint8_t i;
    for (i = 0; i < NUM_OF_PINS; i++)
    {
        GPIO_SetPinState(Loc_arrStrGpios[i].port, Loc_arrStrGpios[i].pin, GPIO_PINSTATE_HIGH);
    }
*/

    return 0;
}