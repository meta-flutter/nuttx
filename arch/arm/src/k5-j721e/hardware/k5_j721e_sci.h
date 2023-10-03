/****************************************************************************
 * arch/arm/src/k5_j721e/hardware/k5_j721e_sci.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/* References:
 * K5_J721ELS04x/03x 16/32-Bit RISC Flash Microcontroller,
 * Technical Reference Manual, Texas Instruments,
 * Literature Number: SPNU517A, September 2013
 */

#ifndef __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_SCI_H
#define __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_SCI_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/k5_j721e_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define K5_J721E_SCI_GCR0_OFFSET        0x0000 /* SCI Global Control Register 0 */
#define K5_J721E_SCI_GCR1_OFFSET        0x0004 /* SCI Global Control Register 1 */
#define K5_J721E_SCI_GCR2_OFFSET        0x0008 /* SCI Global Control Register 2 */
#define K5_J721E_SCI_SETINT_OFFSET      0x000c /* SCI Set Interrupt Register */
#define K5_J721E_SCI_CLEARINT_OFFSET    0x0010 /* SCI Clear Interrupt Register */
#define K5_J721E_SCI_SETINTLVL_OFFSET   0x0014 /* SCI Set Interrupt Level Register */
#define K5_J721E_SCI_CLEARINTLVL_OFFSET 0x0018 /* SCI Clear Interrupt Level Register */
#define K5_J721E_SCI_FLR_OFFSET         0x001c /* SCI Flags Register */
#define K5_J721E_SCI_INTVECT0_OFFSET    0x0020 /* SCI Interrupt Vector Offset 0 */
#define K5_J721E_SCI_INTVECT1_OFFSET    0x0024 /* SCI Interrupt Vector Offset 1 */
#define K5_J721E_SCI_FORMAT_OFFSET      0x0028 /* SCI Format Control Register */
#define K5_J721E_SCI_BRS_OFFSET         0x002c /* Baud Rate Selection Register */
#define K5_J721E_SCI_ED_OFFSET          0x0030 /* Receiver Emulation Data Buffer */
#define K5_J721E_SCI_RD_OFFSET          0x0034 /* Receiver Data Buffer */
#define K5_J721E_SCI_TD_OFFSET          0x0038 /* Transmit Data Buffer */
#define K5_J721E_SCI_PIO0_OFFSET        0x003c /* SCI Pin I/O Control Register 0 */
#define K5_J721E_SCI_PIO1_OFFSET        0x0040 /* SCI Pin I/O Control Register 1 */
#define K5_J721E_SCI_PIO2_OFFSET        0x0044 /* SCI Pin I/O Control Register 2 */
#define K5_J721E_SCI_PIO3_OFFSET        0x0048 /* SCI Pin I/O Control Register 3 */
#define K5_J721E_SCI_PIO4_OFFSET        0x004c /* SCI Pin I/O Control Register 4 */
#define K5_J721E_SCI_PIO5_OFFSET        0x0050 /* SCI Pin I/O Control Register 5 */
#define K5_J721E_SCI_PIO6_OFFSET        0x0054 /* SCI Pin I/O Control Register 6 */
#define K5_J721E_SCI_PIO7_OFFSET        0x0058 /* SCI Pin I/O Control Register 7 */
#define K5_J721E_SCI_PIO8_OFFSET        0x005c /* SCI Pin I/O Control Register 8 */
#define K5_J721E_LIN_COMPARE_OFFSET     0x0060 /* LIN Compare Register */
#define K5_J721E_LIN_RD0_OFFSET         0x0064 /* LIN Receive Buffer 0 Register */
#define K5_J721E_LIN_RD1_OFFSET         0x0068 /* LIN Receive Buffer 1 Register */
#define K5_J721E_LIN_MASK_OFFSET        0x006c /* LIN Mask Register */
#define K5_J721E_LIN_ID_OFFSET          0x0070 /* LIN Identification Register */
#define K5_J721E_LIN_TD0_OFFSET         0x0074 /* LIN Transmit Buffer 0 */
#define K5_J721E_LIN_TD1_OFFSET         0x0078 /* LIN Transmit Buffer 1 */
#define K5_J721E_SCI_MBRS_OFFSET        0x007c /* Maximum Baud Rate Selection Register */
#define K5_J721E_SCI_IODFTCTRL_OFFSET   0x0090 /* Input/Output Error Enable Register */

