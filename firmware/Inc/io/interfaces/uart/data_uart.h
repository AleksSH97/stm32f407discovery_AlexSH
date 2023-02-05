/*
 * data_uart.h
 *
 *  Created on: Dec 27, 2022
 *      Author: АлексанDOOR
 */

#ifndef DATA_UART_H_
#define DATA_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "log.h"
#include "uart.h"

#define DATA_TX_Pin       GPIO_PIN_10
#define DATA_RX_Pin       GPIO_PIN_11
#define DATA_GPIO_Port    GPIOB

extern UART_HandleTypeDef huart3;
extern struct uart data_uart;

enum uart_commands {
    UART_BLINK_LED    = 0x02,
    UART_LED_TOGGLE   = 0x03,
};

void data_uart_init(void);
struct msg data_uart_create_msg(enum uart_commands command);

#ifdef __cplusplus
}
#endif

#endif /* DATA_UART_H_ */
