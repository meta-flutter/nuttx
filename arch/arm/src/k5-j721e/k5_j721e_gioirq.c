/****************************************************************************
 * arch/arm/src/k5_j721e/k5_j721e_gioirq.c
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
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/init.h>
#include <nuttx/arch.h>

#include <nuttx/irq.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "k5_j721e_gio.h"
#include "hardware/k5_j721e_gio.h"

#ifdef CONFIG_K5_J721E_GIO_IRQ

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: k5_j721e_gio_interrupt
 *
 * Description:
 *   Receive GIO interrupts
 *
 ****************************************************************************/

static int tms3570_gio_interrupt(int irq, void *context, void *arg)
{
  uint32_t off1;
  int irq2;

  /* Loop until all pending GIO interrupts have been processed */

  while ((off1 = getreg32(K5_J721E_GIO_OFF1)) != GIO_OFF_NONE)
    {
      /* Convert the offset value to the second-level IRQ number */

      irq2 = off1 + K5_J721E_IRQ_GIOA0 - 1;

      /* And dispatch the second-level GIO IRQ */

      irq_dispatch(irq2, context);
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: k5_j721e_gioirqinitialize
 *
 * Description:
 *   Initialize logic to support a second level of interrupt decoding for
 *   GIO pins.
 *
 ****************************************************************************/

void k5_j721e_gioirq_initialize(void)
{
  /* Disable all pin interrupts on the pin.
   * Make sure they are all level 0.
   */

  putreg32(0xffffffff, K5_J721E_GIO_ENACLR);
  putreg32(0xffffffff, K5_J721E_GIO_LVLCLR);

  /* Attach and enable the GIO level 0 interrupt */

  DEBUGVERIFY(irq_attach(K5_J721E_REQ_GIO_0, tms3570_gio_interrupt, NULL));
  up_enable_irq(K5_J721E_REQ_GIO_0);
}

/****************************************************************************
 * Name: k5_j721e_gioirq
 *
 * Description:
 *   Configure an interrupt for the specified GIO pin.
 *
 ****************************************************************************/

void k5_j721e_gioirq(gio_pinset_t pinset)
{
  uint32_t port = k5_j721e_gio_port(pinset);
  uint32_t pin = k5_j721e_gio_pin(pinset);
  irqstate_t flags;
  uint32_t regval;

  /* Start with the pin interrupts disabled.
   *  Make sure that level 0 is selected.
   */

  putreg32(GIO_ENACLR_PORT_PIN(port, pin), K5_J721E_GIO_ENACLR);
  putreg32(GIO_LVLCLR_PORT_PIN(port, pin), K5_J721E_GIO_LVLCLR);

  /* Make sure that the pin is configured as an input and that interrupts can
   * e supported on this port.
   */

  if ((pinset & GIO_MODE_MASK) == GIO_INPUT && port < K5_J721E_NIRQPORTS)
    {
      flags = enter_critical_section();
      switch (pinset & GIO_INT_MASK)
      {
        case GIO_INT_NONE:
        default:
          break;

        case GIO_INT_RISING:
          {
             /* Enable rising edge detectioni */

             regval  = getreg32(K5_J721E_GIO_POL);
             regval |= GIO_POL_PORT_PIN(port, pin);
             putreg32(regval, K5_J721E_GIO_POL);

             /* Disable both rising and falling edge detection */

             regval  = getreg32(K5_J721E_GIO_INTDET);
             regval &= ~GIO_INTDET_PORT_PIN(port, pin);
             putreg32(regval, K5_J721E_GIO_INTDET);
          }
          break;

        case GIO_INT_FALLING:
          {
             /* Enable falling edge detectioni */

             regval  = getreg32(K5_J721E_GIO_POL);
             regval &= ~GIO_POL_PORT_PIN(port, pin);
             putreg32(regval, K5_J721E_GIO_POL);

             /* Disable both rising and falling edge detection */

             regval  = getreg32(K5_J721E_GIO_INTDET);
             regval &= ~GIO_INTDET_PORT_PIN(port, pin);
             putreg32(regval, K5_J721E_GIO_INTDET);
          }
          break;

        case GIO_INT_BOTHEDGES:
          {
             /* Enable both rising and falling edge detection */

             regval = getreg32(K5_J721E_GIO_INTDET);
             regval |=  GIO_INTDET_PORT_PIN(port, pin);
             putreg32(regval, K5_J721E_GIO_INTDET);
          }
          break;
        }

      leave_critical_section(flags);
    }
}

/****************************************************************************
 * Name: k5_j721e_gioirqenable
 *
 * Description:
 *   Enable the interrupt for specified GIO IRQ
 *
 ****************************************************************************/

void k5_j721e_gioirqenable(int irq)
{
  int offset;
  int port;
  int pin;

  offset = irq - K5_J721E_IRQ_GIOA0;
  if (offset < K5_J721E_NGIO_IRQS)
    {
      /* Convert the offset IRQ number to a port and pin number */

      pin  = offset & 7;
      port = offset >> 3;

      /* Enable this pin interrupt */

      putreg32(GIO_ENACLR_PORT_PIN(port, pin), K5_J721E_GIO_ENASET);
    }
}

/****************************************************************************
 * Name: k5_j721e_gioirqdisable
 *
 * Description:
 *   Disable the interrupt for specified GIO IRQ
 *
 ****************************************************************************/

void k5_j721e_gioirqdisable(int irq)
{
  int offset;
  int port;
  int pin;

  offset = irq - K5_J721E_IRQ_GIOA0;
  if (offset < K5_J721E_NGIO_IRQS)
    {
      /* Convert the offset IRQ number to a port and pin number */

      pin  = offset & 7;
      port = offset >> 3;

      /* Enable this pin interrupt */

      putreg32(GIO_ENACLR_PORT_PIN(port, pin), K5_J721E_GIO_ENACLR);
    }
}

#endif /* CONFIG_K5_J721E_GIO_IRQ */
