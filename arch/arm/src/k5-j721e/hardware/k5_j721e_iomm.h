/****************************************************************************
 * arch/arm/src/k5_j721e/hardware/k5_j721e_iomm.h
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

#ifndef __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_IOMM_H
#define __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_IOMM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/k5_j721e_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define K5_J721E_IOMM_REVISION_OFFSET     0x0000 /* Revision Register */
#define K5_J721E_IOMM_BOOT_OFFSET         0x0020 /* Boot Mode Register */
#define K5_J721E_IOMM_KICK0_OFFSET        0x0038 /* Kicker Register 0 */
#define K5_J721E_IOMM_KICK1_OFFSET        0x003c /* Kicker Register 1 */
#define K5_J721E_IOMM_ERRRAWSTATUS_OFFSET 0x00e0 /* Error Raw Status / Set Register */
#define K5_J721E_IOMM_ERRSTATUS_OFFSET    0x00e4 /* Error Enabled Status / Clear Register */
#define K5_J721E_IOMM_ERRENABLE_OFFSET    0x00e8 /* Error Signaling Enable Register */
#define K5_J721E_IOMM_ERRENABLECLR_OFFSET 0x00ec /* Error Signaling Enable Clear Register */
#define K5_J721E_IOMM_FAULTADDRESS_OFFSET 0x00f4 /* Fault Address Register */
#define K5_J721E_IOMM_FAULTSTATUS_OFFSET  0x00f8 /* Fault Status Register */
#define K5_J721E_IOMM_FAULTCLR_OFFSET     0x00fc /* Fault Clear Register */

#define K5_J721E_IOMM_PINMMR_OFFSET(n)    (0x0110 + ((unsigned int)(n) << 2))
#  define K5_J721E_IOMM_PINMMR0_OFFSET    0x0110 /* Pin Multiplexing Control Register 0 */
#  define K5_J721E_IOMM_PINMMR1_OFFSET    0x0114 /* Pin Multiplexing Control Register 1 */
#  define K5_J721E_IOMM_PINMMR2_OFFSET    0x0118 /* Pin Multiplexing Control Register 2 */
#  define K5_J721E_IOMM_PINMMR3_OFFSET    0x011c /* Pin Multiplexing Control Register 3 */
#  define K5_J721E_IOMM_PINMMR4_OFFSET    0x0120 /* Pin Multiplexing Control Register 4 */
#  define K5_J721E_IOMM_PINMMR5_OFFSET    0x0124 /* Pin Multiplexing Control Register 5 */
#  define K5_J721E_IOMM_PINMMR6_OFFSET    0x0128 /* Pin Multiplexing Control Register 6 */
#  define K5_J721E_IOMM_PINMMR7_OFFSET    0x012c /* Pin Multiplexing Control Register 7 */
#  define K5_J721E_IOMM_PINMMR8_OFFSET    0x0130 /* Pin Multiplexing Control Register 8 */
#  define K5_J721E_IOMM_PINMMR9_OFFSET    0x0134 /* Pin Multiplexing Control Register 9 */
#  define K5_J721E_IOMM_PINMMR10_OFFSET   0x0138 /* Pin Multiplexing Control Register 10 */
#  define K5_J721E_IOMM_PINMMR11_OFFSET   0x013c /* Pin Multiplexing Control Register 11 */
#  define K5_J721E_IOMM_PINMMR12_OFFSET   0x0140 /* Pin Multiplexing Control Register 12 */
#  define K5_J721E_IOMM_PINMMR13_OFFSET   0x0144 /* Pin Multiplexing Control Register 13 */
#  define K5_J721E_IOMM_PINMMR14_OFFSET   0x0148 /* Pin Multiplexing Control Register 14 */
#  define K5_J721E_IOMM_PINMMR15_OFFSET   0x014c /* Pin Multiplexing Control Register 15 */
#  define K5_J721E_IOMM_PINMMR16_OFFSET   0x0150 /* Pin Multiplexing Control Register 16 */
#  define K5_J721E_IOMM_PINMMR17_OFFSET   0x0154 /* Pin Multiplexing Control Register 17 */
#  define K5_J721E_IOMM_PINMMR18_OFFSET   0x0158 /* Pin Multiplexing Control Register 18 */
#  define K5_J721E_IOMM_PINMMR19_OFFSET   0x015c /* Pin Multiplexing Control Register 19 */
#  define K5_J721E_IOMM_PINMMR20_OFFSET   0x0160 /* Pin Multiplexing Control Register 20 */
#  define K5_J721E_IOMM_PINMMR21_OFFSET   0x0164 /* Pin Multiplexing Control Register 21 */
#  define K5_J721E_IOMM_PINMMR22_OFFSET   0x0168 /* Pin Multiplexing Control Register 22 */
#  define K5_J721E_IOMM_PINMMR23_OFFSET   0x016c /* Pin Multiplexing Control Register 23 */
#  define K5_J721E_IOMM_PINMMR24_OFFSET   0x0170 /* Pin Multiplexing Control Register 24 */
#  define K5_J721E_IOMM_PINMMR25_OFFSET   0x0174 /* Pin Multiplexing Control Register 25 */
#  define K5_J721E_IOMM_PINMMR26_OFFSET   0x0178 /* Pin Multiplexing Control Register 26 */
#  define K5_J721E_IOMM_PINMMR27_OFFSET   0x017c /* Pin Multiplexing Control Register 27 */
#  define K5_J721E_IOMM_PINMMR28_OFFSET   0x0180 /* Pin Multiplexing Control Register 28 */
#  define K5_J721E_IOMM_PINMMR29_OFFSET   0x0184 /* Pin Multiplexing Control Register 29 */
#  define K5_J721E_IOMM_PINMMR30_OFFSET   0x0188 /* Pin Multiplexing Control Register 30 */

