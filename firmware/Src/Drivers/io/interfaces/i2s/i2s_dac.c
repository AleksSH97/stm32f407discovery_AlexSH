/*
 * i2s_dac.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "i2s_dac.h"

I2S_HandleTypeDef hi2s3;

void I2S3Init(void)
{
    hi2s3.Instance = SPI3;
    hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s3.Init.DataFormat = I2S_DATAFORMAT_24B;
    hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_48K;
    hi2s3.Init.CPOL = I2S_CPOL_LOW;
    hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;

    if (HAL_I2S_Init(&hi2s3) != HAL_OK)
    {
      Error_Handler();
    }
}

bool I2S3TransmitDMA(I2S_HandleTypeDef *hi2s, uint16_t *buff)
{
    return HAL_I2S_Transmit_DMA(hi2s, buff, 64) == HAL_OK;
}
