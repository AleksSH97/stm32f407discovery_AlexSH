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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************/
/* Public defines ----------------------------------------------------------- */
/******************************************************************************/
#define AUDIO_CODEC_STANDARD                0x04
#define AUDIO_I2C_ADDRESS                     0x94

  /* Audio Reset Pin definition */
#define AUDIO_RESET_GPIO_CLK_ENABLE()         __GPIOD_CLK_ENABLE()
#define AUDIO_RESET_PIN                       GPIO_PIN_4
#define AUDIO_RESET_GPIO                      GPIOD

/* Codec output DEVICE */
#define AUDIO_OUTPUT_DEVICE_SPEAKER         1
#define AUDIO_OUTPUT_DEVICE_HEADPHONE       2
#define AUDIO_OUTPUT_DEVICE_BOTH            3
#define AUDIO_OUTPUT_DEVICE_AUTO            4

/* Volume Levels values */
#define AUDIO_DEFAULT_VOLMIN                0x00
#define AUDIO_DEFAULT_VOLMAX                0xFF
#define AUDIO_DEFAULT_VOLSTEP               0x04

#define AUDIO_PAUSE                   0
#define AUDIO_RESUME                  1

/* Codec POWER DOWN modes */
#define AUDIO_CODEC_PDWN_HW                 1
#define AUDIO_CODEC_PDWN_SW                 2

/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0

/* AUDIO FREQUENCY */
#define AUDIO_FREQUENCY_192K          ((uint32_t)192000)
#define AUDIO_FREQUENCY_96K           ((uint32_t)96000)
#define AUDIO_FREQUENCY_48K           ((uint32_t)48000)
#define AUDIO_FREQUENCY_44K           ((uint32_t)44100)
#define AUDIO_FREQUENCY_32K           ((uint32_t)32000)
#define AUDIO_FREQUENCY_22K           ((uint32_t)22050)
#define AUDIO_FREQUENCY_16K           ((uint32_t)16000)
#define AUDIO_FREQUENCY_11K           ((uint32_t)11025)
#define AUDIO_FREQUENCY_8K            ((uint32_t)8000)

/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/


/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
void AudioIoInit(void);

/******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#endif /* AUDIO_H_ */
