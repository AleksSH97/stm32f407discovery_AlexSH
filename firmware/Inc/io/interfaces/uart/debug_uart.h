/*
 * debug_uart.h
 *
 *  Created on: Dec 27, 2022
 *      Author: АлексанDOOR
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUG_UART_H_
#define DEBUG_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "log.h"
#include "uart.h"

#define DEBUG_RX_PIN          GPIO_PIN_3
#define DEBUG_TX_Pin          GPIO_PIN_2
#define DEBUG_GPIO_Port       GPIOA

extern struct uart debug_uart;
extern struct msg msg;

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

void debug_uart_init(void);
bool debug_uart_validate_msg(struct msg *msg_ptr);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_UART_H_ */
