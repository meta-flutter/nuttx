/****************************************************************************
 * arch/arm/src/k5_j721e/k5_j721e_gio.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "hardware/k5_j721e_gio.h"
#include "k5_j721e_gio.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_DEBUG_GPIO_INFO
static const char g_portchar[K5_J721E_NPORTS] =
{
  'A'
#if K5_J721E_NPORTS > 1
  , 'B'
#endif
#if K5_J721E_NPORTS > 2
  , 'C'
#endif
#if K5_J721E_NPORTS > 3
  , 'D'
#endif
#if K5_J721E_NPORTS > 4
  , 'E'
#endif
#if K5_J721E_NPORTS > 5
  , 'F'
#endif
#if K5_J721E_NPORTS > 6
  , 'G'
#endif
#if K5_J721E_NPORTS > 7
  , 'H'
#endif
};
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: k5_j721e_gio_initialize
 *
 * Description:
 *   Take the GIO block out of reset and assure that it is ready for use.
 *
 ****************************************************************************/

int k5_j721e_gio_initialize(void)
{
  /* Take the GIO block out of reset */

  putreg32(GIO_GCR0_RESET, K5_J721E_GIO_GCR0);

  /* Disable all pin interrupts on the pin.
   * Make sure they are all level 0.
   */

  putreg32(0xffffffff, K5_J721E_GIO_ENACLR);
  putreg32(0xffffffff, K5_J721E_GIO_LVLCLR);
  return OK;
}

/****************************************************************************
 * Name: k5_j721e_configgio
 *
 * Description:
 *   Configure a GIO pin based on bit-encoded description of the pin.
 *
 ****************************************************************************/

