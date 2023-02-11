/*
 * i2s_microphone.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "i2s_microphone.h"

static bool i2s2_receive_dma(I2S_HandleTypeDef *hi2s, uint16_t *buff);

//I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;

struct microphone microphone;

//void i2s_microphone_init(void)
//{
//    i2s2_init();
//}

//void i2s_setup_receive_dma(void)
//{
//    i2s2_receive_dma(&hi2s2, &microphone.buff[0]);
//
//    if (!) {
//        microphone.state == MICROPHONE_INIT_ERROR;
//    }
//}

void HAL_I2S_MspInit(I2S_HandleTypeDef* hi2s)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
    PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_SPI2_CLK_ENABLE();

    hdma_spi2_rx.Instance = DMA1_Stream3;
    hdma_spi2_rx.Init.Channel = DMA_CHANNEL_0;
    hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_spi2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

    if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hi2s, hdmarx, hdma_spi2_rx);
}

//bool i2s2_receive_dma(I2S_HandleTypeDef *hi2s, uint16_t *buff)
//{
//    return HAL_I2S_Receive_DMA(hi2s, buff, 64) == HAL_OK;
//}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    indication_led();
    microphone.state = MICROPHONE_RX_STATE_1;
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    indication_led();
    microphone.state = MICROPHONE_RX_STATE_2;
}










