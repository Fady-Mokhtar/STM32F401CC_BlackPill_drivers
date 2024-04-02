#ifndef MCAL_UART_UART_REG_H_
#define MCAL_UART_UART_REG_H_



#include "STD_TYPES.h"

# define __IO       volatile


/*uasrat REGs.h*/
/**********************
 * CR1
 * bit13	: UE	uart enable
 * bit12	: M		Word length
 * bit10	: PCE	parity control enable
 * bit9		: PS	parity selection
 * bit3		: TE	transimitter enable
 * bit2		: RE	Reciver enable
 * 
*/
typedef struct
{
  __IO uint32_t UART_SR;         /*!< UART Status register,                   Address offset: 0x00 */
  __IO uint32_t UART_DR;         /*!< UART Data register,                     Address offset: 0x04 */
  __IO uint32_t UART_BRR;        /*!< UART Baud rate register,                Address offset: 0x08 */
  __IO uint32_t UART_CR1;        /*!< UART Control register 1,                Address offset: 0x0C */
  __IO uint32_t UART_CR2;        /*!< UART Control register 2,                Address offset: 0x10 */
  __IO uint32_t UART_CR3;        /*!< UART Control register 3,                Address offset: 0x14 */
  __IO uint32_t UART_GTPR;       /*!< UART Guard time and prescaler register, Address offset: 0x18 */

} UART_RegStruct;
/********/

#define UART1_BASE_ADDR     (0x40011000UL)
#define UART2_BASE_ADDR     (0x40004400UL)
#define UART6_BASE_ADDR     (0x40011400UL)

#define UART1    ((volatile UART_RegStruct* const)(UART1_BASE_ADDR))
#define UART2    ((volatile UART_RegStruct* const)(UART2_BASE_ADDR))
#define UART6    ((volatile UART_RegStruct* const)(UART6_BASE_ADDR))




#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region                                */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)

/*!< APB1 peripherals */
#define UART2_BASE           (APB1PERIPH_BASE + 0x4400UL)

/*!< APB2 peripherals */
#define UART1_BASE           (APB2PERIPH_BASE + 0x1000UL)
#define UART6_BASE           (APB2PERIPH_BASE + 0x1400UL)



/******************************************************************************/
/*                                                                            */
/*         Universal Synchronous Asynchronous Receiver Transmitter            */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for UART_SR register  *******************/

#define UART_SR_PE_Pos               (0U)
#define UART_SR_PE_Msk               (0x1UL << UART_SR_PE_Pos)                /*!< 0x00000001 */
#define UART_SR_PE                   UART_SR_PE_Msk                          /*!<Parity Error                 */
#define UART_SR_FE_Pos               (1U)
#define UART_SR_FE_Msk               (0x1UL << UART_SR_FE_Pos)                /*!< 0x00000002 */
#define UART_SR_FE                   UART_SR_FE_Msk                          /*!<Framing Error                */
#define UART_SR_NE_Pos               (2U)
#define UART_SR_NE_Msk               (0x1UL << UART_SR_NE_Pos)                /*!< 0x00000004 */
#define UART_SR_NE                   UART_SR_NE_Msk                          /*!<Noise Error Flag             */
#define UART_SR_ORE_Pos              (3U)
#define UART_SR_ORE_Msk              (0x1UL << UART_SR_ORE_Pos)               /*!< 0x00000008 */
#define UART_SR_ORE                  UART_SR_ORE_Msk                         /*!<OverRun Error                */
#define UART_SR_IDLE_Pos             (4U)
#define UART_SR_IDLE_Msk             (0x1UL << UART_SR_IDLE_Pos)              /*!< 0x00000010 */
#define UART_SR_IDLE                 UART_SR_IDLE_Msk                        /*!<IDLE line detected           */
#define UART_SR_RXNE_Pos             (5U)
#define UART_SR_RXNE_Msk             (0x1UL << UART_SR_RXNE_Pos)              /*!< 0x00000020 */
#define UART_SR_RXNE                 UART_SR_RXNE_Msk                        /*!<Read Data Register Not Empty */
#define UART_SR_TC_Pos               (6U)
#define UART_SR_TC_Msk               (0x1UL << UART_SR_TC_Pos)                /*!< 0x00000040 */
#define UART_SR_TC                   UART_SR_TC_Msk                          /*!<Transmission Complete        */
#define UART_SR_TXE_Pos              (7U)
#define UART_SR_TXE_Msk              (0x1UL << UART_SR_TXE_Pos)               /*!< 0x00000080 */
#define UART_SR_TXE                  UART_SR_TXE_Msk                         /*!<Transmit Data Register Empty */
#define UART_SR_LBD_Pos              (8U)
#define UART_SR_LBD_Msk              (0x1UL << UART_SR_LBD_Pos)               /*!< 0x00000100 */
#define UART_SR_LBD                  UART_SR_LBD_Msk                         /*!<LIN Break Detection Flag     */
#define UART_SR_CTS_Pos              (9U)
#define UART_SR_CTS_Msk              (0x1UL << UART_SR_CTS_Pos)               /*!< 0x00000200 */
#define UART_SR_CTS                  UART_SR_CTS_Msk                         /*!<CTS Flag                     */


