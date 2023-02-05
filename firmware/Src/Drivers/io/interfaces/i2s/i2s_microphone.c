/*
 * i2s_microphone.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "i2s_microphone.h"

static void i2s2_init(void);
static void i2s2_msp_init(void);

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;

void i2s_microphone_init(void)
{
    i2s2_init();
}

void i2s2_msp_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
    PeriphClkInitStruct.PLLI2S.PLLI2SN       = 192;
    PeriphClkInitStruct.PLLI2S.PLLI2SR       = 2;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
      Error_Handler();
    }

    /* I2S2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2S2 GPIO Configuration
    PC3      ------> I2S2_SD
    PB12     ------> I2S2_WS
    PB13     ------> I2S2_CK
    */
    GPIO_InitStruct.Pin       = GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2S2 DMA Init */
    /* SPI2_RX Init */
    hdma_spi2_rx.Instance                 = DMA1_Stream3;
    hdma_spi2_rx.Init.Channel             = DMA_CHANNEL_0;
    hdma_spi2_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_spi2_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_spi2_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi2_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hdma_spi2_rx.Init.Mode                = DMA_CIRCULAR;
    hdma_spi2_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_spi2_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_spi2_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_spi2_rx.Init.MemBurst            = DMA_MBURST_SINGLE;
    hdma_spi2_rx.Init.PeriphBurst         = DMA_MBURST_SINGLE;

    if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK) {
      Error_Handler();
    }

    __HAL_LINKDMA(&hi2s2,hdmarx,hdma_spi2_rx);

    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
}

void i2s2_init(void)
{
    __HAL_I2S_DISABLE(&hi2s2);

    hi2s2.Instance            = SPI2;
    hi2s2.Init.Mode           = I2S_MODE_SLAVE_RX;
    hi2s2.Init.Standard       = I2S_STANDARD_PHILIPS;
    hi2s2.Init.DataFormat     = I2S_DATAFORMAT_16B;
    hi2s2.Init.MCLKOutput     = I2S_MCLKOUTPUT_DISABLE;
    hi2s2.Init.AudioFreq      = I2S_AUDIOFREQ_48K;
    hi2s2.Init.CPOL           = I2S_CPOL_HIGH;
    hi2s2.Init.ClockSource    = I2S_CLOCK_PLL;
    hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;

    if (HAL_I2S_Init(&hi2s2) != HAL_OK) {
      Error_Handler();
    }

    i2s2_msp_init();
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    microphone_half_transfer_callback(hi2s);
}