/* Register Addresses *******************************************************/

#define K5_J721E_IOMM_REVISION            (K5_J721E_IOMM_BASE+K5_J721E_IOMM_REVISION_OFFSET)
#define K5_J721E_IOMM_BOOT                (K5_J721E_IOMM_BASE+K5_J721E_IOMM_BOOT_OFFSET)
#define K5_J721E_IOMM_KICK0               (K5_J721E_IOMM_BASE+K5_J721E_IOMM_KICK0_OFFSET)
#define K5_J721E_IOMM_KICK1               (K5_J721E_IOMM_BASE+K5_J721E_IOMM_KICK1_OFFSET)
#define K5_J721E_IOMM_ERRRAWSTATUS        (K5_J721E_IOMM_BASE+K5_J721E_IOMM_ERRRAWSTATUS_OFFSET)
#define K5_J721E_IOMM_ERRSTATUS           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_ERRSTATUS_OFFSET)
#define K5_J721E_IOMM_ERRENABLE           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_ERRENABLE_OFFSET)
#define K5_J721E_IOMM_ERRENABLECLR        (K5_J721E_IOMM_BASE+K5_J721E_IOMM_ERRENABLECLR_OFFSET)
#define K5_J721E_IOMM_FAULTADDRESS        (K5_J721E_IOMM_BASE+K5_J721E_IOMM_FAULTADDRESS_OFFSET)
#define K5_J721E_IOMM_FAULTSTATUS         (K5_J721E_IOMM_BASE+K5_J721E_IOMM_FAULTSTATUS_OFFSET)
#define K5_J721E_IOMM_FAULTCLR            (K5_J721E_IOMM_BASE+K5_J721E_IOMM_FAULTCLR_OFFSET)

