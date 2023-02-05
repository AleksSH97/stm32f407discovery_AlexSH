/*
 * data_uart.c
 *
 *  Created on: Dec 27, 2022
 *      Author: АлексанDOOR
 */

#include "data_uart.h"

UART_HandleTypeDef huart3;

/**
 * @brief          Data uart init function
 */
void data_uart_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin          = DATA_TX_Pin|DATA_RX_Pin;
    GPIO_InitStruct.Mode         = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull         = GPIO_NOPULL;
    GPIO_InitStruct.Speed        = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate    = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_USART3_CLK_ENABLE();

    HAL_NVIC_SetPriority(USART3_IRQn, 0x01, 0x00);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    huart3.Instance          = USART3;
    huart3.Init.BaudRate     = 115200;
    huart3.Init.WordLength   = UART_WORDLENGTH_8B;
    huart3.Init.StopBits     = UART_STOPBITS_1;
    huart3.Init.Parity       = UART_PARITY_NONE;
    huart3.Init.Mode         = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
      indication_led_error();
      Error_Handler();
    }
}
/******************************************************************************/

/**
 * @brief          Create message for data uart to send
 */
struct msg data_uart_create_msg(enum uart_commands command)
{
    struct msg msg = {0};
    msg.start_byte = START_BYTE;
    msg.payload = command;
    msg.stop_byte = STOP_BYTE;

    printf("msg have been created: %d %d %d\r\n", msg.start_byte, msg.payload, msg.stop_byte);

    return msg;
}
/******************************************************************************/
