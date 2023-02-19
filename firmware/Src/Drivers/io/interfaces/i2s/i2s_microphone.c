/*
 * i2s_microphone.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "i2s_microphone.h"

I2S_HandleTypeDef hi2s2;

struct microphone microphone;

void i2s2_init(void)
{
    hi2s2.Instance = SPI2;
    hi2s2.Init.Mode = I2S_MODE_SLAVE_RX;
    hi2s2.Init.Standard = I2S_STANDARD_MSB;
    hi2s2.Init.DataFormat = I2S_DATAFORMAT_24B;
    hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_48K;
    hi2s2.Init.CPOL = I2S_CPOL_LOW;
    hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;

    if (HAL_I2S_Init(&hi2s2) != HAL_OK) {
      Error_Handler();
    }
}

bool i2s2_receive_dma(I2S_HandleTypeDef *hi2s, uint16_t *buff)
{
    return HAL_I2S_Receive_DMA(hi2s, buff, 64) == HAL_OK;
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    microphone.state = MICROPHONE_RX_STATE_1;
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    microphone.state = MICROPHONE_RX_STATE_2;
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    microphone.state = MICROPHONE_TX_STATE_1;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    microphone.state = MICROPHONE_TX_STATE_2;
}






