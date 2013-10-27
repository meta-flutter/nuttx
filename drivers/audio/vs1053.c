/****************************************************************************
 * drivers/audio/vs1053.c
 *
 * Audio device driver for VLSI Solutions VS1053 Audio codec.
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <debug.h>
#include <errno.h>
#include <queue.h>

#include <nuttx/kmalloc.h>
#include <nuttx/fs/fs.h>
#include <nuttx/fs/ioctl.h>
#include <nuttx/audio/audio.h>
#include <nuttx/audio/vs1053.h>
#include <nuttx/math.h>

#include "vs1053.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

#ifndef CONFIG_VS1053_SPIMODE
#  define CONFIG_VS1053_SPIMODE SPIDEV_MODE0
#endif

#ifndef CONFIG_VS1053_XTALI
#  define CONFIG_VS1053_XTALI             12288000
#endif

#ifndef CONFIG_VS1053_MP3_DECODE_FREQ
#  define CONFIG_VS1053_MP3_DECODE_FREQ   43000000
#endif

#ifndef CONFIG_VS1053_MSG_PRIO
#  define CONFIG_VS1053_MSG_PRIO          1
#endif

#ifndef CONFIG_VS1053_BUFFER_SIZE
#  define CONFIG_VS1053_BUFFER_SIZE       8192
#endif

#ifndef CONFIG_VS1053_NUM_BUFFERS
#  define CONFIG_VS1053_NUM_BUFFERS       2
#endif

#define VS1053_DUMMY                      0xFF
#define VS1053_DEFAULT_XTALI              12288000
#define VS1053_DATA_FREQ                  20000000
#define VS1053_RST_USECS                  2000

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct vs1053_struct_s
{
  /* We are an audio lower half driver */

  struct audio_lowerhalf_s lower;

  /* Our specific driver data goes here */

  const FAR struct vs1053_lower_s *hw_lower;/* Pointer to the hardware lower functions */
  FAR struct spi_dev_s    *spi;             /* Pointer to the SPI bus */
  FAR struct ap_buffer_s  *pBuf;            /* Pointer to the buffer we are processing */
  struct dq_queue_s       apbq;             /* Our queue for enqueued buffers */
  unsigned long           spi_freq;         /* Frequency to run the SPI bus at. */
  unsigned long           chip_freq;        /* Current chip frequency */
  mqd_t                   mq;               /* Message queue for receiving messages */
  char                    mqname[16];       /* Our message queue name */
  pthread_t               threadid;         /* ID of our thread */
  sem_t                   apbq_sem;         /* Audio Pipeline Buffer Queue sem access */
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
  int16_t                 volume;           /* Current volume level */
#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
  int16_t                 balance;          /* Current balance level */
#endif  /* CONFIG_AUDIO_EXCLUDE_BALANCE */
#endif  /* CONFIG_AUDIO_EXCLUDE_VOLUME */
#ifndef CONFIG_AUDIO_EXCLUDE_TONE
  uint8_t                 bass;             /* Bass level */
  uint8_t                 treble;           /* Bass level */
#endif
  uint16_t                endfillbytes;
  uint8_t                 endfillchar;      /* Fill char to send when no more data */
  uint8_t                 running;
  uint8_t                 paused;
  uint8_t                 endmode;
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
  uint8_t                 cancelmode;
#endif
  uint8_t                 busy;             /* Set true when device reserved */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int     vs1053_getcaps(FAR struct audio_lowerhalf_s *lower, int type,
                 FAR struct audio_caps_s *pCaps);
static int     vs1053_shutdown(FAR struct audio_lowerhalf_s *lower);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int     vs1053_configure(FAR struct audio_lowerhalf_s *lower,
                 FAR void *session, FAR const struct audio_caps_s *pCaps);
static int     vs1053_start(FAR struct audio_lowerhalf_s *lower,
                 FAR void *session);
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
static int     vs1053_stop(FAR struct audio_lowerhalf_s *lower,
                 FAR void *session);
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
static int     vs1053_pause(FAR struct audio_lowerhalf_s *lower,
                 FAR void *session);
static int     vs1053_resume(FAR struct audio_lowerhalf_s *lower,
                 FAR void *session);
#endif  /* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */
static int     vs1053_reserve(FAR struct audio_lowerhalf_s *lower,
                 FAR void** ppContext);
static int     vs1053_release(FAR struct audio_lowerhalf_s *lower,
                 FAR void* pContext);
#else
static int     vs1053_configure(FAR struct audio_lowerhalf_s *lower,
                 FAR const struct audio_caps_s *pCaps);
static int     vs1053_start(FAR struct audio_lowerhalf_s *lower);
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
static int     vs1053_stop(FAR struct audio_lowerhalf_s *lower);
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
static int     vs1053_pause(FAR struct audio_lowerhalf_s *lower);
static int     vs1053_resume(FAR struct audio_lowerhalf_s *lower);
#endif  /* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */
static int     vs1053_reserve(FAR struct audio_lowerhalf_s *lower);
static int     vs1053_release(FAR struct audio_lowerhalf_s *lower);
#endif /* CONFIG_AUDIO_MULTI_SESION */
static int     vs1053_enqueuebuffer(FAR struct audio_lowerhalf_s *lower,
                 FAR struct ap_buffer_s *apb);
static int     vs1053_cancelbuffer(FAR struct audio_lowerhalf_s *lower,
                 FAR struct ap_buffer_s *apb);