/* Friendler register aliases */

#define K5_J721E_SCI_FUN_OFFSET         K5_J721E_SCI_PIO0_OFFSET  /* Pin Function Register */
#define K5_J721E_SCI_DIR_OFFSET         K5_J721E_SCI_PIO1_OFFSET  /* Pin Direction Register */
#define K5_J721E_SCI_DIN_OFFSET         K5_J721E_SCI_PIO2_OFFSET  /* Pin Data In Register */
#define K5_J721E_SCI_DOUT_OFFSET        K5_J721E_SCI_PIO3_OFFSET  /* Pin Data Out Register */
#define K5_J721E_SCI_SET_OFFSET         K5_J721E_SCI_PIO4_OFFSET  /* Pin Data Set Register */
#define K5_J721E_SCI_CLR_OFFSET         K5_J721E_SCI_PIO5_OFFSET  /* Pin Data Clr Register */
#define K5_J721E_SCI_ODR_OFFSET         K5_J721E_SCI_PIO6_OFFSET  /* Pin Open Drain Output Enable Register */
#define K5_J721E_SCI_PD_OFFSET          K5_J721E_SCI_PIO7_OFFSET  /* Pin Pullup/Pulldown Disable Register */
#define K5_J721E_SCI_PSL_OFFSET         K5_J721E_SCI_PIO8_OFFSET  /* Pin Pullup/Pulldown Selection Register */

/* Register Addresses *******************************************************/

#define K5_J721E_SCI1_GCR0              (K5_J721E_SCI1_BASE+K5_J721E_SCI_GCR0_OFFSET)
#define K5_J721E_SCI1_GCR1              (K5_J721E_SCI1_BASE+K5_J721E_SCI_GCR1_OFFSET)
#define K5_J721E_SCI1_GCR2              (K5_J721E_SCI1_BASE+K5_J721E_SCI_GCR2_OFFSET)
#define K5_J721E_SCI1_SETINT            (K5_J721E_SCI1_BASE+K5_J721E_SCI_SETINT_OFFSET)
#define K5_J721E_SCI1_CLEARINT          (K5_J721E_SCI1_BASE+K5_J721E_SCI_CLEARINT_OFFSET)
#define K5_J721E_SCI1_SETINTLVL         (K5_J721E_SCI1_BASE+K5_J721E_SCI_SETINTLVL_OFFSET)
#define K5_J721E_SCI1_CLEARINTLVL       (K5_J721E_SCI1_BASE+K5_J721E_SCI_CLEARINTLVL_OFFSET)
#define K5_J721E_SCI1_FLR               (K5_J721E_SCI1_BASE+K5_J721E_SCI_FLR_OFFSET)
#define K5_J721E_SCI1_INTVECT0          (K5_J721E_SCI1_BASE+K5_J721E_SCI_INTVECT0_OFFSET)
#define K5_J721E_SCI1_INTVECT1          (K5_J721E_SCI1_BASE+K5_J721E_SCI_INTVECT1_OFFSET)
#define K5_J721E_SCI1_FORMAT            (K5_J721E_SCI1_BASE+K5_J721E_SCI_FORMAT_OFFSET)
#define K5_J721E_SCI1_BRS               (K5_J721E_SCI1_BASE+K5_J721E_SCI_BRS_OFFSET)
#define K5_J721E_SCI1_ED                (K5_J721E_SCI1_BASE+K5_J721E_SCI_ED_OFFSET)
#define K5_J721E_SCI1_RD                (K5_J721E_SCI1_BASE+K5_J721E_SCI_RD_OFFSET)
#define K5_J721E_SCI1_TD                (K5_J721E_SCI1_BASE+K5_J721E_SCI_TD_OFFSET)
#define K5_J721E_SCI1_PIO0              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO0_OFFSET)
#define K5_J721E_SCI1_PIO1              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO1_OFFSET)
#define K5_J721E_SCI1_PIO2              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO2_OFFSET)
#define K5_J721E_SCI1_PIO3              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO3_OFFSET)
#define K5_J721E_SCI1_PIO4              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO4_OFFSET)
#define K5_J721E_SCI1_PIO5              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO5_OFFSET)
#define K5_J721E_SCI1_PIO6              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO6_OFFSET)
#define K5_J721E_SCI1_PIO7              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO7_OFFSET)
#define K5_J721E_SCI1_PIO8              (K5_J721E_SCI1_BASE+K5_J721E_SCI_PIO8_OFFSET)
#define K5_J721E_LIN1_COMPARE           (K5_J721E_SCI1_BASE+K5_J721E_LIN_COMPARE_OFFSET)
#define K5_J721E_LIN1_RD0               (K5_J721E_SCI1_BASE+K5_J721E_LIN_RD0_OFFSET)
#define K5_J721E_LIN1_RD1               (K5_J721E_SCI1_BASE+K5_J721E_LIN_RD1_OFFSET)
#define K5_J721E_LIN1_MASK              (K5_J721E_SCI1_BASE+K5_J721E_LIN_MASK_OFFSET)
#define K5_J721E_LIN1_ID                (K5_J721E_SCI1_BASE+K5_J721E_LIN_ID_OFFSET)
#define K5_J721E_LIN1_TD0               (K5_J721E_SCI1_BASE+K5_J721E_LIN_TD0_OFFSET)
#define K5_J721E_LIN1_TD1               (K5_J721E_SCI1_BASE+K5_J721E_LIN_TD1_OFFSET)
#define K5_J721E_SCI1_MBRS              (K5_J721E_SCI1_BASE+K5_J721E_SCI_MBRS_OFFSET)
#define K5_J721E_SCI1_IODFTCTRL         (K5_J721E_SCI1_BASE+K5_J721E_SCI_IODFTCTRL_OFFSET)

