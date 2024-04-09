

//#if APP == USART_test

//#define usart_test
#ifdef usart_test

// #include <stdint.h>
// #include "RCC.h"
// #include "STK.h"
// #include "Schedular.h"
#include "GPIO.h"
#include "GPIO_cfg.h"
#include "USART.h"
#include "delay.h"


extern GPIO_StrCfg_t Loc_arrStrGpios[__NUM_OF_PINS];
extern USART_ManagerStruct USART1Manager;

uint8_t usart_recieve_buffer[5] = {'0', '0', '0', '0', '0'};

int main(void)
{

	/* Initialize all configured peripherals */
	GPIO_Init(Loc_arrStrGpios);
	USART_Init(&(USART1Manager));

	// UART interrupts testing

	/*
		char data;

		while(1)
		{
			data = USART_recieveByte_polling(&USART1Manager); // Receive Byte from Terminal1
			USART_sendByte_polling(&USART1Manager, data);
		}
	*/

	// delay_ms(1000);
	if (USART_startRecieve_IT(&USART1Manager, usart_recieve_buffer, 5) != MCAL_OK)
	{
		while (1)
		{
		}
	}

	delay_ms(5000);
	if (USART_startTransmit_IT(&USART1Manager, usart_recieve_buffer, 5) != MCAL_OK)
	{
		while (1)
		{
		}
	}

	while (1)
	{
	}

	return 0;
}

/* Templates */
// USART Polling
/**
 *   data = USART_recieveByte_polling(&usart1Manager); // Receive Byte from Terminal1
 *   USART_sendByte_polling(&usart1Manager, data);
 */

// I2C transmit
/**
 * 	uint8 i2cdata[6] = {'B', 'o', 'u', 'l', 'e', 's'};
 * 	uint8 i2cdata2[4] = {'F', 'A', 'D', 'Y'};
 *
 * 	I2C_Master_Transmit(1, 0x35, &i2cdata, 6);
 * 	delay_ms(7000);
 * 	I2C_Master_Transmit(1, 0x35, &i2cdata2, 4);
 * 	delay_ms(7000);
 */

// STK
/**
 * STK_stop();
 * STK_setTime_ms(7000);
 * STK_start();
 * while ( STK_isExpire() == 0);
 *
 */

#endif