static int     vs1053_ioctl(FAR struct audio_lowerhalf_s *lower, int cmd,
                 unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct audio_ops_s g_audioops =
{
  vs1053_getcaps,       /* getcaps        */
  vs1053_configure,     /* configure      */
  vs1053_shutdown,      /* shutdown       */
  vs1053_start,         /* start          */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
  vs1053_stop,          /* stop           */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
  vs1053_pause,         /* pause          */
  vs1053_resume,        /* resume         */
#endif
  NULL,                 /* alloc_buffer   */
  NULL,                 /* free_buffer    */
  vs1053_enqueuebuffer, /* enqueue_buffer */
  vs1053_cancelbuffer,  /* cancel_buffer  */
  vs1053_ioctl,         /* ioctl          */
  NULL,                 /* read           */
  NULL,                 /* write          */
  vs1053_reserve,       /* reserve        */
  vs1053_release        /* release        */
};

/* ISR context pointers */

static struct vs1053_struct_s* g_isrdata[CONFIG_VS1053_DEVICE_COUNT] = { NULL, };

/* Volume control log table.  This table is in increments of 2% of
 * requested volume level and is the register value that should be
 * programmed to the VS1053 to achieve that volume pecentage.
 */

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
static const uint8_t   g_logtable [] =
{
  254, 170, 140, 122, 110,    /* 0  - 8 */
  100, 92,  85,  80,  74,     /* 10 - 18 */
  70,  66,  62,  59,  55,     /* 20 - 28 */
  52,  49,  47,  44,  42,     /* 30 - 38 */
  40,  38,  36,  34,  32,     /* 40 - 48 */
  30,  28,  27,  25,  24,     /* 50 - 58 */
  22,  21,  19,  18,  17,     /* 60 - 68 */
  15,  14,  13,  12,  11,     /* 70 - 78 */
  10,   9,   8,   7,   6,     /* 80 - 88 */
   5,   4,   3,   2,   1,     /* 90 - 98 */
    0                         /* 100     */
};
#endif /* CONFIG_AUDIO_EXCLUDE_VOLUME */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: vs1053_spi_lock
 ************************************************************************************/

static void vs1053_spi_lock(FAR struct spi_dev_s *dev, unsigned long freq_mhz)
{
  /* On SPI busses where there are multiple devices, it will be necessary to
   * lock SPI to have exclusive access to the busses for a sequence of
   * transfers.  The bus should be locked before the chip is selected.
   *
   * This is a blocking call and will not return until we have exclusiv access to
   * the SPI buss.  We will retain that exclusive access until the bus is unlocked.
   */

  (void)SPI_LOCK(dev, true);

  /* After locking the SPI bus, the we also need call the setfrequency, setbits, and
   * setmode methods to make sure that the SPI is properly configured for the device.
   * If the SPI buss is being shared, then it may have been left in an incompatible
   * state.
   */

  SPI_SETMODE(dev, CONFIG_VS1053_SPIMODE);
  SPI_SETBITS(dev, 8);
  (void)SPI_SETFREQUENCY(dev, freq_mhz);
}

/************************************************************************************
 * Name: vs1053_spi_unlock
 ************************************************************************************/

static inline void vs1053_spi_unlock(FAR struct spi_dev_s *dev)
{
  (void)SPI_LOCK(dev, false);
}

/************************************************************************************
 * Name: vs1053_readreg - Read the specified 16-bit register from the
 *                        VS1053 device.  Caller must hold the SPI lock.
 ************************************************************************************/

static uint16_t vs1053_readreg(FAR struct vs1053_struct_s *dev, uint8_t reg)
{
  uint16_t ret;
  FAR struct spi_dev_s *spi = dev->spi;

  /* Select the AUDIO_CTRL device on the SPI bus */

  SPI_SELECT(spi, SPIDEV_AUDIO_CTRL, true);

  /* Send the WRITE command followed by the address */

  SPI_SEND(spi, VS1053_OPCODE_READ);
  SPI_SEND(spi, reg);

  /* Now read the 16-bit value */

  ret = SPI_SEND(spi, VS1053_DUMMY) << 8;
  ret |= SPI_SEND(spi, VS1053_DUMMY);

  /* Deselect the CODEC */

  SPI_SELECT(spi, SPIDEV_AUDIO_CTRL, false);

  return ret;
}

/************************************************************************************
 * Name: vs1053_writereg - Write the specified 16-bit register to the
 *                         VS1053 device.  Caller must hold the SPI lock.
 ************************************************************************************/

static void vs1053_writereg(FAR struct vs1053_struct_s *dev, uint8_t reg, uint16_t val)
{
  FAR struct spi_dev_s *spi = dev->spi;

  /* Select the AUDIO_CTRL device on the SPI bus */

  audvdbg("Write Reg %d = 0x%0X\n", reg, val);

  SPI_SELECT(spi, SPIDEV_AUDIO_CTRL, true);

  /* Send the WRITE command followed by the address */

  SPI_SEND(spi, VS1053_OPCODE_WRITE);
  SPI_SEND(spi, reg);

  /* Now read the 16-bit value */

  SPI_SEND(spi, val >> 8);
  SPI_SEND(spi, val & 0xFF);

  /* Deselect the CODEC */

  SPI_SELECT(spi, SPIDEV_AUDIO_CTRL, false);

  /* Short delay after a write for VS1053 processing time */

  usleep(10);
}

/****************************************************************************
 * Name: vs1053_setfrequency
 *
 * Description: Get the audio device capabilities
 *
 ****************************************************************************/

static int vs1053_setfrequency(FAR struct vs1053_struct_s *dev, uint32_t freq)
{
  double    factor;
  uint16_t  reg;
  uint8_t   timeout;

  audvdbg("Entry\n");

  /* Calculate the clock divisor based on the input frequency */

  factor = (double) freq / (double) CONFIG_VS1053_XTALI * 10.0 + 0.5;

  /* Check the input frequency against bounds */

  if (factor > 50.0)
    {
      audvdbg("Frequency too high!  Limiting to XTALI * 5\n");
      factor = 50.0;
      return -EINVAL;
    }

  if (factor < 10.0)
    {
      factor = 10.0;
    }

  /* Calculate the clock mulit register based on the factor */

  if ((int) factor == 10)
    {
      reg = 0;
    }
  else
    {
      reg = (((int) factor - 15) / 5) << VS1053_SC_MULT_SHIFT;
    }

  /* Set the MULT_ADD factor to the max to allow the chip to dynamically
   * increase the frequency the maximum amount as needed
   */

  reg |= (VS1053_SC_ADD_XTALIx20 << VS1053_SC_ADD_SHIFT);

  /* If we aren't running with a 12.228Mhz input crystal, then we
   * must tell the chip what the frequency is
   */

  if (CONFIG_VS1053_XTALI != VS1053_DEFAULT_XTALI)
    {
      /* Calculate register value based on equation: (XTALI - 8000000) / 4000
       * per the datasheet.
       */

      reg |= (CONFIG_VS1053_XTALI - 8000000) / 4000;
    }

  /* Now set the new clock multiplier register */

  vs1053_writereg(dev, VS1053_SCI_CLOCKF, reg);

  /* Wait for DREQ to go active */

  timeout = 200;
  while (!dev->hw_lower->read_dreq(dev->hw_lower) && timeout)
    {
      usleep(1000);
      timeout--;
    }

  /* Update our internal variables */

  dev->chip_freq = freq;
  dev->spi_freq = freq / 7;

  return OK;
}

/************************************************************************************
 * Name: vs1053_logapprox - Approximate the register value in .5 dB increments
 *                          level based on the percentage using a log table since
 *                          math libraries aren't available.
 ************************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
uint8_t vs1053_logapprox(int percent)
{
  /* Check percentage for bounds */

  if (percent >= 100)
    {
      return 0;
    }

  return (g_logtable[percent >> 1] + g_logtable[(percent+1) >> 1]) >> 1;
}
#endif /* CONFIG_AUDIO_EXCLUDE_VOLUME */

/************************************************************************************
 * Name: vs1053_setvolume - Set the right and left volume values in the VS1053
 *                          device based on the current volume and balance settings.
 ************************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
static void vs1053_setvolume(FAR struct vs1053_struct_s *dev)
{
  FAR struct spi_dev_s *spi = dev->spi;
  uint32_t              leftlevel;
  uint32_t              rightlevel;
  uint8_t               leftreg, rightreg;

  /* Constrain balance */
