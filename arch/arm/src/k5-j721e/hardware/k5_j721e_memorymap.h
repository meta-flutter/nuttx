/****************************************************************************
 * arch/arm/src/k5-j721e/hardware/k5_j721e_memorymap.h
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

#ifndef __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_MEMORYMAP_H
#define __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_MEMORYMAP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Memory Map Overview */

#define K5_J721E_FLASH_BASE         0x00000000 /* 0x00000000-0x0005ffff: Program FLASH */
                                             /* 0x00060000-0x07ffffff: Reserved */
#define K5_J721E_RAM_BASE           0x08000000 /* 0x08000000-0x08007fff: RAM */
                                             /* 0x08008000-0x1fffffff: Reserved */
#define K5_J721E_MIRROR_BASE        0x20000000 /* 0x20000000-0x2005ffff: Program FLASH mirror */
                                             /* 0x20060000-0xefffffff: Reserved */
#define K5_J721E_BUS2_BASE          0xf0000000 /* 0xf0000000-0xf07fffff: Flash Module BUS2 Interface */
                                             /* 0xf0800000-0xfbffffff: Reserved */
#define K5_J721E_PERIPH2_BASE       0xfc000000 /* 0xfc000000-0xfcffffff: Peripherals - Frame 2 */
                                             /* 0xfd000000-0xfdffffff: Reserved */
#define K5_J721E_CRC_BASE           0xfe000000 /* 0xfe000000-0xfeffffff: CRC */
#define K5_J721E_PERIPH1_BASE       0xff000000 /* 0xff000000-0xff7fffff: Peripherals - Frame 1 */
#define K5_J721E_SYSTEM_BASE        0xfff80000 /* 0xff800000-0xffffffff: System Modules */

/* Flash Bus2 Interface: OTP, ECC, EEPROM Bank */

#define K5_J721E_CUSTTCM_BASE       0xf0000000 /* 0xf0000000-0xf000dfff: Customer OTP, TCM FLASH bank */
#define K5_J721E_CUSTEEPROM_BASE    0xf000e000 /* 0xf000e000-0xf000ffff: Customer OTP, EEPROM bank */
#define K5_J721E_CUSTECC_BASE       0xf0040000 /* 0xf0040000-0xf00403ff: Customer OTP-ECC, TCM FLASH bank */
#define K5_J721E_CUSTECCEEPROM_BASE 0xf0040000 /* 0xf0041c00-0xf0041fff: Customer OTP-ECC, EEPROM bank */

#define K5_J721E_TITCM_BASE         0xf0080000 /* 0xf0080000-0xf008dfff: TI OTP, TCM FLASH bank */
#define K5_J721E_TIEEPROM_BASE      0xf008e000 /* 0xf008e000-0xf008ffff: TI OTP, EEPROM bank */
#define K5_J721E_TIECC_BASE         0xf00c0000 /* 0xf00c0000-0xf00c03ff: TI OTP-ECC, TCM FLASH bank */
#define K5_J721E_TIECCEEPROM_BASE   0xf00c1c00 /* 0xf00c1c00-0xf00c1fff: TI OTP-ECC, EEPROM bank */

#define K5_J721E_EEC_BASE           0xf0100000 /* 0xf0100000-0xf013ffff: EEPROM Bank - EEC */
#define K5_J721E_EEPROM_BASE        0xf0200000 /* 0xf0200000-0xf03fffff: EEPROM Bank */
#define K5_J721E_FDATA_BASE         0xf0400000 /* 0xf0400000-0xf04fffff: FLASH Data Space - ECC */

/* Debug Components */

#define K5_J721E_CORESIGHT_BASE     0xffa00000 /* 0xffa00000-0xffa00fff: CoreSight Debug ROM */
#define K5_J721E_CORTEXR4_BASE      0xffa01000 /* 0xffa01000-0xffa01fff: Cortex-R4 Debug */

/* Peripheral Memories */

#define K5_J721E_MIBSPI1RAM_BASE    0xff0e0000 /* 0xff0e0000-0xff0fffff: MIBSPI1 RAM */
#define K5_J721E_DCAN2RAM_BASE      0xff1c0000 /* 0xff1c0000-0xff1dffff: DCAN2 RAM */
#define K5_J721E_DCAN1RAM_BASE      0xff1e0000 /* 0xff1c0000-0xff1fffff: DCAN1 RAM */
#define K5_J721E_MIBADCRAM_BASE     0xff3e0000 /* 0xff3e0000-0xff3fffff: MIBADC RAM */
#define K5_J721E_MIBADCLUT_BASE     0xff3e0000 /* 0xff3e0000-0xff3fffff: MIBADC Lookup Table */
#define K5_J721E_N2HETRAM_BASE      0xff460000 /* 0xff460000-0xff47ffff: N2HET RAM */
#define K5_J721E_HETTURAM_BASE      0xff4e0000 /* 0xff4e0000-0xff4fffff: HET TU RAM */