/*******************  Bit definition for UART_DR register  *******************/

#define UART_DR_DR						0x000001FF							/*!<Data value */

/******************  Bit definition for UART_BRR register  *******************/
#define UART_BRR_DIV_Fraction_Pos    (0U)
#define UART_BRR_DIV_Fraction_Msk    (0xFUL << UART_BRR_DIV_Fraction_Pos)     /*!< 0x0000000F */
#define UART_BRR_DIV_Fraction        UART_BRR_DIV_Fraction_Msk               /*!<Fraction of UARTDIV */
#define UART_BRR_DIV_Mantissa_Pos    (4U)
#define UART_BRR_DIV_Mantissa_Msk    (0xFFFUL << UART_BRR_DIV_Mantissa_Pos)   /*!< 0x0000FFF0 */
#define UART_BRR_DIV_Mantissa        UART_BRR_DIV_Mantissa_Msk               /*!<Mantissa of UARTDIV */

/******************  Bit definition for UART_CR1 register  *******************/
#define UART_CR1_SBK_Pos             (0U)
#define UART_CR1_SBK_Msk             (0x1UL << UART_CR1_SBK_Pos)              /*!< 0x00000001 */
#define UART_CR1_SBK                 UART_CR1_SBK_Msk                        /*!<Send Break                             */
#define UART_CR1_RWU_Pos             (1U)
#define UART_CR1_RWU_Msk             (0x1UL << UART_CR1_RWU_Pos)              /*!< 0x00000002 */
#define UART_CR1_RWU                 UART_CR1_RWU_Msk                        /*!<Receiver wakeup                        */
#define UART_CR1_RE_Pos              (2U)
#define UART_CR1_RE_clrMsk           (0x1UL << UART_CR1_RE_Pos)               /*!< 0x00000004 */
#define UART_CR1_RE_Msk              (0x1UL << UART_CR1_RE_Pos)               /*!< 0x00000004 */
#define UART_CR1_RE                  UART_CR1_RE_Msk                         /*!<Receiver Enable                        */
#define UART_CR1_TE_Pos              (3U)
#define UART_CR1_TE_clrMsk              (0x1UL << UART_CR1_TE_Pos)               /*!< 0x00000008 */
#define UART_CR1_TE_Msk              (0x1UL << UART_CR1_TE_Pos)               /*!< 0x00000008 */
#define UART_CR1_TE                  UART_CR1_TE_Msk                         /*!<Transmitter Enable                     */
#define UART_CR1_IDLEIE_Pos          (4U)
#define UART_CR1_IDLEIE_clrMsk          (0x1UL << UART_CR1_IDLEIE_Pos)           /*!< 0x00000010 */
#define UART_CR1_IDLEIE_Msk          (0x1UL << UART_CR1_IDLEIE_Pos)           /*!< 0x00000010 */
#define UART_CR1_IDLEIE              UART_CR1_IDLEIE_Msk                     /*!<IDLE Interrupt Enable                  */
#define UART_CR1_RXNEIE_Pos          (5U)
#define UART_CR1_RXNEIE_clrMsk          (0x1UL << UART_CR1_RXNEIE_Pos)           /*!< 0x00000020 */
#define UART_CR1_RXNEIE_Msk          (0x1UL << UART_CR1_RXNEIE_Pos)           /*!< 0x00000020 */
#define UART_CR1_RXNEIE              UART_CR1_RXNEIE_Msk                     /*!<RXNE Interrupt Enable                  */
#define UART_CR1_TCIE_Pos            (6U)
#define UART_CR1_TCIE_clrMsk         (0x1UL << UART_CR1_TCIE_Pos)             /*!< 0x00000040 */
#define UART_CR1_TCIE_Msk            (0x1UL << UART_CR1_TCIE_Pos)             /*!< 0x00000040 */
#define UART_CR1_TCIE                UART_CR1_TCIE_Msk                       /*!<Transmission Complete Interrupt Enable */
#define UART_CR1_TXEIE_Pos           (7U)
#define UART_CR1_TXEIE_clrMsk        (0x1UL << UART_CR1_TXEIE_Pos)            /*!< 0x00000080 */
#define UART_CR1_TXEIE_Msk           (0x1UL << UART_CR1_TXEIE_Pos)            /*!< 0x00000080 */
#define UART_CR1_TXEIE               UART_CR1_TXEIE_Msk                      /*!<TXE Interrupt Enable                   */
#define UART_CR1_PEIE_Pos            (8U)
#define UART_CR1_PEIE_Msk            (0x1UL << UART_CR1_PEIE_Pos)             /*!< 0x00000100 */
#define UART_CR1_PEIE_clrMsk         UART_CR1_PEIE_Msk                       /*!<PE Interrupt Enable                    */
#define UART_CR1_PEIE                UART_CR1_PEIE_Msk                       /*!<PE Interrupt Enable                    */
#define UART_CR1_PS_Pos              (9U)
#define UART_CR1_PS_clrMsk           (0x1UL << UART_CR1_PS_Pos)               /*!< 0x00000200 */
#define UART_CR1_PS_Msk              (0x1UL << UART_CR1_PS_Pos)               /*!< 0x00000200 */
#define UART_CR1_PS                  UART_CR1_PS_Msk                         /*!<Parity Selection                       */
#define UART_CR1_PCE_Pos             (10U)
#define UART_CR1_PCE_clrMsk          (0x1UL << UART_CR1_PCE_Pos)              /*!< 0x00000400 */
#define UART_CR1_PCE_Msk             (0x1UL << UART_CR1_PCE_Pos)              /*!< 0x00000400 */
#define UART_CR1_PCE                 UART_CR1_PCE_Msk                        /*!<Parity Control Enable                  */
#define UART_CR1_WAKE_Pos            (11U)
#define UART_CR1_WAKE_Msk            (0x1UL << UART_CR1_WAKE_Pos)             /*!< 0x00000800 */
#define UART_CR1_WAKE                UART_CR1_WAKE_Msk                       /*!<Wakeup method                          */
#define UART_CR1_M_Pos               (12U)
#define UART_CR1_M_clrMsk            (0x1UL << UART_CR1_M_Pos)                /*!< 0x00001000 */
#define UART_CR1_M_Msk               (0x1UL << UART_CR1_M_Pos)                /*!< 0x00001000 */
#define UART_CR1_M                   UART_CR1_M_Msk                          /*!<Word length                            */
#define UART_CR1_UE_Pos              (13U)
#define UART_CR1_UE_clrMsk           (0x1UL << UART_CR1_UE_Pos)               /*!< 0x00002000 */
#define UART_CR1_UE_Msk              (0x1UL << UART_CR1_UE_Pos)               /*!< 0x00002000 */
#define UART_CR1_UE                  UART_CR1_UE_Msk                         /*!<UART Enable                           */
#define UART_CR1_OVER8_Pos           (15U)
#define UART_CR1_OVER8_Msk           (0x1UL << UART_CR1_OVER8_Pos)            /*!< 0x00008000 */
#define UART_CR1_OVER8               UART_CR1_OVER8_Msk                      /*!<UART Oversampling by 8 enable         */
#define UART_CR1_OVER16_Pos          (15U)
#define UART_CR1_OVER16_Msk          (0x1UL << UART_CR1_OVER16_Pos)            
#define UART_CR1_OVER16              UART_CR1_OVER16_Msk 