#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
  if (dev->balance > 1000)
    {
      dev->balance = 1000;
    }

  /* Calculate the left channel volume level */

  if (dev->balance <= 500)
    {
      leftlevel = dev->volume;
    }
  else if (dev->balance == 1000)
    {
      leftlevel = 0;
    }
  else
    {
      leftlevel = dev->volume * (1000 - dev->balance) / 500;
    }

  /* Calculate the right channel volume level */

  if (dev->balance >= 500)
    {
      rightlevel = dev->volume;
    }
  else if (dev->balance == 0)
    {
      rightlevel = 0;
    }
  else
    {
      rightlevel = dev->volume * dev->balance / 500;
    }
#else
  leftlevel = rightlevel = dev->volume;
#endif

  /* Calculate the left and right register values */
  /* The register sets the volume in dB which is a logrithmic scale,
   * so we must use log() to calculate the register value.
   */

  leftreg = vs1053_logapprox(leftlevel / 10);
  rightreg = vs1053_logapprox(rightlevel / 10);

  /* Lock the SPI bus to get exclsive access to the chip. */

  vs1053_spi_lock(spi, dev->spi_freq);
  vs1053_writereg(dev, VS1053_SCI_VOL, (leftreg << 8) | rightreg);
  vs1053_spi_unlock(spi);
}
#endif /* CONFIG_AUDIO_EXCLUDE_VOLUME */

/************************************************************************************
 * Name: vs1053_setbass - Set the bass and treble level as specified in the
 *                        context's bass and treble variables..
 *
 * The level and range are in whole percentage levels (0-100).
 *
 ************************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void vs1053_setbass(FAR struct vs1053_struct_s *dev)
{
  FAR struct spi_dev_s *spi = dev->spi;
  int       bass_range, bass_boost;
  int       treble_range, treble_boost;

  /* Calculate range and boost based on level */

  bass_boost = 15 * dev->bass / 100;
  bass_range = 15;
  treble_boost = 15 * dev->treble / 100;
  treble_range = 15;

  /* Lock the SPI bus to get exclsive access to the chip. */

  vs1053_spi_lock(spi, dev->spi_freq);
  vs1053_writereg(dev, VS1053_SCI_BASS, (treble_boost << 12) | (treble_range << 8) |
      (bass_boost << 4) | bass_range);
  vs1053_spi_unlock(spi);
}
#endif /* CONFIG_AUDIO_EXCLUDE_TONE */

/****************************************************************************
 * Name: vs1053_getcaps
 *
 * Description: Get the audio device capabilities
 *
 ****************************************************************************/

static int vs1053_getcaps(FAR struct audio_lowerhalf_s *lower, int type,
            FAR struct audio_caps_s *pCaps)
{
  audvdbg("Entry\n");

  /* Validate the structure */

  DEBUGASSERT(pCaps->ac_len >= sizeof(struct audio_caps_s));

  /* Fill in the caller's structure based on requested info */

  pCaps->ac_format[0] = 0;
  pCaps->ac_format[1] = 0;
  pCaps->ac_controls[0] = 0;
  pCaps->ac_controls[1] = 0;
  pCaps->ac_controls[2] = 0;
  pCaps->ac_controls[3] = 0;

  switch (pCaps->ac_type)
    {
      /* Caller is querying for the types of units we support */

      case AUDIO_TYPE_QUERY:

        /* Provide our overall capabilities.  The interfacing software
         * must then call us back for specific info for each capability.
         */

        pCaps->ac_channels = 2;       /* Stereo output */

        /* The input formats we can decode / accept */

        pCaps->ac_format[0] = 0
#ifdef CONFIG_AUDIO_FORMAT_AC3
              | AUDIO_FMT_AC3
#endif
#ifdef CONFIG_AUDIO_FORMAT_MP3
              | AUDIO_FMT_MP3
#endif
#ifdef CONFIG_AUDIO_FORMAT_WMA
              | AUDIO_FMT_WMA
#endif
#ifdef CONFIG_AUDIO_FORMAT_MIDI
              | AUDIO_FMT_MIDI
#endif
#ifdef CONFIG_AUDIO_FORMAT_PCM
              | AUDIO_FMT_PCM
#endif
          ;

#ifdef CONFIG_AUDIO_FORMAT_OGG_VORBIS
        pCaps->ac_format[1] = (AUDIO_FMT_OGG_VORBIS) >> 8;
#endif

        /* The types of audio units we implement */

        pCaps->ac_controls[0] = AUDIO_TYPE_OUTPUT | AUDIO_TYPE_FEATURE |
                                AUDIO_TYPE_PROCESSING;

        break;

      /* Provide capabilities of our OUTPUT unit */

      case AUDIO_TYPE_OUTPUT:

        pCaps->ac_channels = 2;

        switch (pCaps->ac_subtype)
          {
            case AUDIO_TYPE_QUERY:

              /* Report the Sample rates we support */

              pCaps->ac_controls[0] = AUDIO_SAMP_RATE_8K | AUDIO_SAMP_RATE_11K |
                                      AUDIO_SAMP_RATE_16K | AUDIO_SAMP_RATE_22K |
                                      AUDIO_SAMP_RATE_32K | AUDIO_SAMP_RATE_44K |
                                      AUDIO_SAMP_RATE_48K;
              break;

            case AUDIO_FMT_MP3:
            case AUDIO_FMT_WMA:
            case AUDIO_FMT_PCM:
              /* Report the Bit rates we support.  The bit rate support is actually a
               * complex function of the format and selected sample rate, and the datasheet
               * has multiple tables to indicate the supported bit rate vs sample rate vs
               * format.  The selected sample rate should be provided in the ac_format
               * field of the query, and only a single sample rate should be given.
               */

              /* TODO:  Create a table or set of tables to report this! */

              break;

            default:
              break;
          }

        break;

      /* Provide capabilities of our FEATURE units */

      case AUDIO_TYPE_FEATURE:

        /* If the sub-type is UNDEF, then report the Feature Units we support */

        if (pCaps->ac_subtype == AUDIO_FU_UNDEF)
          {
            /* Fill in the ac_controls section with the Feature Units we have */

            pCaps->ac_controls[0] = AUDIO_FU_VOLUME | AUDIO_FU_BASS | AUDIO_FU_TREBLE;
          }
        else
          {
            /* TODO:  Do we need to provide specific info for the Feature Units,
             * such as volume setting ranges, etc.?
             */
          }

        break;

      /* Provide capabilities of our PROCESSING unit */

      case AUDIO_TYPE_PROCESSING:

        switch (pCaps->ac_subtype)
          {
            case AUDIO_PU_UNDEF:

              /* Provide the type of Processing Units we support */

              pCaps->ac_controls[0] = AUDIO_PU_STEREO_EXTENDER;
              break;

            case AUDIO_PU_STEREO_EXTENDER:

              /* Proivde capabilities of our Stereo Extender */

              pCaps->ac_controls[0] = AUDIO_STEXT_ENABLE | AUDIO_STEXT_WIDTH;
              break;

            default:

              /* Other types of processing uint we don't support */

              break;
          }

        break;

      /* All others we don't support */

      default:

        /* Zero out the fields to indicate no support */

        pCaps->ac_subtype = 0;
        pCaps->ac_channels = 0;

        break;
    }

  /* Return the length of the audio_caps_s struct for validation of
   * proper Audio device type.
   */

  return pCaps->ac_len;
}

