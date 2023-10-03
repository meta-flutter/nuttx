/****************************************************************************
 * arch/arm/src/k5_j721e/hardware/k5_j721e_pinmux.h
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

#ifndef __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_PINMUX_H
#define __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_PINMUX_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pulbic Type Definitions
 ****************************************************************************/

/* Each chip-specific pinmux header file defines initializers for a type
 * like:
 */

struct k5_j721e_pinmux_s
{
  uint8_t mmrndx;  /* Index to the PINMMR register, 0-30 */
  uint8_t shift;   /* Shift value to isolate the pin field in the PINMMR register */
  uint8_t value;   /* The new value for the pin field in the PINMMR register */
};

#endif /* __ARCH_ARM_SRC_K5_J721E_HARDWARE_K5_J721E_PINMUX_H */