/******************  Bit definition for UART_CR2 register  *******************/
#define UART_CR2_ADD_Pos             (0U)
#define UART_CR2_ADD_Msk             (0xFUL << UART_CR2_ADD_Pos)              /*!< 0x0000000F */
#define UART_CR2_ADD                 UART_CR2_ADD_Msk                        /*!<Address of the UART node            */
#define UART_CR2_LBDL_Pos            (5U)
#define UART_CR2_LBDL_Msk            (0x1UL << UART_CR2_LBDL_Pos)             /*!< 0x00000020 */
#define UART_CR2_LBDL                UART_CR2_LBDL_Msk                       /*!<LIN Break Detection Length           */
#define UART_CR2_LBDIE_Pos           (6U)
#define UART_CR2_LBDIE_Msk           (0x1UL << UART_CR2_LBDIE_Pos)            /*!< 0x00000040 */
#define UART_CR2_LBDIE               UART_CR2_LBDIE_Msk                      /*!<LIN Break Detection Interrupt Enable */
#define UART_CR2_LBCL_Pos            (8U)
#define UART_CR2_LBCL_Msk            (0x1UL << UART_CR2_LBCL_Pos)             /*!< 0x00000100 */
#define UART_CR2_LBCL                UART_CR2_LBCL_Msk                       /*!<Last Bit Clock pulse                 */
#define UART_CR2_CPHA_Pos            (9U)
#define UART_CR2_CPHA_Msk            (0x1UL << UART_CR2_CPHA_Pos)             /*!< 0x00000200 */
#define UART_CR2_CPHA                UART_CR2_CPHA_Msk                       /*!<Clock Phase                          */
#define UART_CR2_CPOL_Pos            (10U)
#define UART_CR2_CPOL_Msk            (0x1UL << UART_CR2_CPOL_Pos)             /*!< 0x00000400 */
#define UART_CR2_CPOL                UART_CR2_CPOL_Msk                       /*!<Clock Polarity                       */
#define UART_CR2_CLKEN_Pos           (11U)
#define UART_CR2_CLKEN_Msk           (0x1UL << UART_CR2_CLKEN_Pos)            /*!< 0x00000800 */
#define UART_CR2_CLKEN               UART_CR2_CLKEN_Msk                      /*!<Clock Enable                         */

