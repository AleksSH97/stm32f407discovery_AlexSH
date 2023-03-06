/**
 ******************************************************************************
 * @file           : i2s_microphone.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : I2S2 (microphone) hardware
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "i2s_microphone.h"




/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
I2S_HandleTypeDef hi2s2;

struct microphone microphone;

/******************************************************************************/


/**
 * @brief           I2S2 init
 */
void I2S2Init(void)
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
/******************************************************************************/




/**
 * @brief          I2S2 receive DMA
 */
bool I2S2ReceiveDMA(I2S_HandleTypeDef *hi2s, uint16_t *buff)
{
    return HAL_I2S_Receive_DMA(hi2s, buff, 64) == HAL_OK;
}
/******************************************************************************/




/**
 * @brief          I2S2 RX half cplt callback
 */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    MicrophoneSetStatus(MICROPHONE_RX_STATE_1);
}
/******************************************************************************/




/**
 * @brief          I2S2 RX cplt callback
 */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    MicrophoneSetStatus(MICROPHONE_RX_STATE_2);
}
/******************************************************************************/



/**
 * @brief          I2S3 TX half cplt callback
 */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    MicrophoneSetStatus(MICROPHONE_TX_STATE_1);
}
/******************************************************************************/



/**
 * @brief          I2S3 TX cplt callback
 */
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    MicrophoneSetStatus(MICROPHONE_TX_STATE_2);
}
/******************************************************************************/