/****************************************************************************
 * Name: vs1053_configure
 *
 * Description: Configure the audio device for the specified  mode of
 *              operation.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int vs1053_configure(FAR struct audio_lowerhalf_s *lower,
            FAR void *session, FAR const struct audio_caps_s *pCaps)
#else
static int vs1053_configure(FAR struct audio_lowerhalf_s *lower,
            FAR const struct audio_caps_s *pCaps)
#endif
{
  int     ret = OK;
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) || !defined(CONFIG_AUDIO_EXCLUDE_TONE)
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;
#endif

  audvdbg("Entry\n");

  /* Process the configure operation */

  switch (pCaps->ac_type)
    {
      case AUDIO_TYPE_FEATURE:

        /* Process based on Feature Unit */

        switch (*((uint16_t *) pCaps->ac_format))
          {
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
            case AUDIO_FU_VOLUME:
              /* Set the volume */

              dev->volume = *((uint16_t *) pCaps->ac_controls);
              vs1053_setvolume(dev);

              break;
#endif  /* CONFIG_AUDIO_EXCLUDE_VOLUME */

#if !defined(CONFIG_AUDIO_EXCLUDE_TONE) && !defined(CONFIG_AUDIO_EXCLUDE_VOLUME)
            case AUDIO_FU_BALANCE:
              /* Set the volume */

              dev->balance = *((uint16_t *) pCaps->ac_controls);
              vs1053_setvolume(dev);

              break;
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
            case AUDIO_FU_BASS:
              /* Set the bass.  The percentage level (0-100) is in the
               * ac_controls[0] parameter.
               */

              dev->bass = pCaps->ac_controls[0];
              if (dev->bass > 100)
                dev->bass = 100;
              vs1053_setbass(dev);

              break;

            case AUDIO_FU_TREBLE:
              /* Set the treble.  The percentage level (0-100) is in the
               * ac_controls[0] parameter.
               */

              dev->treble = pCaps->ac_controls[0];
              if (dev->treble > 100)
                dev->treble = 100;
              vs1053_setbass(dev);

              break;
#endif  /* CONFIG_AUDIO_EXCLUDE_TONE */

            default:
              /* Others we don't support */

              break;
          }

        break;

      case AUDIO_TYPE_PROCESSING:

        /* We only support STEREO_EXTENDER */

        if (*((uint16_t *) pCaps->ac_format) == AUDIO_PU_STEREO_EXTENDER)
          {
          }

        break;
    }

  return ret;
}

/****************************************************************************
 * Name: vs1053_softreset
 *
 * Description: Performs a soft reset on the VS1053 chip by setting the
 *              RESET bit of the MODE register.
 *
 ****************************************************************************/

static int vs1053_softreset(FAR struct vs1053_struct_s *dev)
{
  uint16_t reg;
  uint16_t timeout;

  /* First disable interrupts, lower the frequency and lock the SPI bus */

  dev->hw_lower->disable(dev->hw_lower);   /* Disable the DREQ interrupt */
  vs1053_spi_lock(dev->spi, VS1053_DEFAULT_XTALI / 7);

  /* Now issue a reset command */

  reg = vs1053_readreg(dev, VS1053_SCI_MODE);
  vs1053_writereg(dev, VS1053_SCI_MODE, reg | VS1053_SM_RESET);

  /* Now wait for the SM_RESET to go inactive */

  timeout = 1000;
  while (vs1053_readreg(dev, VS1053_SCI_MODE) & VS1053_SM_RESET && timeout)
    {
      timeout--;
    }

  /* Switch to low frequency, Unlock the SPI bus and exit */

  vs1053_setfrequency(dev, CONFIG_VS1053_XTALI);
  vs1053_spi_unlock(dev->spi);
  return OK;
}

/****************************************************************************
 * Name: vs1053_hardreset
 *
 * Description: Performs a hardware reset on the VS1053 chip by toggling
 *              the RST line, disabling IRQ, and setting the default
 *              XTALI frequency.
 *
 ****************************************************************************/

static int vs1053_hardreset(FAR struct vs1053_struct_s *dev)
{
  dev->hw_lower->disable(dev->hw_lower);   /* Disable the DREQ interrupt */
  dev->hw_lower->reset(dev->hw_lower, false);
  usleep(10);
  dev->hw_lower->reset(dev->hw_lower, true);
  usleep(VS1053_RST_USECS);
  vs1053_setfrequency(dev, CONFIG_VS1053_XTALI);  /* Slow speed at first */

  return OK;
}

/****************************************************************************
 * Name: vs1053_shutdown
 *
 * Description: Shutdown the VS1053 chip and put it in the lowest power
 *              state possible.
 *
 ****************************************************************************/

static int vs1053_shutdown(FAR struct audio_lowerhalf_s *lower)
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;
  FAR struct spi_dev_s  *spi = dev->spi;

  audvdbg("Entry\n");
  vs1053_spi_lock(spi, dev->spi_freq);            /* Lock the device */
  vs1053_setfrequency(dev, CONFIG_VS1053_XTALI);  /* Reduce speed to minimum */
  vs1053_writereg(dev, VS1053_SCI_VOL, 0xFEFE);   /* Power down the DAC outputs */
  vs1053_spi_unlock(spi);                         /* Unlock the device */
  return OK;
}

/****************************************************************************
 * Name: vs1053_feeddata
 *
 * Description: Feeds more data to the vs1053 chip from the enqueued
 *              buffers.  It will continue feeding data until the DREQ
 *              line indicates it can't accept any more data.
 *
 ****************************************************************************/

