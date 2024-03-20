/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "../../include/MCAL/GPIO_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

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
        },

        [2] = 
        {
            .port = GPIO_PORTA,
            .pin = GPIO_PIN7,
            .mode = GPIO_MODE_Output,
            .out_type = GPIO_OUTPUT_PushPull,
            .pupd = GPIO_Default,
            .speed = GPIO_SPEED_Low
        }
    };

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