#define K5_J721E_SCI1_FUN               K5_J721E_SCI_PIO0  /* Pin Function Register */
#define K5_J721E_SCI1_DIR               K5_J721E_SCI_PIO1  /* Pin Direction Register */
#define K5_J721E_SCI1_DIN               K5_J721E_SCI_PIO2  /* Pin Data In Register */
#define K5_J721E_SCI1_DOUT              K5_J721E_SCI_PIO3  /* Pin Data Out Register */
#define K5_J721E_SCI1_SET               K5_J721E_SCI_PIO4  /* Pin Data Set Register */
#define K5_J721E_SCI1_CLR               K5_J721E_SCI_PIO5  /* Pin Data Clr Register */
#define K5_J721E_SCI1_ODR               K5_J721E_SCI_PIO6  /* Pin Open Drain Output Enable Register */
#define K5_J721E_SCI1_PD                K5_J721E_SCI_PIO7  /* Pin Pullup/Pulldown Disable Register */
#define K5_J721E_SCI1_PSL               K5_J721E_SCI_PIO8  /* Pin Pullup/Pulldown Selection Register */

#if K5_J721E_NSCI > 1
#  define K5_J721E_SCI2_GCR0            (K5_J721E_SCI2_BASE+K5_J721E_SCI_GCR0_OFFSET)
#  define K5_J721E_SCI2_GCR1            (K5_J721E_SCI2_BASE+K5_J721E_SCI_GCR1_OFFSET)
#  define K5_J721E_SCI2_GCR2            (K5_J721E_SCI2_BASE+K5_J721E_SCI_GCR2_OFFSET)
#  define K5_J721E_SCI2_SETINT          (K5_J721E_SCI2_BASE+K5_J721E_SCI_SETINT_OFFSET)
#  define K5_J721E_SCI2_CLEARINT        (K5_J721E_SCI2_BASE+K5_J721E_SCI_CLEARINT_OFFSET)
#  define K5_J721E_SCI2_SETINTLVL       (K5_J721E_SCI2_BASE+K5_J721E_SCI_SETINTLVL_OFFSET)
#  define K5_J721E_SCI2_CLEARINTLVL     (K5_J721E_SCI2_BASE+K5_J721E_SCI_CLEARINTLVL_OFFSET)
#  define K5_J721E_SCI2_FLR             (K5_J721E_SCI2_BASE+K5_J721E_SCI_FLR_OFFSET)
#  define K5_J721E_SCI2_INTVECT0        (K5_J721E_SCI2_BASE+K5_J721E_SCI_INTVECT0_OFFSET)
#  define K5_J721E_SCI2_INTVECT1        (K5_J721E_SCI2_BASE+K5_J721E_SCI_INTVECT1_OFFSET)
#  define K5_J721E_SCI2_FORMAT          (K5_J721E_SCI2_BASE+K5_J721E_SCI_FORMAT_OFFSET)
#  define K5_J721E_SCI2_BRS             (K5_J721E_SCI2_BASE+K5_J721E_SCI_BRS_OFFSET)
#  define K5_J721E_SCI2_ED              (K5_J721E_SCI2_BASE+K5_J721E_SCI_ED_OFFSET)
#  define K5_J721E_SCI2_RD              (K5_J721E_SCI2_BASE+K5_J721E_SCI_RD_OFFSET)
#  define K5_J721E_SCI2_TD              (K5_J721E_SCI2_BASE+K5_J721E_SCI_TD_OFFSET)
#  define K5_J721E_SCI2_PIO0            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO0_OFFSET)
#  define K5_J721E_SCI2_PIO1            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO1_OFFSET)
#  define K5_J721E_SCI2_PIO2            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO2_OFFSET)
#  define K5_J721E_SCI2_PIO3            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO3_OFFSET)
#  define K5_J721E_SCI2_PIO4            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO4_OFFSET)
#  define K5_J721E_SCI2_PIO5            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO5_OFFSET)
#  define K5_J721E_SCI2_PIO6            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO6_OFFSET)
#  define K5_J721E_SCI2_PIO7            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO7_OFFSET)
#  define K5_J721E_SCI2_PIO8            (K5_J721E_SCI2_BASE+K5_J721E_SCI_PIO8_OFFSET)
#  define K5_J721E_LIN2_COMPARE         (K5_J721E_SCI2_BASE+K5_J721E_LIN_COMPARE_OFFSET)
#  define K5_J721E_LIN2_RD0             (K5_J721E_SCI2_BASE+K5_J721E_LIN_RD0_OFFSET)
#  define K5_J721E_LIN2_RD1             (K5_J721E_SCI2_BASE+K5_J721E_LIN_RD1_OFFSET)
#  define K5_J721E_LIN2_MASK            (K5_J721E_SCI2_BASE+K5_J721E_LIN_MASK_OFFSET)
#  define K5_J721E_LIN2_ID              (K5_J721E_SCI2_BASE+K5_J721E_LIN_ID_OFFSET)
#  define K5_J721E_LIN2_TD0             (K5_J721E_SCI2_BASE+K5_J721E_LIN_TD0_OFFSET)
#  define K5_J721E_LIN2_TD1             (K5_J721E_SCI2_BASE+K5_J721E_LIN_TD1_OFFSET)
#  define K5_J721E_SCI2_MBRS            (K5_J721E_SCI2_BASE+K5_J721E_SCI_MBRS_OFFSET)
#  define K5_J721E_SCI2_IODFTCTRL       (K5_J721E_SCI2_BASE+K5_J721E_SCI_IODFTCTRL_OFFSET)

