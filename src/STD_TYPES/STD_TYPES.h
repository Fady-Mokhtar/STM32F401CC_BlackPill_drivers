
/* ******************************************************************************
 *
 * Module: Common - Platform Types Abstraction
 *
 * File Name: STD_TYPES.h
 *
 * Description: Types for STM32F401CC blackpill
 * 
 * Author: Fady
 * 
 *******************************************************************************/




#ifndef _STD_TYPES_H
#define _STD_TYPES_H

/*
typedef enum 
{
	FALSE,
	TRUE
}bool_t;
*/

/* Boolean Data Type */
typedef unsigned char bool_t;

/* Boolean Values */
#ifndef FALSE
#define FALSE       (0u)
#endif
#ifndef TRUE
#define TRUE        (1u)
#endif

#define HIGH        (1u)
#define LOW         (0u)

/* NULL Value */
#define NULL_t	(void*)0

typedef unsigned char 			u8;				/*       0 .. 255             		*/
typedef unsigned short int 		u16;			/*       0 .. 65535           		*/
typedef unsigned long int  		u32;			/*       0 .. 4294967295      		*/
typedef unsigned long long    	u64;			/*       0 .. 18446744073709551615  */

typedef signed char 			s8;				/*        -128 .. +127            	*/
typedef signed short int 		s16;			/*      -32768 .. +32767         	*/
typedef signed long int  		s32;			/*	 -2147483648 .. +2147483647     */
typedef signed long long      	s64;

typedef float  f32;
typedef double f64;



#endif /* STD_TYPES_H_ */