#define K5_J721E_IOMM_PINMMR(n)           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR_OFFSET(n))
#  define K5_J721E_IOMM_PINMMR0           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR0_OFFSET)
#  define K5_J721E_IOMM_PINMMR1           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR1_OFFSET)
#  define K5_J721E_IOMM_PINMMR2           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR2_OFFSET)
#  define K5_J721E_IOMM_PINMMR3           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR3_OFFSET)
#  define K5_J721E_IOMM_PINMMR4           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR4_OFFSET)
#  define K5_J721E_IOMM_PINMMR5           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR5_OFFSET)
#  define K5_J721E_IOMM_PINMMR6           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR6_OFFSET)
#  define K5_J721E_IOMM_PINMMR7           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR7_OFFSET)
#  define K5_J721E_IOMM_PINMMR8           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR8_OFFSET)
#  define K5_J721E_IOMM_PINMMR9           (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR9_OFFSET)
#  define K5_J721E_IOMM_PINMMR10          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR10_OFFSET)
#  define K5_J721E_IOMM_PINMMR11          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR11_OFFSET)
#  define K5_J721E_IOMM_PINMMR12          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR12_OFFSET)
#  define K5_J721E_IOMM_PINMMR13          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR13_OFFSET)
#  define K5_J721E_IOMM_PINMMR14          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR14_OFFSET)
#  define K5_J721E_IOMM_PINMMR15          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR15_OFFSET)
#  define K5_J721E_IOMM_PINMMR16          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR16_OFFSET)
#  define K5_J721E_IOMM_PINMMR17          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR17_OFFSET)
#  define K5_J721E_IOMM_PINMMR18          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR18_OFFSET)
#  define K5_J721E_IOMM_PINMMR19          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR19_OFFSET)
#  define K5_J721E_IOMM_PINMMR20          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR20_OFFSET)
#  define K5_J721E_IOMM_PINMMR21          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR21_OFFSET)
#  define K5_J721E_IOMM_PINMMR22          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR22_OFFSET)
#  define K5_J721E_IOMM_PINMMR23          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR23_OFFSET)
#  define K5_J721E_IOMM_PINMMR24          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR24_OFFSET)
#  define K5_J721E_IOMM_PINMMR25          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR25_OFFSET)
#  define K5_J721E_IOMM_PINMMR26          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR26_OFFSET)
#  define K5_J721E_IOMM_PINMMR27          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR27_OFFSET)
#  define K5_J721E_IOMM_PINMMR28          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR28_OFFSET)
#  define K5_J721E_IOMM_PINMMR29          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR29_OFFSET)
#  define K5_J721E_IOMM_PINMMR30          (K5_J721E_IOMM_BASE+K5_J721E_IOMM_PINMMR30_OFFSET)

/* Register Bit-Field Definitions *******************************************/

/* Revision Register */
#define IOMM_REVISION_

/* Boot Mode Register */
#define IOMM_BOOT_

/* Kicker Register 0 */

#define IOMM_KICK0_UNLOCK               0x83e70b13 /* Unlock value */
#define IOMM_KICK0_LOCK                 0x00000000 /* Any other value locks */

/* Kicker Register 1 */

#define IOMM_KICK1_UNLOCK               0x95a4f1e0 /* Unlock value */
#define IOMM_KICK1_LOCK                 0x00000000 /* Any other value locks */

/* Error Raw Status / Set Register */
#define IOMM_ERRRAWSTATUS_

/* Error Enabled Status / Clear Register */
#define IOMM_ERRSTATUS_

/* Error Signaling Enable Register */
#define IOMM_ERRENABLE_

/* Error Signaling Enable Clear Register */
#define IOMM_ERRENABLECLR_

/* Fault Address Register */
#define IOMM_FAULTADDRESS_

/* Fault Status Register */
#define IOMM_FAULTSTATUS_

/* Fault Clear Register */
#define IOMM_FAULTCLR_

/* Pin Multiplexing Control Register n, n=0..30.
 * Each 8-bit field controls the functionality of one pin/ball.
 * There are then a maximum of 31*4 = 124 pin/ball configurations supported.
 */

#define IOMM_PINMMR_REGNDX(n)           ((n) >> 2)
#define IOMM_PINMMR_PINSHIFT(n)         (((n) & 3) << 3)
#define IOMM_PINMMR_PINMASK(n)          (0xff << IOMM_PINMMR_PINSHIFT(n))
#  define IOMM_PINMMR_PINVALUE(n,v)     ((uint32_t)(v) << IOMM_PINMMR_PINSHIFT(n))

#endif /* __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_IOMM_H */