#  define K5_J721E_SCI1_FUN             K5_J721E_SCI_PIO0  /* Pin Function Register */
#  define K5_J721E_SCI1_DIR             K5_J721E_SCI_PIO1  /* Pin Direction Register */
#  define K5_J721E_SCI1_DIN             K5_J721E_SCI_PIO2  /* Pin Data In Register */
#  define K5_J721E_SCI1_DOUT            K5_J721E_SCI_PIO3  /* Pin Data Out Register */
#  define K5_J721E_SCI1_SET             K5_J721E_SCI_PIO4  /* Pin Data Set Register */
#  define K5_J721E_SCI1_CLR             K5_J721E_SCI_PIO5  /* Pin Data Clr Register */
#  define K5_J721E_SCI1_ODR             K5_J721E_SCI_PIO6  /* Pin Open Drain Output Enable Register */
#  define K5_J721E_SCI1_PD              K5_J721E_SCI_PIO7  /* Pin Pullup/Pulldown Disable Register */
#  define K5_J721E_SCI1_PSL             K5_J721E_SCI_PIO8  /* Pin Pullup/Pulldown Selection Register */
#endif

/* Register Bit-Field Definitions *******************************************/

/* SCI Global Control Register 0 */

#define SCI_GCR0_RESET                (1 << 0)  /* Bit 0:  SCI/LIN module is out of reset */