static void vs1053_feeddata(FAR struct vs1053_struct_s *dev)
{
  int                   bytecount;
  int                   ret;
  uint8_t               *pSamp = NULL;
  uint16_t              reg;
  struct ap_buffer_s    *pBuf;
  FAR struct spi_dev_s  *spi = dev->spi;

  /* Check for false interrupt caused by an SCI transaction */

  if (!dev->hw_lower->read_dreq(dev->hw_lower) || dev->paused)
    {
      return;
    }

  /* Grab the SPI bus.  We can run at 20Mhz because we increased the
   * chip frequency above 40Mhz for the decode operation.
   */

  vs1053_spi_lock(spi, VS1053_DATA_FREQ);   /* Lock the SPI bus */
  SPI_SELECT(spi, SPIDEV_AUDIO_DATA, true); /* Select the VS1053 data bus */

  /* Local stack copy of our active buffer */

  pBuf = dev->pBuf;
  //auddbg("Entry pBuf=%p, Bytes left=%d\n", pBuf, pBuf->nbytes - pBuf->curbyte);

  /* Setup pointer to the next sample in the buffer */

  if (pBuf)
    {
      pSamp = &pBuf->samp[pBuf->curbyte];
    }
  else if (!dev->endmode)
    {
      SPI_SELECT(spi, SPIDEV_AUDIO_DATA, false);
      vs1053_spi_unlock(spi);
      return;
    }

  /* Loop until the FIFO is full */

  while (dev->hw_lower->read_dreq(dev->hw_lower))
    {
      /* If endmode, then send fill characters */

      if (dev->endmode)
        {
          bytecount = 32;
          while (bytecount)
            {
              SPI_SEND(spi, dev->endfillchar);
              bytecount--;
            }

          /* For the VS1053, after the file has been played, we must
           * send 2052 bytes of endfillchar per the datasheet.
           */

          dev->endfillbytes += 32;

          /* Process end mode logic.  We send 2080 bytes of endfillchar as
           * directed by the datasheet, then set SM_CANCEL.  Then we wait
           * until the chip clears SM_CANCEL while sending endfillchar
           * 32 bytes at a time.
           */

          if (dev->endfillbytes == 32*65)
            {
              /* After at least 2052 bytes, we send an SM_CANCEL */

              dev->hw_lower->disable(dev->hw_lower);   /* Disable the DREQ interrupt */
              (void)SPI_SETFREQUENCY(dev->spi, dev->spi_freq);
              reg = vs1053_readreg(dev, VS1053_SCI_MODE);
              vs1053_writereg(dev, VS1053_SCI_MODE, reg | VS1053_SM_CANCEL);
              dev->hw_lower->enable(dev->hw_lower);   /* Enable the DREQ interrupt */
            }
          else if (dev->endfillbytes >= 32*130)
            {
              /* Do a hard reset and terminate */

              vs1053_hardreset(dev);
              dev->running = FALSE;
              dev->endmode = FALSE;
              break;
            }
          else if (dev->endfillbytes > 32*65)
            {
              /* After each 32 byte of endfillchar, check the status
               * register to see if SM_CANCEL has been cleard.  If
               * it has been cleared, then we're done.
               */

              if (!(vs1053_readreg(dev, VS1053_SCI_STATUS) & VS1053_SM_CANCEL))
                {
                  (void)SPI_SETFREQUENCY(dev->spi, dev->spi_freq);
                  dev->hw_lower->disable(dev->hw_lower);   /* Disable the DREQ interrupt */
                  auddbg("HDAT1: 0x%0X   HDAT0: 0x%0X\n",
                      vs1053_readreg(dev, VS1053_SCI_HDAT1),
                      vs1053_readreg(dev, VS1053_SCI_HDAT0));
                  vs1053_writereg(dev, VS1053_SCI_WRAMADDR, VS1053_END_FILL_BYTE);
                  dev->endfillchar = vs1053_readreg(dev, VS1053_SCI_WRAM) >> 8;
                  auddbg("EndFillChar: 0x%0X\n", dev->endfillchar);
                  reg = vs1053_readreg(dev, VS1053_SCI_MODE);
                  vs1053_writereg(dev, VS1053_SCI_MODE, reg | VS1053_SM_RESET);
                  dev->running = FALSE;
                  dev->endmode = FALSE;
                  break;
                }
            }
        }
      else
        {
          /* Send 32 more bytes.  We only send 32 at a time because this is
           * the meaning of DREQ active from the chip ... that it can
           * accept at least 32 more bytes.  After each 32 byte block, we
           * will recheck the DREQ line again.
           */

          bytecount = pBuf->nbytes - pBuf->curbyte;
          if (bytecount > 32)
            {
              bytecount = 32;
            }
#if 1
          SPI_SNDBLOCK(spi, pSamp, bytecount);
          pSamp += bytecount;
#else
          bytecount = bytecount;
          while (bytecount--)
            {
              /* Send next byte from the buffer */

              SPI_SEND(spi, *pSamp);
              pSamp++;
            }
#endif
          pBuf->curbyte += bytecount;

          /* Test if we are in cancel mode.  If we are, then we need
           * to continue sending file data and check for the SM_CANCEL
           * bit going inactive.
           */

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
          if (dev->cancelmode)
            {
              /* Read the VS1053 MODE register */

              dev->hw_lower->disable(dev->hw_lower);   /* Disable the DREQ interrupt */
              (void)SPI_SETFREQUENCY(dev->spi, dev->spi_freq);
              reg = vs1053_readreg(dev, VS1053_SCI_MODE);
              (void)SPI_SETFREQUENCY(dev->spi, VS1053_DATA_FREQ);
              dev->hw_lower->enable(dev->hw_lower);   /* Enable the DREQ interrupt */

              /* Check the SM_CANCEL bit */

              if (!(reg & VS1053_SM_CANCEL))
                {
                  /* Cancel has begun.  Switch to endmode */

                  pBuf->curbyte = pBuf->nbytes = 0;
                }
            }
#endif /* CONFIG_AUDIO_EXCLUDE_STOP */

          /* Test if we are at the end of the buffer */

          if (pBuf->curbyte >= pBuf->nbytes)
            {
              if (pBuf->nbytes != pBuf->nmaxbytes)
                {
                  /* This is the final buffer.  Get the VS1053 endfillchar */

                  dev->hw_lower->disable(dev->hw_lower);   /* Disable the DREQ interrupt */
                  (void)SPI_SETFREQUENCY(dev->spi, dev->spi_freq);
                  vs1053_writereg(dev, VS1053_SCI_WRAMADDR, VS1053_END_FILL_BYTE);
                  dev->endfillchar = vs1053_readreg(dev, VS1053_SCI_WRAM) >> 8;
                  (void)SPI_SETFREQUENCY(dev->spi, VS1053_DATA_FREQ);
                  dev->hw_lower->enable(dev->hw_lower);   /* Enable the DREQ interrupt */

                  /* Mark the device as endmode */

                  dev->endmode = TRUE;
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
                  if (dev->cancelmode)
                    {
                      /* If we are in cancel mode, then we don't dequeue the buffer
                       * or need to send another SM_CANCEL, so jump into the middle
                       * of the stop sequence.
                       */

                      dev->endfillbytes = 32*65+1;
                      continue;
                    }
                  else
#endif  /* CONFIG_AUDIO_EXCLUDE_STOP */
                    {
                      dev->endfillbytes = 0;
                    }
                }

              /* We referenced the buffer so we must free it */

              apb_free(pBuf);
#ifdef CONFIG_AUDIO_MULTI_SESSION
              dev->lower.upper(dev->lower.priv, AUDIO_CALLBACK_DEQUEUE,
                  pBuf, OK, NULL);
#else
              dev->lower.upper(dev->lower.priv, AUDIO_CALLBACK_DEQUEUE,
                  pBuf, OK);
#endif

              /* Lock the buffer queue to pop the next buffer */

              if ((ret = sem_wait(&dev->apbq_sem)) != OK)
                {
#ifdef CONFIG_AUDIO_MULTI_SESSION
                  dev->lower.upper(dev->lower.priv,
                      AUDIO_CALLBACK_IOERR, NULL, ret, NULL);
#else
                  dev->lower.upper(dev->lower.priv,
                      AUDIO_CALLBACK_IOERR, NULL, ret);
#endif
                  auddbg("I/O error!\n");

                  goto err_out;
                }

              /* Pop the next entry */

              pBuf = (struct ap_buffer_s *) dq_remfirst(&dev->apbq);
              dev->pBuf = pBuf;

              //auddbg("Next Buffer = %p, bytes = %d\n", pBuf, pBuf ? pBuf->nbytes : 0);
              if (pBuf == NULL)
                {
                  sem_post(&dev->apbq_sem);
                  break;
                }

              pSamp = &pBuf->samp[pBuf->curbyte];
              apb_reference(pBuf);                /* Add our buffer reference */
              sem_post(&dev->apbq_sem);
            }
        }
    }

  /* Deselect the SPI bus and unlock it */
err_out:
  SPI_SELECT(spi, SPIDEV_AUDIO_DATA, false);
  vs1053_spi_unlock(spi);
}

