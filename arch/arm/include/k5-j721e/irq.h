/****************************************************************************
 * arch/arm/include/k5-j721e/irq.h
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

/* This file should never be included directly but, rather,
 * only indirectly through nuttx/irq.h
 */

#ifndef __ARCH_ARM_INCLUDE_K5_J721E_IRQ_H
#define __ARCH_ARM_INCLUDE_K5_J721E_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <arch/k5-j721e/chip.h>

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

/* The interrupt vector table only has 96 entries, one phantom vector and 95
 * interrupt channels.
 * Channel 95 does not have a dedicated vector and shall not be used.
 */

#define K5_J721E_VECT_PHANTOM    0  /* The first is the "phantom" interrupt */

/* The interrupt vector table only has 96 entries, one phantom vector and 95
 * interrupt channels.
 * Channel 95 does not have a dedicated vector and shall not be used.
 */

#define K5_J721E_IRQ_NCHANNELS   95  /* The "phantom" vector is followed by 95 real * interrupt channels */

/* Default request to channel assignments.  Undefined requests are reserved */

#define K5_J721E_REQ_ESMHIGH     0  /* ESM High level interrupt (NMI) */
#define K5_J721E_REQ_RTICMP0     2  /* RTI compare interrupt 0 */
#define K5_J721E_REQ_RTICMP1     3  /* RTI compare interrupt 1 */
#define K5_J721E_REQ_RTICMP2     4  /* RTI compare interrupt 2 */
#define K5_J721E_REQ_RTICMP3     5  /* RTI compare interrupt 3 */
#define K5_J721E_REQ_RTIOVF0     6  /* RTI overflow interrupt 0 */
#define K5_J721E_REQ_RTIOVF1     7  /* RTI overflow interrupt 1 */
#define K5_J721E_REQ_GIO_0       9  /* GIO level 0 interrupt */
#define K5_J721E_REQ_N2HET_0     10 /* N2HET level 0 interrupt */
#define K5_J721E_REQ_HTU_0       11 /* HTU level 0 interrupt */
#define K5_J721E_REQ_MIBSPI1_0   12 /* MIBSPI1 level 0 interrupt */
#define K5_J721E_REQ_SCI1_0      13 /* SCI1/LIN1 level 0 interrupt */
#define K5_J721E_REQ_MIBADCEV    14 /* MIBADC event group interrupt */
#define K5_J721E_REQ_MIBADSW1    15 /* MIBADC sw group 1 interrupt */
#define K5_J721E_REQ_DCAN1_0     16 /* DCAN1 level 0 interrupt */
#define K5_J721E_REQ_SPI2_0      17 /* SPI2 level 0 interrupt */
#define K5_J721E_REQ_ESMLO       20 /* ESM Low level interrupt */
#define K5_J721E_REQ_SYSTEM      21 /* Software interrupt (SSI) */
#define K5_J721E_REQ_CPU         22 /* PMU interrupt */
#define K5_J721E_REQ_GIO_1       23 /* GIO level 1 interrupt */
#define K5_J721E_REQ_N2HET_1     24 /* N2HET level 1 interrupt */
#define K5_J721E_REQ_HTU_1       25 /* HTU level 1 interrupt */
#define K5_J721E_REQ_MIBSPI1_1   26 /* MIBSPI1 level 1 interrupt */
#define K5_J721E_REQ_SCI1_1      27 /* SCI1/LIN1 level 1 interrupt */
#define K5_J721E_REQ_MIBADCSW2   28 /* MIBADC sw group 2 interrupt */
#define K5_J721E_REQ_DCAN1_1     29 /* DCAN1 level 1 interrupt */
#define K5_J721E_REQ_SPI2_1      30 /* SPI2 level 1 interrupt */
#define K5_J721E_REQ_MIBADCMC    31 /* MIBADC magnitude compare interrupt */
#define K5_J721E_REQ_DCAN2_0     35 /* DCAN2 level 0 interrupt */
#define K5_J721E_REQ_SPI3_0      37 /* SPI3 level 0 interrupt */
#define K5_J721E_REQ_SPI3_1      38 /* SPI3 level 1 interrupt */
#define K5_J721E_REQ_DCAN2_1     42 /* DCAN2 level 1 interrupt */
#define K5_J721E_REQ_FMC         61 /* FSM_DONE interrupt */
#define K5_J721E_REQ_HWAGH       80 /* WA_INT_REQ_H */
#define K5_J721E_REQ_DCC         82 /* DCC done interrupt */
#define K5_J721E_REQ_EQEP        84 /* eQEP Interrupt */
#define K5_J721E_REQ_PBIST       85 /* PBIST Done Interrupt */
#define K5_J721E_REQ_HWAGL       88 /* HWA_INT_REQ_L */