/* SCI Global Control Register 1 */

#define SCI_GCR1_COMM                 (1 << 0)  /* Bit 0:  SCI/LIN communication mode */
#define SCI_GCR1_TIMING               (1 << 1)  /* Bit 1:  SCI timing mode */
#define SCI_GCR1_PARENA               (1 << 2)  /* Bit 2:  Parity enable */
#define SCI_GCR1_PARITY               (1 << 3)  /* Bit 3:  SCI parity odd/even selection */
#define SCI_GCR1_STOP                 (1 << 4)  /* Bit 4:  SCI number of stop bits per frame */
#define SCI_GCR1_CLOCK                (1 << 5)  /* Bit 5:  SCI internal clock enable */
#define SCI_GCR1_LIN                  (1 << 6)  /* Bit 6:  LIN mode */
#define SCI_GCR1_SWRST                (1 << 7)  /* Bit 7:  Software reset (active low) */
#define SCI_GCR1_SLEEP                (1 << 8)  /* Bit 8:  SCI sleep */
#define SCI_GCR1_ADAPT                (1 << 9)  /* Bit 9:  Adapt */
#define SCI_GCR1_MBUF                 (1 << 10) /* Bit 10: Multibuffer mode */
#define SCI_GCR1_CTYPE                (1 << 11) /* Bit 11:  Checksum type */
#define SCI_GCR1_HGEN                 (1 << 12) /* Bit 12: HGEN control */
#define SCI_GCR1_STOPEXT              (1 << 13) /* Bit 13: Stop extended frame communication */
#define SCI_GCR1_LOOPBACK             (1 << 16) /* Bit 16: Loopback bit */
#define SCI_GCR1_CONT                 (1 << 17) /* Bit 13: Continue on suspend */
#define SCI_GCR1_RXENA                (1 << 24) /* Bit 24: Receive enable */
#define SCI_GCR1_TXENA                (1 << 25) /* Bit 25: Transmit enable */

/* SCI Global Control Register 2 */

#define SCI_GCR2_POWERDOWN            (1 << 0)  /* Bit 0:  Power down */
#define SCI_GCR2_GENWU                (1 << 8)  /* Bit 8:  Generate wakeup signal */
#define SCI_GCR2_SC                   (1 << 16) /* Bit 16: Send checksum byte */
#define SCI_GCR2_CC                   (1 << 17) /* Bit 17: Compare checksum */

/* SCI Set Interrupt Register,
 * SCI Clear Interrupt Register,
 * SCI Set Interrupt Level Register, and
 * SCI Clear Interrupt Level Register
 */

#define SCI_INT_BRKDT                 (1 << 0)  /* Bit 0:  Break detect interrupt */
#define SCI_INT_WAKEUP                (1 << 1)  /* Bit 1:  Wake-up interrupt */
#define SCI_INT_TIMEOUT               (1 << 4)  /* Bit 4:  Timeout interrupt */
#define SCI_INT_TOAWUS                (1 << 6)  /* Bit 6:  Timeout after wakeup signal interrupt */
#define SCI_INT_TOA3WUS               (1 << 7)  /* Bit 7:  Timeout after 2 Wakeup signls interrupt */
#define SCI_INT_TX                    (1 << 8)  /* Bit 8:  Tranmitter interrupt */
#define SCI_INT_RX                    (1 << 9)  /* Bit 9:  Receiver interrupt */
#define SCI_INT_ID                    (1 << 13) /* Bit 13: Identification interrupt */
#define SCI_INT_PE                    (1 << 24) /* Bit 24: Parity error interrupt */
#define SCI_INT_OE                    (1 << 25) /* Bit 25: Overrun error interrupt */
#define SCI_INT_FE                    (1 << 26) /* Bit 26: Framing error interrupt */
#define SCI_INT_NRE                   (1 << 27) /* Bit 27: No response error interrupt */
#define SCI_INT_ISFE                  (1 << 28) /* Bit 28: Inconsistent synch field error interrupt */
#define SCI_INT_CE                    (1 << 29) /* Bit 29: Checksum error interrupt */
#define SCI_INT_PBE                   (1 << 30) /* Bit 30: Physical bus error interrupt */
#define SCI_INT_BE                    (1 << 31) /* Bit 31: Bit error interrupt */