#define UART_CR2_STOP_Pos            (12U)
#define UART_CR2_STOP_Msk            (0x3UL << UART_CR2_STOP_Pos)             /*!< 0x00003000 */
#define UART_CR2_STOP_clrMsk         UART_CR2_STOP_Msk                       /*!<STOP[1:0] bits (STOP bits) */
#define UART_CR2_STOP                UART_CR2_STOP_Msk                       /*!<STOP[1:0] bits (STOP bits) */
#define UART_CR2_STOP_0              (0x1UL << UART_CR2_STOP_Pos)             /*!< 0x1000 */
#define UART_CR2_STOP_1              (0x2UL << UART_CR2_STOP_Pos)             /*!< 0x2000 */

#define UART_CR2_LINEN_Pos           (14U)
#define UART_CR2_LINEN_Msk           (0x1UL << UART_CR2_LINEN_Pos)            /*!< 0x00004000 */
#define UART_CR2_LINEN               UART_CR2_LINEN_Msk                      /*!<LIN mode enable */

/******************  Bit definition for UART_CR3 register  *******************/
#define UART_CR3_EIE_Pos             (0U)
#define UART_CR3_EIE_Msk             (0x1UL << UART_CR3_EIE_Pos)              /*!< 0x00000001 */
#define UART_CR3_EIE_clrMsk          UART_CR3_EIE_Msk                        /*!<Error Interrupt Enable      */
#define UART_CR3_EIE                 UART_CR3_EIE_Msk                        /*!<Error Interrupt Enable      */
#define UART_CR3_IREN_Pos            (1U)
#define UART_CR3_IREN_Msk            (0x1UL << UART_CR3_IREN_Pos)             /*!< 0x00000002 */
#define UART_CR3_IREN                UART_CR3_IREN_Msk                       /*!<IrDA mode Enable            */
#define UART_CR3_IRLP_Pos            (2U)
#define UART_CR3_IRLP_Msk            (0x1UL << UART_CR3_IRLP_Pos)             /*!< 0x00000004 */
#define UART_CR3_IRLP                UART_CR3_IRLP_Msk                       /*!<IrDA Low-Power              */
#define UART_CR3_HDSEL_Pos           (3U)
#define UART_CR3_HDSEL_Msk           (0x1UL << UART_CR3_HDSEL_Pos)            /*!< 0x00000008 */
#define UART_CR3_HDSEL               UART_CR3_HDSEL_Msk                      /*!<Half-Duplex Selection       */
#define UART_CR3_NACK_Pos            (4U)
#define UART_CR3_NACK_Msk            (0x1UL << UART_CR3_NACK_Pos)             /*!< 0x00000010 */
#define UART_CR3_NACK                UART_CR3_NACK_Msk                       /*!<Smartcard NACK enable       */
#define UART_CR3_SCEN_Pos            (5U)
#define UART_CR3_SCEN_Msk            (0x1UL << UART_CR3_SCEN_Pos)             /*!< 0x00000020 */
#define UART_CR3_SCEN                UART_CR3_SCEN_Msk                       /*!<Smartcard mode enable       */
#define UART_CR3_DMAR_Pos            (6U)
#define UART_CR3_DMAR_Msk            (0x1UL << UART_CR3_DMAR_Pos)             /*!< 0x00000040 */
#define UART_CR3_DMAR                UART_CR3_DMAR_Msk                       /*!<DMA Enable Receiver         */
#define UART_CR3_DMAT_Pos            (7U)
#define UART_CR3_DMAT_Msk            (0x1UL << UART_CR3_DMAT_Pos)             /*!< 0x00000080 */
#define UART_CR3_DMAT                UART_CR3_DMAT_Msk                       /*!<DMA Enable Transmitter      */
#define UART_CR3_RTSE_Pos            (8U)
#define UART_CR3_RTSE_Msk            (0x1UL << UART_CR3_RTSE_Pos)             /*!< 0x00000100 */
#define UART_CR3_RTSE                UART_CR3_RTSE_Msk                       /*!<RTS Enable                  */
#define UART_CR3_CTSE_Pos            (9U)
#define UART_CR3_CTSE_Msk            (0x1UL << UART_CR3_CTSE_Pos)             /*!< 0x00000200 */
#define UART_CR3_CTSE                UART_CR3_CTSE_Msk                       /*!<CTS Enable                  */
#define UART_CR3_CTSIE_Pos           (10U)
#define UART_CR3_CTSIE_Msk           (0x1UL << UART_CR3_CTSIE_Pos)            /*!< 0x00000400 */
#define UART_CR3_CTSIE               UART_CR3_CTSIE_Msk                      /*!<CTS Interrupt Enable        */
#define UART_CR3_ONEBIT_Pos          (11U)
#define UART_CR3_ONEBIT_Msk          (0x1UL << UART_CR3_ONEBIT_Pos)           /*!< 0x00000800 */
#define UART_CR3_ONEBIT              UART_CR3_ONEBIT_Msk                     /*!<UART One bit method enable */

