#ifndef MCAL_DMA_DMA_PRIVATE_H_
#define MCAL_DMA_DMA_PRIVATE_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "../../../lib/Errors.h"
#include "../../../lib/STD_TYPES.h"
#include "../../../lib/BIT_MATH.h"

/* Driver Libraries   */


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/*
#define DMA_1_BASE_ADDR            (0x40026000UL)
#define DMA_2_BASE_ADDR            (0x40026400UL)

#define DMA_1	    ((volatile DMA* const)(DMA_1_BASE_ADDR))
#define DMA_2       ((volatile DMA* const)(DMA_2_BASE_ADDR))
*/

#define DMA1_BASE             (0x40026000UL)            //(AHB1PERIPH_BASE + 0x6000UL)
#define DMA1_Stream0_BASE     (DMA1_BASE + 0x010UL)
#define DMA1_Stream1_BASE     (DMA1_BASE + 0x028UL)
#define DMA1_Stream2_BASE     (DMA1_BASE + 0x040UL)
#define DMA1_Stream3_BASE     (DMA1_BASE + 0x058UL)
#define DMA1_Stream4_BASE     (DMA1_BASE + 0x070UL)
#define DMA1_Stream5_BASE     (DMA1_BASE + 0x088UL)
#define DMA1_Stream6_BASE     (DMA1_BASE + 0x0A0UL)
#define DMA1_Stream7_BASE     (DMA1_BASE + 0x0B8UL)
#define DMA2_BASE             (0x40026400UL)            //(AHB1PERIPH_BASE + 0x6400UL)
#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010UL)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028UL)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040UL)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058UL)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070UL)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088UL)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0UL)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8UL)

#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
#define DMA1_Stream0        ((DMA_Stream_TypeDef *) DMA1_Stream0_BASE)
#define DMA1_Stream1        ((DMA_Stream_TypeDef *) DMA1_Stream1_BASE)
#define DMA1_Stream2        ((DMA_Stream_TypeDef *) DMA1_Stream2_BASE)
#define DMA1_Stream3        ((DMA_Stream_TypeDef *) DMA1_Stream3_BASE)
#define DMA1_Stream4        ((DMA_Stream_TypeDef *) DMA1_Stream4_BASE)
#define DMA1_Stream5        ((DMA_Stream_TypeDef *) DMA1_Stream5_BASE)
#define DMA1_Stream6        ((DMA_Stream_TypeDef *) DMA1_Stream6_BASE)
#define DMA1_Stream7        ((DMA_Stream_TypeDef *) DMA1_Stream7_BASE)

#define DMA2                ((DMA_TypeDef *) DMA2_BASE)
#define DMA2_Stream0        ((DMA_Stream_TypeDef *) DMA2_Stream0_BASE)
#define DMA2_Stream1        ((DMA_Stream_TypeDef *) DMA2_Stream1_BASE)
#define DMA2_Stream2        ((DMA_Stream_TypeDef *) DMA2_Stream2_BASE)
#define DMA2_Stream3        ((DMA_Stream_TypeDef *) DMA2_Stream3_BASE)
#define DMA2_Stream4        ((DMA_Stream_TypeDef *) DMA2_Stream4_BASE)
#define DMA2_Stream5        ((DMA_Stream_TypeDef *) DMA2_Stream5_BASE)
#define DMA2_Stream6        ((DMA_Stream_TypeDef *) DMA2_Stream6_BASE)
#define DMA2_Stream7        ((DMA_Stream_TypeDef *) DMA2_Stream7_BASE)

/* Private types -------------------------------------------------------------*/

// typedef struct
// {
//   volatile uint32_t ISR;   /*!< DMA interrupt status register */
//   volatile uint32_t Reserved0;
//   volatile uint32_t IFCR;  /*!< DMA interrupt flag clear register */
// } DMA_Base_Registers;

/**
  * @brief DMA Controller
  */

typedef struct
{
  volatile uint32_t CR;     /*!< DMA stream x configuration register      */
  volatile uint32_t NDTR;   /*!< DMA stream x number of data register     */
  volatile uint32_t PAR;    /*!< DMA stream x peripheral address register */
  volatile uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
  volatile uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
  volatile uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;

typedef struct
{
  volatile uint32_t LISR;   /*!< DMA low interrupt status register,      Address offset: 0x00 */
  volatile uint32_t HISR;   /*!< DMA high interrupt status register,     Address offset: 0x04 */
  volatile uint32_t LIFCR;  /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
  volatile uint32_t HIFCR;  /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
  volatile DMA_Stream_TypeDef Stream[8];
} DMA_TypeDef;







#endif // MCAL_DMA_DMA_PRIVATE_H_