/****************************************************************************
 * Name: vs1053_dreq_isr
 *
 *  This is the ISR that services the DREQ pin from the VS1053, which
 *  indicates the chip is ready to receive additional data.  We use it to
 *  send a message to our workertherad message queue so it knows to wake
 *  up and send more data.
 *
 ****************************************************************************/

static int vs1053_dreq_isr(int irq, FAR void *context)
{
  struct vs1053_struct_s *dev = NULL;
  struct audio_msg_s      msg;

  /* Get the driver context */

#if CONFIG_VS1053_DEVICE_COUNT == 1
  dev = g_isrdata[0];       /* Simple case */
#else
  /* More complex case */
  {
    int x;

    for (x = 0; x < CONFIG_VS1053_DEVICE_COUNT; x++)
      {
        if (g_isrdata[x]->hw_lower->irq == irq)
          {
            dev = g_isrdata[x];
            break;
          }
      }

    DEBUGASSERT(dev);
  }
#endif

  /* Now create a message and send it to the workerthread */

  if (dev->running)
    {
      msg.msgId = AUDIO_MSG_DATA_REQUEST;
      mq_send(dev->mq, &msg, sizeof(msg), CONFIG_VS1053_MSG_PRIO);
    }
  else
    {
      msg.msgId = AUDIO_MSG_DATA_REQUEST;
    }

  return 0;
}

/****************************************************************************
 * Name: vs1053_workerthread
 *
 *  This is the thread that feeds data to the chip and keeps the audio
 *  stream going.
 *
 ****************************************************************************/

static void *vs1053_workerthread(pthread_addr_t pvarg)
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) pvarg;
  struct audio_msg_s      msg;
  FAR struct ap_buffer_s  *pBuf;
  int                     size, prio;
  uint16_t                reg;
  uint8_t                 timeout;

  auddbg("Entry\n");

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
  dev->cancelmode = 0;
#endif
  dev->endmode = dev->endfillbytes = 0;

  /* Fill the VS1053 FIFO with initial data.  */

  vs1053_feeddata(dev);             /* Fill the VS1053 FIFO */

  /* Wait for DREQ to go active so we can issue a READ command */

  timeout = 200;
  while (!dev->hw_lower->read_dreq(dev->hw_lower) && timeout)
    {
      usleep(100);
      timeout--;
    }

  /* Loop as long as we are supposed to be running */

  dev->running = TRUE;
  dev->hw_lower->enable(dev->hw_lower);   /* Enable the DREQ interrupt */
  while (dev->running || dev->endmode)
    {
      if (dev->hw_lower->read_dreq(dev->hw_lower))
        {
          vs1053_feeddata(dev);    /* Feed more data to the VS1053 FIFO */
        }

      /* Wait for messages from our message queue */

      size = mq_receive(dev->mq, &msg, sizeof(msg), &prio);

      /* Handle the case when we return with no message */

      if (size == 0)
        {
          /* Should we just stop running? */

          dev->running = FALSE;
          break;
        }

      /* Process the message */

      switch (msg.msgId)
        {
          /* The ISR has requested more data */

          case AUDIO_MSG_DATA_REQUEST:
            usleep(500);
            vs1053_feeddata(dev);   /* Feed more data to the VS1053 FIFO */
            break;

          /* Stop the playback */

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
          case AUDIO_MSG_STOP:
            if (!dev->hw_lower->read_dreq(dev->hw_lower))
              {
                usleep(300);
              }

            /* Send CANCEL message to VS1053 */

            dev->hw_lower->disable(dev->hw_lower);
            vs1053_spi_lock(dev->spi, dev->spi_freq);
            reg = vs1053_readreg(dev, VS1053_SCI_MODE);
            vs1053_writereg(dev, VS1053_SCI_MODE, reg | VS1053_SM_CANCEL);
            vs1053_spi_unlock(dev->spi);
            dev->hw_lower->enable(dev->hw_lower);

            /* Set cancelmode */

            dev->cancelmode = TRUE;

            break;
#endif

          /* We will wake up when a new buffer enqueued just in case */

          case AUDIO_MSG_ENQUEUE:
            break;

          default:
            break;
        }
    }

  /* Disable the DREQ interrupt */

  dev->hw_lower->disable(dev->hw_lower);

  /* Cancel any leftover buffer in our queue */

  if (sem_wait(&dev->apbq_sem) == OK)
    {
      /* Get the next buffer from the queue */

      while ((pBuf = (FAR struct ap_buffer_s *) dq_remfirst(&dev->apbq)) != NULL)
        ;
    }

  sem_post(&dev->apbq_sem);

  /* Free the active buffer */

  if (dev->pBuf != NULL)
    {
      apb_free(dev->pBuf);
      dev->pBuf = NULL;
    }

  /* Close the message queue */

  mq_close(dev->mq);
  mq_unlink(dev->mqname);
  dev->mq = NULL;

  /* Send an AUDIO_MSG_COMPLETE message to the client */

#ifdef CONFIG_AUDIO_MULTI_SESSION
  dev->lower.upper(dev->lower.priv, AUDIO_CALLBACK_COMPLETE, NULL, OK, NULL);