/******************  Bit definition for UART_GTPR register  ******************/
#define UART_GTPR_PSC_Pos            (0U)
#define UART_GTPR_PSC_Msk            (0xFFUL << UART_GTPR_PSC_Pos)            /*!< 0x000000FF */
#define UART_GTPR_PSC                UART_GTPR_PSC_Msk                       /*!<PSC[7:0] bits (Prescaler value) */
#define UART_GTPR_PSC_0              (0x01UL << UART_GTPR_PSC_Pos)            /*!< 0x0001 */
#define UART_GTPR_PSC_1              (0x02UL << UART_GTPR_PSC_Pos)            /*!< 0x0002 */
#define UART_GTPR_PSC_2              (0x04UL << UART_GTPR_PSC_Pos)            /*!< 0x0004 */
#define UART_GTPR_PSC_3              (0x08UL << UART_GTPR_PSC_Pos)            /*!< 0x0008 */
#define UART_GTPR_PSC_4              (0x10UL << UART_GTPR_PSC_Pos)            /*!< 0x0010 */
#define UART_GTPR_PSC_5              (0x20UL << UART_GTPR_PSC_Pos)            /*!< 0x0020 */
#define UART_GTPR_PSC_6              (0x40UL << UART_GTPR_PSC_Pos)            /*!< 0x0040 */
#define UART_GTPR_PSC_7              (0x80UL << UART_GTPR_PSC_Pos)            /*!< 0x0080 */

#define UART_GTPR_GT_Pos             (8U)
#define UART_GTPR_GT_Msk             (0xFFUL << UART_GTPR_GT_Pos)             /*!< 0x0000FF00 */
#define UART_GTPR_GT                 UART_GTPR_GT_Msk                        /*!<Guard time value */

/**************************************************************************************************************************/

#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            ((uint32_t)((((uint64_t)(_PCLK_))*25U)/(4U*((uint64_t)(_BAUD_)))))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        ((((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U)\
                                                         + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            ((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))

#define UART_DIV_SAMPLING8(_PCLK_, _BAUD_)             ((uint32_t)((((uint64_t)(_PCLK_))*25U)/(2U*((uint64_t)(_BAUD_)))))
#define UART_DIVMANT_SAMPLING8(_PCLK_, _BAUD_)         (UART_DIV_SAMPLING8((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING8(_PCLK_, _BAUD_)         ((((UART_DIV_SAMPLING8((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING8((_PCLK_), (_BAUD_)) * 100U)) * 8U)\
                                                         + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + ((UART DIVFRAQ & 0xF8) << 1) + (UART DIVFRAQ & 0x07U) */
#define UART_BRR_SAMPLING8(_PCLK_, _BAUD_)             ((UART_DIVMANT_SAMPLING8((_PCLK_), (_BAUD_)) << 4U) + \
                                                        ((UART_DIVFRAQ_SAMPLING8((_PCLK_), (_BAUD_)) & 0xF8U) << 1U) + \
                                                        (UART_DIVFRAQ_SAMPLING8((_PCLK_), (_BAUD_)) & 0x07U))









#endif // MCAL_UART_UART_REG_H_