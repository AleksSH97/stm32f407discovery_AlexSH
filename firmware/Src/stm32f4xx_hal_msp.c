/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */


#include "main.h"

DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi3_tx;

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_I2S_MspInit(I2S_HandleTypeDef* i2sHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (i2sHandle->Instance==SPI2) {
      __HAL_RCC_SPI2_CLK_ENABLE();

      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      /**I2S2 GPIO Configuration
      PC3     ------> I2S2_SD
      PB10     ------> I2S2_CK
      PB12     ------> I2S2_WS
      */
      GPIO_InitStruct.Pin = GPIO_PIN_3;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
      if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK) {
        Error_Handler();
      }

      __HAL_LINKDMA(i2sHandle,hdmarx,hdma_spi2_rx);

    }
    else if(i2sHandle->Instance==SPI3) {
      __HAL_RCC_SPI3_CLK_ENABLE();

      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      /**I2S3 GPIO Configuration
      PA4     ------> I2S3_WS
      PC7     ------> I2S3_MCK
      PC12    ------> I2S3_SD
      PB3     ------> I2S3_CK
      */
      GPIO_InitStruct.Pin = GPIO_PIN_4;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_7;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_3;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

      hdma_spi3_tx.Instance = DMA1_Stream5;
      hdma_spi3_tx.Init.Channel = DMA_CHANNEL_0;
      hdma_spi3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
      hdma_spi3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_spi3_tx.Init.MemInc = DMA_MINC_ENABLE;
      hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      hdma_spi3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
      hdma_spi3_tx.Init.Mode = DMA_CIRCULAR;
      hdma_spi3_tx.Init.Priority = DMA_PRIORITY_LOW;
      hdma_spi3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

      if (HAL_DMA_Init(&hdma_spi3_tx) != HAL_OK)
      {
        Error_Handler();
      }

      __HAL_LINKDMA(i2sHandle,hdmatx,hdma_spi3_tx);
    }
}









