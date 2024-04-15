
// #if (APP == LCD_test)

//#define lcd_test
#ifdef lcd_test

/* Includes ------------------------------------------------------------------*/
#include "std_types.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "dma.h"
#include "Schedular.h"
#include "stk.h"
#include "nvic.h"
#include "led.h"
#include "lcd.h"

//#define __NUM_OF_PINS     15
extern GPIO_StrCfg_t Loc_arrStrGpios[__NUM_OF_PINS_LED];

void LCD_Write();
void tog_led();

int main(void)
{

  /* Initialize all configured peripherals */
  RCC_enableAHB1Peripheral(RCC_AHB1PERIPHERAL_GPIOA);
  // GPIO_Init(Loc_arrStrGpios);

  LED_Init();
  LCD_Init();
  sched_init();
  sched_start();

  while (1)
  {
  }

  return 0;
}

void LCD_Write()
{
  LCD_WriteStringAsync("hello", 5, tog_led);
  LCD_SetCursorAsync(0, 10, NULL);

  LCD_WriteStringAsync("ahmed", 5, NULL);
  LCD_SetCursorAsync(0, 10, NULL);

  LCD_WriteStringAsync("nour", 5, NULL);

  LCD_WriteStringAsync("hello", 5, tog_led);
  LCD_SetCursorAsync(1, 0, NULL);

  LCD_WriteStringAsync("ahmed", 5, NULL);
  LCD_SetCursorAsync(1, 8, NULL);

  LCD_WriteStringAsync("nour", 5, NULL);
}

void tog_led()
{
  LED_ToggleLed(0);
}

#endif /*LCD_test*/