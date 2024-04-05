#ifndef E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_DELAY_H_
#define E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_DELAY_H_





#ifndef F_CPU
#define F_CPU 16000000
#endif

# define delay_ms(numof_milliseconds)   {	unsigned long cycles = ((F_CPU)/7000UL) * numof_milliseconds; \
											for (unsigned long i = 0; i < cycles; i++){\
												__asm volatile("nop");\
											}\
										}


#endif // E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_DELAY_H_