#else
  dev->lower.upper(dev->lower.priv, AUDIO_CALLBACK_COMPLETE, NULL, OK);
#endif

  auddbg("Exit\n");

  return NULL;
}

/****************************************************************************
 * Name: vs1053_start
 *
 * Description: Start the configured operation (audio streaming, volume
 *              enabled, etc.).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int vs1053_start(FAR struct audio_lowerhalf_s *lower, FAR void *session)
#else
static int vs1053_start(FAR struct audio_lowerhalf_s *lower)
#endif
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;
  struct mq_attr      attr;
  struct sched_param  sparam;
  pthread_attr_t      tattr;
  int                 ret;
  void                *value;

  auddbg("Entry\n");

  vs1053_spi_lock(dev->spi, dev->spi_freq);     /* Lock the device */
  auddbg("Entry HDAT1=0x%0X  HDAT0=0x%0X\n",
      vs1053_readreg(dev, VS1053_SCI_HDAT1),
      vs1053_readreg(dev, VS1053_SCI_HDAT0));
  vs1053_spi_unlock(dev->spi);

  /* Do a soft reset, just in case */

  vs1053_softreset(dev);

  /* Increase the frequency of the part during processing */

  vs1053_spi_lock(dev->spi, dev->spi_freq);     /* Lock the device */
  vs1053_setfrequency(dev, CONFIG_VS1053_MP3_DECODE_FREQ);
  auddbg("Reset HDAT1=0x%0X  HDAT0=0x%0X\n",
      vs1053_readreg(dev, VS1053_SCI_HDAT1),
      vs1053_readreg(dev, VS1053_SCI_HDAT0));
  vs1053_spi_unlock(dev->spi);

  /* Create a message queue for the worker thread */

  snprintf(dev->mqname, sizeof(dev->mqname), "/tmp/%X", dev);
  attr.mq_maxmsg = 16;
  attr.mq_msgsize = sizeof(struct audio_msg_s);
  attr.mq_curmsgs = 0;
  attr.mq_flags = 0;
  dev->mq = mq_open(dev->mqname, O_RDWR | O_CREAT, 0644, &attr);
  if (dev->mq == NULL)
    {
      /* Error creating message queue! */

      auddbg("Couldn't allocate message queue\n");
      return -ENOMEM;
    }

  /* Pop the first enqueued buffer */

  if ((ret = (sem_wait(&dev->apbq_sem)) == OK))
    {
      dev->pBuf = (FAR struct ap_buffer_s *) dq_remfirst(&dev->apbq);
      apb_reference(dev->pBuf);               /* Add our buffer reference */
      sem_post(&dev->apbq_sem);
    }
  else
    {
      auddbg("Error getting APB Queue sem\n");
      return ret;
    }

  /* Join any old worker thread we had created to prevent a memory leak */

  if (dev->threadid != 0)
    {
      auddbg("Joining old thread\n");
      pthread_join(dev->threadid, &value);
    }

  /* Start our thread for sending data to the device */

  pthread_attr_init(&tattr);
  sparam.sched_priority = sched_get_priority_max(SCHED_FIFO) - 3;
  pthread_attr_setschedparam(&tattr, &sparam);

  auddbg("Starting workerthread\n");
  ret = pthread_create(&dev->threadid, &tattr, vs1053_workerthread,
      (pthread_addr_t) dev);
  if (ret != OK)
    {
      auddbg("Can't create worker thread, errno=%d\n", errno);
    }
  else
      auddbg("Created worker thread\n");

  return ret;
}

/****************************************************************************
 * Name: vs1053_stop
 *
 * Description: Stop the configured operation (audio streaming, volume
 *              disabled, etc.).
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int vs1053_stop(FAR struct audio_lowerhalf_s *lower, FAR void* session)
#else
static int vs1053_stop(FAR struct audio_lowerhalf_s *lower)
#endif
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;
  struct audio_msg_s  term_msg;
  FAR void*           value;

  /* Send a message to stop all audio streaming */

  term_msg.msgId = AUDIO_MSG_STOP;
  term_msg.u.data = 0;
  mq_send(dev->mq, &term_msg, sizeof(term_msg), CONFIG_VS1053_MSG_PRIO);

  /* Join the worker thread */

  pthread_join(dev->threadid, &value);
  dev->threadid = 0;

  /* Reduce the decoder's operating frequency to save power */

  vs1053_spi_lock(dev->spi, dev->spi_freq);       /* Lock the device */
  vs1053_setfrequency(dev, CONFIG_VS1053_XTALI);
  vs1053_spi_unlock(dev->spi);

  /* Wait for a bit */

  up_mdelay(40);

  return OK;
}
#endif

/****************************************************************************
 * Name: vs1053_pause
 *
 * Description: Pauses the playback.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int vs1053_pause(FAR struct audio_lowerhalf_s *lower, FAR void* session)
#else
static int vs1053_pause(FAR struct audio_lowerhalf_s *lower)
#endif
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;

  if (!dev->running)
    return OK;

  /* Disable interrupts to prevent us from suppling any more data */

  dev->paused = TRUE;
  dev->hw_lower->disable(dev->hw_lower);   /* Disable the DREQ interrupt */
  return OK;
}
#endif /* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: vs1053_resume
 *
 * Description: Resuems the playback.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int vs1053_resume(FAR struct audio_lowerhalf_s *lower, FAR void* session)
#else
static int vs1053_resume(FAR struct audio_lowerhalf_s *lower)
#endif
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;

  if (!dev->running)
    return OK;

  /* Enable interrupts to allow suppling data */

  dev->paused = FALSE;
  vs1053_feeddata(dev);
  dev->hw_lower->enable(dev->hw_lower);   /* Enable the DREQ interrupt */
  return OK;
}
#endif /* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: vs1053_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/

static int vs1053_enqueuebuffer(FAR struct audio_lowerhalf_s *lower,
                 FAR struct ap_buffer_s *apb )
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;
  struct audio_msg_s  term_msg;
  int ret;

  audvdbg("Entry\n");

  /* Lock access to the apbq */

  if ((ret = sem_wait(&dev->apbq_sem)) == OK)
    {
      /* We can now safely add the buffer to the queue */

      apb->curbyte = 0;
      apb->flags = AUDIO_APB_OUTPUT_ENQUEUED;
      dq_addlast(&apb->dq_entry, &dev->apbq);
      sem_post(&dev->apbq_sem);

      /* Send a message indicating a new buffer enqueued */

      if (dev->mq != NULL)
        {
          term_msg.msgId = AUDIO_MSG_ENQUEUE;
          term_msg.u.data = 0;
          mq_send(dev->mq, &term_msg, sizeof(term_msg), CONFIG_VS1053_MSG_PRIO);
        }
    }

  return ret;
}