/* Peripheral Control Registers */

#define K5_J721E_HTU_BASE           0xfff7a400 /* 0xfff7a400-0xfff7a4ff: HTU */
#define K5_J721E_N2HET_BASE         0xfff7b800 /* 0xfff7b800-0xfff7b8ff: N2HET */
#define K5_J721E_GIO_BASE           0xfff7bc00 /* 0xfff7bc00-0xfff7bcff: GIO */
#define K5_J721E_MIBADC_BASE        0xfff7bc00 /* 0xfff7c000-0xfff7c1ff: MIBADC */
#define K5_J721E_DCAN1_BASE         0xfff7dc00 /* 0xfff7dc00-0xfff7ddff: DCAN1 */
#define K5_J721E_DCAN2_BASE         0xfff7de00 /* 0xfff7de00-0xfff7dfff: DCAN2 */
#define K5_J721E_SCI1_BASE          0xfff7e400 /* 0xfff7e400-0xfff7e4ff: SCI1/LIN1 */
#define K5_J721E_MIBSPI1_BASE       0xfff7f400 /* 0xfff7f400-0xfff7f5ff: MibSPI1 */
#define K5_J721E_SPI2_BASE          0xfff7f600 /* 0xfff7f600-0xfff7f7ff: SPI2 */
#define K5_J721E_SPI3_BASE          0xfff7f800 /* 0xfff7f800-0xfff7f9ff: SPI3 */
#define K5_J721E_EQEP_BASE          0xfff79900 /* 0xfff79900-0xfff799ff: EQEP */
#define K5_J721E_EQEPM_BASE         0xfcf79900 /* 0xfcf79900-0xfcf799ff: EQEP (Mirrored) */

/* System Modules Control Registers and Memories */

#define K5_J721E_VIMRAM_BASE        0xfff82000 /* 0xfff82000-0xfff82fff: VIM RAM */
#define K5_J721E_FWRAP_BASE         0xfff87000 /* 0xfff87000-0xfff87fff: Flash Wrapper */
#define K5_J721E_EFFC_BASE          0xfff8c000 /* 0xfff8c000-0xfff8cfff: eFuse Farm Controller */
#define K5_J721E_PCR_BASE           0xffffe000 /* 0xffffe000-0xffffe0ff: PCR registers */
#define K5_J721E_SYS2_BASE          0xffffe100 /* 0xffffe100-0xffffe1ff: System Module - Frame 2 */
#define K5_J721E_PBIST_BASE         0xffffe400 /* 0xffffe400-0xffffe5ff: PBIST */
#define K5_J721E_STC_BASE           0xffffe600 /* 0xffffe600-0xffffe6ff: STC */
#define K5_J721E_IOMM_BASE          0xffffea00 /* 0xffffea00-0xffffeBff: IOMM Multiplexing */
#define K5_J721E_DCC_BASE           0xffffec00 /* 0xffffec00-0xffffeCff: DCC */
#define K5_J721E_ESM_BASE           0xfffff500 /* 0xfffff500-0xfffff5ff: ESM */
#define K5_J721E_CCMR4_BASE         0xfffff600 /* 0xfffff600-0xfffff6ff: CCM-R4 */
#define K5_J721E_RAMECCE_BASE       0xfffff800 /* 0xfffff800-0xfffff8ff: RAM ECC even */
#define K5_J721E_RAMECCO_BASE       0xfffff900 /* 0xfffff900-0xfffff9ff: RAM ECC odd */
#define K5_J721E_RTIDWWD_BASE       0xfffffc00 /* 0xfffffc00-0xfffffcff: RTI + DWWD */
#define K5_J721E_VIMPAR_BASE        0xfffffd00 /* 0xfffffd00-0xfffffdff: VIM Parity */
#define K5_J721E_VIM_BASE           0xfffffe00 /* 0xfffffe00-0xfffffeff: VIM */
#define K5_J721E_SYS_BASE           0xffffff00 /* 0xffffff00-0xffffffff: System Module - Frame 1 */

#endif /* __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_MEMORYMAP_H */
