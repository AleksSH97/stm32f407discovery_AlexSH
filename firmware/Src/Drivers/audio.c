/**
 ******************************************************************************
 * @file           : audio.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Driver for audio.
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "audio.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
struct audio_drv cs43l22_drv =
{
  Cs43l22Init,
  Cs43l22DeInit,
  Cs43l22ReadID,

  Cs43l22Play,
  Cs43l22Pause,
  Cs43l22Resume,
  Cs43l22Stop,

  Cs43l22SetFrequency,
  Cs43l22SetMute,
  Cs43l22SetMute,
  Cs43l22SetOutputMode,
  Cs43l22Reset,
};


/******************************************************************************/


void AudioTask(void *argument)
{
    PrintfLogsCRLF(CLR_GR"AUDIO INIT..."CLR_DEF);

    AudioIoInit();

    for (;;) {

    }
}
/******************************************************************************/




void AudioIoInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable Reset GPIO Clock */
    AUDIO_RESET_GPIO_CLK_ENABLE();

    /* Audio reset pin configuration */
    GPIO_InitStruct.Pin = AUDIO_RESET_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(AUDIO_RESET_GPIO, &GPIO_InitStruct);

    DacI2cInit();

    /* Power Down the codec */
    HAL_GPIO_WritePin(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, GPIO_PIN_RESET);

    /* Wait for a delay to insure registers erasing */
    HAL_Delay(5);

    /* Power on the codec */
    HAL_GPIO_WritePin(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, GPIO_PIN_SET);

    /* Wait for a delay to insure registers erasing */
    HAL_Delay(5);
}
/******************************************************************************/




