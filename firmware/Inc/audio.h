/**
 ******************************************************************************
 * @file           : audio.h
 * @author         : Aleksandr Shabalin    <alexnv97@gmail.com>
 * @brief          : Header file for audio
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin ------------------ *
 ******************************************************************************
 * This module is a confidential and proprietary property of Aleksandr Shabalin
 * and possession or use of this module requires written permission
 * of Aleksandr Shabalin.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef AUDIO_H_
#define AUDIO_H_

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "dac_i2c.h"
#include "cs43l22.h"
#include "log.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************/
/* Public defines ----------------------------------------------------------- */
/******************************************************************************/
#define AUDIO_CODEC_STANDARD                0x04
#define AUDIO_I2C_ADDRESS                   0x94

  /* Audio Reset Pin definition */
#define AUDIO_RESET_GPIO_CLK_ENABLE()       __GPIOD_CLK_ENABLE()
#define AUDIO_RESET_PIN                     GPIO_PIN_4
#define AUDIO_RESET_GPIO                    GPIOD

/* Volume Levels values */
#define AUDIO_DEFAULT_VOLMIN                0x00
#define AUDIO_DEFAULT_VOLMAX                0xFF
#define AUDIO_DEFAULT_VOLSTEP               0x04

#define AUDIO_PAUSE                         0
#define AUDIO_RESUME                        1

/* Codec POWER DOWN modes */
#define AUDIO_CODEC_PDWN_HW                 1
#define AUDIO_CODEC_PDWN_SW                 2

/* MUTE commands */
#define AUDIO_MUTE_ON                       1
#define AUDIO_MUTE_OFF                      0

/* AUDIO FREQUENCY */
#define AUDIO_FREQUENCY_192K                ((uint32_t)192000)
#define AUDIO_FREQUENCY_96K                 ((uint32_t)96000)
#define AUDIO_FREQUENCY_48K                 ((uint32_t)48000)
#define AUDIO_FREQUENCY_44K                 ((uint32_t)44100)
#define AUDIO_FREQUENCY_32K                 ((uint32_t)32000)
#define AUDIO_FREQUENCY_22K                 ((uint32_t)22050)
#define AUDIO_FREQUENCY_16K                 ((uint32_t)16000)
#define AUDIO_FREQUENCY_11K                 ((uint32_t)11025)
#define AUDIO_FREQUENCY_8K                  ((uint32_t)8000)

/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/
struct audio_drv
{
    uint32_t  (*Init)(uint16_t, uint16_t, uint8_t, uint32_t);
    void      (*DeInit)(void);
    uint32_t  (*ReadID)(uint16_t);
    uint32_t  (*Play)(uint16_t, uint16_t*, uint16_t);
    uint32_t  (*Pause)(uint16_t);
    uint32_t  (*Resume)(uint16_t);
    uint32_t  (*Stop)(uint16_t, uint32_t);
    uint32_t  (*SetFrequency)(uint16_t, uint32_t);
    uint32_t  (*SetVolume)(uint16_t, uint8_t);
    uint32_t  (*SetMute)(uint16_t, uint32_t);
    uint32_t  (*SetOutputMode)(uint16_t, uint8_t);
    uint32_t  (*Reset)(uint16_t);
};

typedef enum {
    AUDIO_IDLE = 0x00,
    AUDIO_INIT,
    AUDIO_PLAY,
    AUDIO_STOP,
    AUDIO_ERROR,
} audio_status_t;

typedef enum {
    AUDIO_OK = 0x00,
    AUDIO_INIT_ERROR,
    AUDIO_PLAY_ERROR,
    AUDIO_UNDEFINED_ERROR,
} audio_error_t;

typedef enum {
    AUDIO_OUTPUT_DEVICE_SPEAKER = 0x00,
    AUDIO_OUTPUT_DEVICE_HEADPHONE,
    AUDIO_OUTPUT_DEVICE_BOTH,
    AUDIO_OUTPUT_DEVICE_AUTO,
} audio_output_device_t;

struct audio {
    lwrb_t lwrb;
    bool stop;

    audio_status_t status;
    audio_output_device_t output_device;
    audio_error_t error;
};

extern struct audio audio;
extern struct audio_drv cs43l22_drv;

/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
void AudioIoInit(void);
void AudioTask(void *argument);

void AudioSetStatus(audio_status_t status);
audio_status_t AudioGetStatus(void);

void AudioSetError(audio_error_t error);
audio_error_t AudioGetError(void);

uint8_t AudioPlay(void);

/******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#endif /* AUDIO_H_ */