#define SCI_INT_ALLERRORS             0xff000001
#define SCI_INT_LINERRORS             0xff000000
#define SCI_INT_SCIERRORS             0x87000001
#define SCI_INT_ALLINTS               0xff0023d3

/* SCI Flags Register */

#define SCI_FLR_BRKDT                 (1 << 0)  /* Bit 0:  Break detect flag */
#define SCI_FLR_WAKEUP                (1 << 1)  /* Bit 1:  Wake-up flag */
#define SCI_FLR_IDLE                  (1 << 2)  /* Bit 2:  SCI receiver in idle state */
#define SCI_FLR_BUSY                  (1 << 3)  /* Bit 3:  Bus busy flag */
#define SCI_FLR_TIMEOUT               (1 << 4)  /* Bit 4:  Timeout flag */
#define SCI_FLR_TOAWUS                (1 << 6)  /* Bit 6:  Timeout after wakeup signal flag */
#define SCI_FLR_TOA3WUS               (1 << 7)  /* Bit 7:  Timeout after 2 Wakeup signls flag */
#define SCI_FLR_TXRDY                 (1 << 8)  /* Bit 8:  Transmitter buffer register ready flag */
#define SCI_FLR_RXRDY                 (1 << 9)  /* Bit 9:  Receiver ready flag */
#define SCI_FLR_TXWAKE                (1 << 10) /* Bit 10: Transmitter wakeup method select */
#define SCI_FLR_TXEMPTY               (1 << 11) /* Bit 11: Transmitter empty flag */
#define SCI_FLR_RXWAKE                (1 << 12) /* Bit 12: Receiver wakeup detect flag */
#define SCI_FLR_IDTX                  (1 << 13) /* Bit 13: Identifier on transmit flag */
#define SCI_FLR_IDRX                  (1 << 14) /* Bit 14: Identifier on receive flag */
#define SCI_FLR_PE                    (1 << 24) /* Bit 24: Parity error flag */
#define SCI_FLR_OE                    (1 << 25) /* Bit 25: Overrun error flag */
#define SCI_FLR_FE                    (1 << 26) /* Bit 26: Framing error flag */
#define SCI_FLR_NRE                   (1 << 27) /* Bit 27: No response error flag */
#define SCI_FLR_ISFE                  (1 << 28) /* Bit 28: Inconsistent synch field error flag */
#define SCI_FLR_CE                    (1 << 29) /* Bit 29: checksum error flag */
#define SCI_FLR_PBE                   (1 << 30) /* Bit 30: Physical bus error flag */
#define SCI_FLR_BE                    (1 << 31) /* Bit 31: Bit error flag */

/* SCI Interrupt Vector Offset 0/1 */

#define SCI_INTVECT_MASK              (0x1f)    /* Bits 0-4: Interrupt vector offset */
#  define SCI_INTVECT_NONE            (0)       /*   No interrupt */
#  define SCI_INTVECT_WAKEUP          (1)       /*   Wake-up interrupt */
#  define SCI_INTVECT_ISFE            (2)       /*   Inconsistent synch field error interrupt */
#  define SCI_INTVECT_PE              (3)       /*   Parity error interrupt */
#  define SCI_INTVECT_ID              (4)       /*   Identification interrupt */
#  define SCI_INTVECT_PBE             (5)       /*   Physical bus error interrupt */
#  define SCI_INTVECT_FE              (6)       /*   Framing error interrupt */
#  define SCI_INTVECT_BRKDT           (7)       /*   Break detect interrupt */
#  define SCI_INTVECT_CE              (8)       /*   Checksum error interrupt */
#  define SCI_INTVECT_OE              (9)       /*   Overrun error interrupt */
#  define SCI_INTVECT_BE              (10)      /*   Bit error interrupt */
#  define SCI_INTVECT_RX              (11)      /*   Receive interrupt */
#  define SCI_INTVECT_TX              (12)      /*   Tranmit interrupt */
#  define SCI_INTVECT_NRE             (13)      /*   No response error interrupt */
#  define SCI_INTVECT_TOAWUS          (14)      /*   Timeout after wakeup signal interrupt */
#  define SCI_INTVECT_TOA3WUS         (15)      /*   Timeout after 2 Wakeup signls interrupt */
#  define SCI_INTVECT_TIMEOUT         (16)      /*   Timeout interrupt */