#ifdef CONFIG_K5_J721E_GIO_IRQ
#  define K5_J721E_IRQ_GIOA0     (K5_J721E_IRQ_NCHANNELS+1)
#  define K5_J721E_IRQ_GIOA1     (K5_J721E_IRQ_NCHANNELS+2)
#  define K5_J721E_IRQ_GIOA2     (K5_J721E_IRQ_NCHANNELS+3)
#  define K5_J721E_IRQ_GIOA3     (K5_J721E_IRQ_NCHANNELS+4)
#  define K5_J721E_IRQ_GIOA4     (K5_J721E_IRQ_NCHANNELS+5)
#  define K5_J721E_IRQ_GIOA5     (K5_J721E_IRQ_NCHANNELS+6)
#  define K5_J721E_IRQ_GIOA6     (K5_J721E_IRQ_NCHANNELS+7)
#  define K5_J721E_IRQ_GIOA7     (K5_J721E_IRQ_NCHANNELS+8)

#  define K5_J721E_IRQ_GIOB0     (K5_J721E_IRQ_NCHANNELS+9)
#  define K5_J721E_IRQ_GIOB1     (K5_J721E_IRQ_NCHANNELS+10)
#  define K5_J721E_IRQ_GIOB2     (K5_J721E_IRQ_NCHANNELS+11)
#  define K5_J721E_IRQ_GIOB3     (K5_J721E_IRQ_NCHANNELS+12)
#  define K5_J721E_IRQ_GIOB4     (K5_J721E_IRQ_NCHANNELS+13)
#  define K5_J721E_IRQ_GIOB5     (K5_J721E_IRQ_NCHANNELS+14)
#  define K5_J721E_IRQ_GIOB6     (K5_J721E_IRQ_NCHANNELS+15)
#  define K5_J721E_IRQ_GIOB7     (K5_J721E_IRQ_NCHANNELS+16)

#  define K5_J721E_IRQ_GIOC0     (K5_J721E_IRQ_NCHANNELS+17)
#  define K5_J721E_IRQ_GIOC1     (K5_J721E_IRQ_NCHANNELS+18)
#  define K5_J721E_IRQ_GIOC2     (K5_J721E_IRQ_NCHANNELS+19)
#  define K5_J721E_IRQ_GIOC3     (K5_J721E_IRQ_NCHANNELS+20)
#  define K5_J721E_IRQ_GIOC4     (K5_J721E_IRQ_NCHANNELS+21)
#  define K5_J721E_IRQ_GIOC5     (K5_J721E_IRQ_NCHANNELS+22)
#  define K5_J721E_IRQ_GIOC6     (K5_J721E_IRQ_NCHANNELS+23)
#  define K5_J721E_IRQ_GIOC7     (K5_J721E_IRQ_NCHANNELS+24)

#  define K5_J721E_IRQ_GIOD0     (K5_J721E_IRQ_NCHANNELS+25)
#  define K5_J721E_IRQ_GIOD1     (K5_J721E_IRQ_NCHANNELS+26)
#  define K5_J721E_IRQ_GIOD2     (K5_J721E_IRQ_NCHANNELS+27)
#  define K5_J721E_IRQ_GIOD3     (K5_J721E_IRQ_NCHANNELS+28)
#  define K5_J721E_IRQ_GIOD4     (K5_J721E_IRQ_NCHANNELS+29)
#  define K5_J721E_IRQ_GIOD5     (K5_J721E_IRQ_NCHANNELS+30)
#  define K5_J721E_IRQ_GIOD6     (K5_J721E_IRQ_NCHANNELS+31)
#  define K5_J721E_IRQ_GIOD7     (K5_J721E_IRQ_NCHANNELS+32)

#  define K5_J721E_NGIO_IRQS     32
#else
#  define K5_J721E_NGIO_IRQS     0
#endif

/* Total number of IRQ numbers.
 * Includes all channels plus GIO second-level interrupts (if enabled).
 *Excluds the phantom vector.  Zero corresponds to channel 0, vector 1.
 */

#define NR_IRQS  (K5_J721E_IRQ_NCHANNELS + K5_J721E_NGIO_IRQS)

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_ARM_INCLUDE_K5_J721E_IRQ_H */