int k5_j721e_configgio(gio_pinset_t cfgset)
{
  uint32_t port = k5_j721e_gio_port(cfgset);
  uintptr_t base = k5_j721e_gio_base(cfgset);
  uint32_t pin = k5_j721e_gio_pin(cfgset);
  uint32_t pinmask = k5_j721e_gio_pinmask(cfgset);
  uint32_t regval;
  irqstate_t flags;

  /* Disable interrupts to prohibit re-entrance. */

  flags = enter_critical_section();

  /* Force the pin to be an input for now */

  regval  = getreg32(base + K5_J721E_GIO_DIR_OFFSET);
  regval &= ~pinmask;
  putreg32(regval, base + K5_J721E_GIO_DIR_OFFSET);

  /* Disable interrupts on the pin.  Make sure this is a level 0 pin. */

  putreg32(GIO_ENACLR_PORT_PIN(port, pin), K5_J721E_GIO_ENACLR);
  putreg32(GIO_LVLCLR_PORT_PIN(port, pin), K5_J721E_GIO_LVLCLR);

  /* Setup settings common to both input and output pins */

  /* Enable/disable the pull-up/down as requested */

  switch (cfgset & GIO_CFG_MASK)
    {
      case GIO_CFG_DEFAULT: /* Default, no attribute */
      default:
        {
          /* Disable pull functionality */

          regval  = getreg32(base + K5_J721E_GIO_PULDIS_OFFSET);
          regval &= ~pinmask;
          putreg32(regval, base + K5_J721E_GIO_PULDIS_OFFSET);
        }
        break;

      case GIO_CFG_PULLUP: /* Internal pull-up */
        {
          /* Select pull-up */

          regval  = getreg32(base + K5_J721E_GIO_PSL_OFFSET);
          regval |= pinmask;
          putreg32(regval, base + K5_J721E_GIO_PSL_OFFSET);

          /* Enable pull functionality */

          regval  = getreg32(base + K5_J721E_GIO_PULDIS_OFFSET);
          regval |= pinmask;
          putreg32(regval, base + K5_J721E_GIO_PULDIS_OFFSET);
        }
        break;

      case GIO_CFG_PULLDOWN: /* Internal pull-down */
        {
          /* Select pull-down */

          regval  = getreg32(base + K5_J721E_GIO_PSL_OFFSET);
          regval |= pinmask;
          putreg32(regval, base + K5_J721E_GIO_PSL_OFFSET);

          /* Enable pull functionality */

          regval  = getreg32(base + K5_J721E_GIO_DIR_OFFSET);
          regval |= pinmask;
          putreg32(regval, base + K5_J721E_GIO_DIR_OFFSET);
        }
        break;
    }

  /* Then do unique operations for an output pin */

  if ((cfgset & GIO_MODE_MASK) == GIO_OUTPUT)
    {
      /* Enable the open drain driver if requested */

      regval = getreg32(base + K5_J721E_GIO_PDR_OFFSET);
      if ((cfgset & GIO_OPENDRAIN) != 0)
        {
          regval |= pinmask;
        }
      else
        {
          regval &= ~pinmask;
        }

      putreg32(regval, base + K5_J721E_GIO_PDR_OFFSET);

      /* Set default output value */

      if ((cfgset & GIO_OUTPUT_SET) != 0)
       {
          putreg32(pinmask, base + K5_J721E_GIO_DSET_OFFSET);
        }
      else
        {
          putreg32(pinmask, base + K5_J721E_GIO_DCLR_OFFSET);
        }

      /* Finally, configure the pin as an output */

      regval  = getreg32(base + K5_J721E_GIO_DIR_OFFSET);
      regval |= pinmask;
      putreg32(regval, base + K5_J721E_GIO_DIR_OFFSET);
    }

  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: k5_j721e_giowrite
 *
 * Description:
 *   Write one or zero to the selected GIO pin
 *
 ****************************************************************************/

void k5_j721e_giowrite(gio_pinset_t pinset, bool value)
{
  uintptr_t base = k5_j721e_gio_base(pinset);
  uint32_t pinmask = k5_j721e_gio_pinmask(pinset);

  if (value)
    {
      putreg32(pinmask, base + K5_J721E_GIO_DSET_OFFSET);
    }
  else
    {
      putreg32(pinmask, base + K5_J721E_GIO_DCLR_OFFSET);
    }
}

/****************************************************************************
 * Name: k5_j721e_gioread
 *
 * Description:
 *   Read one or zero from the selected GIO pin
 *
 ****************************************************************************/

bool k5_j721e_gioread(gio_pinset_t pinset)
{
  uintptr_t base = k5_j721e_gio_base(pinset);
  uint32_t pinmask = k5_j721e_gio_pinmask(pinset);
  uint32_t regval;

  if ((pinset & GIO_MODE_MASK) == GIO_OUTPUT)
    {
      regval = getreg32(base + K5_J721E_GIO_DOUT_OFFSET);
    }
  else
    {
      regval = getreg32(base + K5_J721E_GIO_DIN_OFFSET);
    }

  return (regval & pinmask) != 0;
}

/****************************************************************************
 * Function:  k5_j721e_dumpgio
 *
 * Description:
 *   Dump all GIO registers associated with the base address of the provided
 *   pinset.
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_GPIO_INFO
int k5_j721e_dumpgio(uint32_t pinset, const char *msg)
{
  irqstate_t    flags;
  uintptr_t     base;
  unsigned int  port;

  _info("GIO%c pinset: %08x base: %08x -- %s\n",
        g_portchar[port], pinset, base, msg);

  /* Get the base address associated with the GIO port */

  port = (pinset & GIO_PORT_MASK) >> GIO_PORT_SHIFT;
  base = K5_J721E_GIO_PORTBASE(port);

  /* The following requires exclusive access to the GIO registers */

  flags = enter_critical_section();

  /* Show global GIO registers */

  _info("   GCR0: %08x INTDET: %08x    POL: %08x   ENA: %08x\n",
        getreg32(K5_J721E_GIO_GCR0), getreg32(K5_J721E_GIO_INTDET),
        getreg32(K5_J721E_GIO_POL), getreg32(K5_J721E_GIO_ENASET));
  _info("    LVL: %08x    FLG: %08x   EMU1: %08x   EMU2: %08x\n",
        getreg32(K5_J721E_GIO_LVLSET), getreg32(K5_J721E_GIO_FLG),
        getreg32(K5_J721E_GIO_EMU1), getreg32(K5_J721E_GIO_EMU2));

  /* Port specific registers */

  _info("    DIR: %08x    DIN: %08x   DOUT: %08x    PDR: %08x\n",
        getreg32(base + K5_J721E_GIO_DIR_OFFSET),
        getreg32(base + K5_J721E_GIO_DIN_OFFSET),
        getreg32(base + K5_J721E_GIO_DOUT_OFFSET),
        getreg32(base + K5_J721E_GIO_PDR_OFFSET));
  _info(" PULDIS: %08x    PSL: %08x\n",
        getreg32(base + K5_J721E_GIO_PULDIS_OFFSET),
        getreg32(base + K5_J721E_GIO_PSL_OFFSET));

  leave_critical_section(flags);
  return OK;
}
#endif