/****************************************************************************
 * Name: vs1053_cancelbuffer
 *
 * Description: Called when an enqueued buffer is being cancelled.
 *
 ****************************************************************************/

static int vs1053_cancelbuffer(FAR struct audio_lowerhalf_s *lower,
                 FAR struct ap_buffer_s *apb )
{
  return OK;
}

/****************************************************************************
 * Name: vs1053_ioctl
 *
 * Description: Perform a device ioctl
 *
 ****************************************************************************/

static int vs1053_ioctl(FAR struct audio_lowerhalf_s *lower, int cmd,
                  unsigned long arg)
{
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
  FAR struct ap_buffer_info_s *pBufInfo;
#endif

  /* Deal with ioctls passed from the upper-half driver */

  switch (cmd)
    {
      /* Check for AUDIOIOC_HWRESET ioctl.  This ioctl is passed straight
       * through from the upper-half audio driver.
       */

      case AUDIOIOC_HWRESET:
        vs1053_hardreset((FAR struct vs1053_struct_s *) lower);
        break;

       /* Report our preferred buffer size and quantity */

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
      case AUDIOIOC_GETBUFFERINFO:

        pBufInfo = (FAR struct ap_buffer_info_s *) arg;
        pBufInfo->buffer_size = CONFIG_VS1053_BUFFER_SIZE;
        pBufInfo->nbuffers = CONFIG_VS1053_NUM_BUFFERS;
        break;
#endif

      default:
        break;
    }

  return OK;
}

/****************************************************************************
 * Name: vs1053_reserve
 *
 * Description: Reserves a session (the only one we have).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int vs1053_reserve(FAR struct audio_lowerhalf_s *lower,
                  FAR void **psession)
#else
static int vs1053_reserve(FAR struct audio_lowerhalf_s *lower)
#endif
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;
  int   ret = OK;

  /* Borrow the APBQ semaphore for thread sync */

  if (sem_wait(&dev->apbq_sem) != OK)
    {
      return -EBUSY;
    }

  if (dev->busy)
    {
      ret = -EBUSY;
    }
  else
    {
      /* Initialize the session context.  We don't really use it. */

#ifdef CONFIG_AUDIO_MULTI_SESSION
      *psession = NULL;
#endif
      dev->busy = TRUE;
      dev->running = FALSE;
      dev->paused = FALSE;
    }

  sem_post(&dev->apbq_sem);

  return ret;
}

/****************************************************************************
 * Name: vs1053_release
 *
 * Description: Releases the session (the only one we have).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int vs1053_release(FAR struct audio_lowerhalf_s *lower,
                  FAR void *psession)
#else
static int vs1053_release(FAR struct audio_lowerhalf_s *lower)
#endif
{
  FAR struct vs1053_struct_s *dev = (struct vs1053_struct_s *) lower;
  void  *value;

  /* Join any old worker thread we had created to prevent a memory leak */

  if (dev->threadid != 0)
    {
      pthread_join(dev->threadid, &value);
      dev->threadid = 0;
    }

  /* Borrow the APBQ semaphore for thread sync */

  if (sem_wait(&dev->apbq_sem) != OK)
    {
      return -EBUSY;
    }

  /* Really we should free any queued buffers here */

  dev->busy = 0;
  sem_post(&dev->apbq_sem);

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vs1053_initialize
 *
 * Description:
 *   Initialize the VS1053 device
 *
 * Input Parameters:
 *   spidevice - This is a placeholder argument until the Audio interface
 *      has been flushed out a bit.
 *
 ****************************************************************************/

struct audio_lowerhalf_s *vs1053_initialize(FAR struct spi_dev_s *spi,
                            FAR const struct vs1053_lower_s *lower,
                            unsigned int devno)
{
  FAR struct vs1053_struct_s *dev;
  uint16_t                    status;
  uint8_t                     id;
  uint8_t                     retry;

  /* Sanity check */

  DEBUGASSERT(spi != NULL);
  DEBUGASSERT(lower != NULL);
  DEBUGASSERT(lower->reset != NULL);

  /* Allocate a VS1053 device structure */

  dev = (struct vs1053_struct_s *)kmalloc(sizeof(struct vs1053_struct_s));
  if (dev)
    {
      /* Initialize the VS1053 device structure */

      dev->lower.ops = &g_audioops;
      dev->lower.upper = NULL;
      dev->lower.priv = NULL;
      dev->hw_lower = lower;
      dev->spi_freq = CONFIG_VS1053_XTALI / 7;
      dev->spi = spi;
      dev->mq = NULL;
      dev->busy = FALSE;
      dev->threadid = 0;
      dev->running = 0;

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
      dev->volume = 250;                /* 25% volume as default */
#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
      dev->balance = 500;               /* Center balance */
#endif
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
      dev->bass = 0;
      dev->treble = 0;
#endif
      sem_init(&dev->apbq_sem, 0, 1);
      dq_init(&dev->apbq);

      /* Reset the VS1053 chip */

      lower->reset(lower, false);
      up_udelay(10);
      lower->reset(lower, true);
      up_udelay(VS1053_RST_USECS);

#if CONFIG_VS1053_XTALI == VS1053_DEFAULT_XTALI

      /* If we have a standard crystal, then wait extra time
       * for the DREQ to be active indicating the device is ready
       */

      retry = 200;;
      while (!lower->read_dreq(lower) && retry)
        {
          up_udelay(10);
          retry--;
        }
#endif

      /* Do device detection to validate the chip is there.
       * We have to hold the SPI lock during reads / writes.
       */

      vs1053_spi_lock(spi, dev->spi_freq);
      status = vs1053_readreg(dev, VS1053_SCI_STATUS);
      vs1053_spi_unlock(spi);

      /* Validate the device ID read from the chip */

      id = (status & VS1053_SS_VER) >> VS1053_VER_SHIFT;
      if (id != VS1053_VER_VS1053)
        {
          auddbg("Unexpected VER bits: 0x%0X\n", id);
          kfree(dev);
          return NULL;
        }
      else
        {
          auddbg("VS1053 Detected!\n");
        }

      /* Attach our ISR to this device */
      dev->hw_lower->attach(dev->hw_lower, vs1053_dreq_isr);

      /* Find a slot to save the device context for ISR lookup */

#if CONFIG_VS1053_DEVICE_COUNT == 1
      g_isrdata[0] = dev;         /* The simple case */
#else
      /* The more complex case */
      {
        int   x;

        /* Initialize the ISR data if not alrady */

        for (x = 0; x < CONFIG_VS1053_DEVICE_COUNT; x++)
          {
            /* Find an empty slot */

            if (g_isrdata[x] == NULL)
              {
                g_isrdata[x] = dev;
                break;
              }
          }
      }
#endif

        /* Do some initialization of the codec */

      vs1053_shutdown(&dev->lower);                   /* Go to shutdown state */
    }

  return &dev->lower;
}
