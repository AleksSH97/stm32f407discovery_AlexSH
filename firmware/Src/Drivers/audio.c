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