/* SCI Format Control Register */

#define SCI_FORMAT_CHAR_SHIFT         (0)       /* Bits 0-2:  Frame length control bits */
#define SCI_FORMAT_CHAR_MASK          (7 << SCI_FORMAT_CHAR_SHIFT)
#  define SCI_FORMAT_CHAR(n)          ((uint32_t)(n) << SCI_FORMAT_CHAR_SHIFT)
#define SCI_FORMAT_LENGTH_SHIFT       (16)      /* Bits 16-18:  Character length control bits */
#define SCI_FORMAT_LENGTH_MASK        (7 << SCI_FORMAT_LENGTH_SHIFT)
#  define SCI_FORMAT_LENGTH(n)        ((uint32_t)(n) << SCI_FORMAT_LENGTH_SHIFT)

/* Baud Rate Selection Register */

#define SCI_BRS_P_SHIFT               (0)       /* Bits 0-23: Baud integer divider */
#define SCI_BRS_P_MASK                (0x00ffffff << SCI_BRS_P_SHIFT)
#  define SCI_BRS_P(n)                ((uint32_t)(n) << SCI_BRS_P_SHIFT)
#define SCI_BRS_M_SHIFT               (24)      /* Bits 24-27: SCI/LIN 4-bit fractional divider selection */
#define SCI_BRS_M_MASK                (15 << SCI_BRS_M_SHIFT)
#  define SCI_BRS_M(n)                ((uint32_t)(n) << SCI_BRS_M_SHIFT)
#define SCI_BRS_U_SHIFT               (28)      /* Bits 28-30: SCI/LIN super fractional divider */
#define SCI_BRS_U_MASK                (7 << SCI_BRS_U_SHIFT)
#  define SCI_BRS_U(n)                ((uint32_t)(n) << SCI_BRS_U_SHIFT)

/* Receiver Emulation Data Buffer */
#define SCI_ED_

/* Receiver Data Buffer */
#define SCI_RD_

/* Transmit Data Buffer */
#define SCI_TD_

/* SCI Pin I/O Control Register 0: Pin Function Register,
 * SCI Pin I/O Control Register 1: Pin Direction Register,
 * SCI Pin I/O Control Register 2: Pin Data In Register,
 * SCI Pin I/O Control Register 3: Pin Data Out Register,
 * SCI Pin I/O Control Register 4: Pin Data Set Register,
 * SCI Pin I/O Control Register 5: Pin Data Clr Register,
 * SCI Pin I/O Control Register 6: Pin Open Drain Output Enable Register,
 * SCI Pin I/O Control Register 7: Pin Pullup/Pulldown Disable Register, and
 * SCI Pin I/O Control Register 8: Pin Pullup/Pulldown Selection Register.
 */

#define SCI_PIO_RX                    (1 << 1)  /* Bit 1: RX pin */
#define SCI_PIO_TX                    (1 << 2)  /* Bit 2: TX pin */

/* LIN Compare Register */
#define LIN_COMPARE_

/* LIN Receive Buffer 0 Register */
#define LIN_RD0_

/* LIN Receive Buffer 1 Register */
#define LIN_RD1_

/* LIN Mask Register */
#define LIN_MASK_

/* LIN Identification Register */
#define LIN_ID_

/* LIN Transmit Buffer 0 */
#define LIN_TD0_

/* LIN Transmit Buffer 1 */
#define LIN_TD1_

/* Maximum Baud Rate Selection Register */
#define SCI_MBRS_

/* Input/Output Error Enable Register */
#define SCI_IODFTCTRL_

#endif /* __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_SCI_H */
