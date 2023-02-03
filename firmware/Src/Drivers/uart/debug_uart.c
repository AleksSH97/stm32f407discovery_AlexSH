/*
 * debug_usart.c
 *
 *  Created on: Dec 27, 2022
 *      Author: АлексанDOOR
 */

#include "debug_uart.h"

UART_HandleTypeDef    huart2;

/**
 * @brief          Debug uart init function
 */
void debug_uart_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin          = DEBUG_TX_Pin | DEBUG_RX_PIN;
    GPIO_InitStruct.Mode         = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull         = GPIO_NOPULL;
    GPIO_InitStruct.Speed        = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate    = GPIO_AF7_USART2;
    HAL_GPIO_Init(DEBUG_GPIO_Port, &GPIO_InitStruct);

    __HAL_RCC_USART2_CLK_ENABLE();

    HAL_NVIC_SetPriority(USART2_IRQn, 0x00, 0x00);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = 115200;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
      indication_led_error();
      Error_Handler();
    }
}
/******************************************************************************/

/**
 * @brief          Validate message debug uart
 */
bool debug_uart_validate_msg(struct msg *msg_ptr)
{
    if (msg_ptr->start_byte != START_BYTE) {
        return false;
    }

    if (msg_ptr->stop_byte != STOP_BYTE) {
        return false;
    }

    return true;
}
/******************************************************************************/
