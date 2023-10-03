/****************************************************************************
 * arch/arm/include/k5-j721e/chip.h
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

#ifndef __ARCH_ARM_INCLUDE_K5_J721E_CHIP_H
#define __ARCH_ARM_INCLUDE_K5_J721E_CHIP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

#  undef  K5_J721E_CORTEX_R4             /* Not Cortex-R4 family */
#  undef  K5_J721E_CORTEX_R4F            /* Not Cortex-R4F family */
#  undef  K5_J721E_CORTEX_R5             /* Not Cortex-R5 family */
#  define K5_J721E_CORTEX_R5F  1         /* Cortex-R5F family */
#  undef  K5_J721E_CORTEX_R7             /* Not Cortex-R7 family */
#  undef  K5_J721E_CORTEX_R7F            /* Not Cortex-R7F family */
#  define K5_J721E_PFLASH      (128*2014)/* 128 KB Program FLASH */
#  define K5_J721E_SRAM        (32*1024) /* 32 KB SRAM */
#  define K5_J721E_DFLASH      (16*1024) /* 16 KB Data FLASH (EEPROM) */
#  define K5_J721E_NEMAC       0         /* No 10/100 Mbit EMAC */
#  define K5_J721E_FLEXRAY_NCH 0         /* No Flexray channels */
#  define K5_J721E_NCAN        2         /* Two CAN */
#  define K5_J721E_NMIBADC     1         /* One MiBADC */
#  define K5_J721E_MIBADC_NCH  16        /* 16 MibADC channels */
#  define K5_J721E_NN2HET      1         /* One N2HET */
#  define K5_J721E_N2HET_NCH   19        /* 19 N2HET channels */
#  define K5_J721E_EPWM_NCH    0         /* No ePWM channels */
#  define K5_J721E_ECAP_NCH    0         /* No eCAP channels */
#  define K5_J721E_EQEP_NCH    1         /* One eQEP channel */
#  define K5_J721E_NMIBSPI     1         /* One MibSPI */
#  define K5_J721E_MIBSPI1_NCS 4         /* MibSPI1: 4 chip selects */
#  define K5_J721E_MIBSPI2_NCS 0         /* MibSPI2: No chip selects */
#  define K5_J721E_MIBSPI3_NCS 0         /* MibSPI3: No chip selects */
#  define K5_J721E_NSPI        2         /* Two SPI */
#  define K5_J721E_SPI1_NCS    0         /* SPI1: No chip selects */
#  define K5_J721E_SPI2_NCS    0         /* SPI2: No chip selects */
#  define K5_J721E_NSCI        1         /* One SCI */
#  define K5_J721E_SCI1_LIN    1         /* SCI1: LIN supported */
#  undef  K5_J721E_SCI2_LIN              /* SCI2: LIN not supported */
#  define K5_J721E_NI2C        0         /* No I2C */
#  define K5_J721E_NGPIOINT    8         /* 8 GPIO interrupts */
#  define K5_J721E_NEMIF16     0         /* No EMIF 16-bit data */
#  undef  K5_J721E_ETM                   /* No ETM (trace) */
#  undef  K5_J721E_RTP                   /* No RAM trace port (RTP) */
#  undef  K5_J721E_DMM                   /* No DMM */

#endif /* __ARCH_ARM_INCLUDE_K5_J721E_CHIP_H */
