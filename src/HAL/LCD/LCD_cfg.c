

#include "GPIO.h"
#include "LCD.h"

LCD_cfg_t LCDs_PinCfg =
    {
        //   .LCD_data_pins[0]
        //   .pin = GPIO_PIN0,
        //   .LCD_data_pins[0].port = GPIO_PORTA,
        //   .LCD_data_pins[1].pin  = GPIO_PIN1,
        //   .LCD_data_pins[1].port = GPIO_PORTA,
        //   .LCD_data_pins[2].pin  = GPIO_PIN2,
        //   .LCD_data_pins[2].port = GPIO_PORTA,
        //   .LCD_data_pins[3].pin  = GPIO_PIN3,
        //   .LCD_data_pins[3].port = GPIO_PORTA,
        .LCD_data_pins[0].pin = GPIO_PIN4,
        .LCD_data_pins[0].port = GPIO_PORTA,
        .LCD_data_pins[1].pin = GPIO_PIN5,
        .LCD_data_pins[1].port = GPIO_PORTA,
        .LCD_data_pins[2].pin = GPIO_PIN6,
        .LCD_data_pins[2].port = GPIO_PORTA,
        .LCD_data_pins[3].pin = GPIO_PIN7,
        .LCD_data_pins[3].port = GPIO_PORTA,
        .R_S_pin.pin = GPIO_PIN10,
        .R_S_pin.port = GPIO_PORTA,
        .R_W_pin.pin = GPIO_PIN9,
        .R_W_pin.port = GPIO_PORTA,
        .E_pin.pin = GPIO_PIN8,
        .E_pin.port = GPIO_PORTA};
