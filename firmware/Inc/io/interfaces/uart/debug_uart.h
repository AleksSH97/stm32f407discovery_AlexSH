/**
 ******************************************************************************
 * @file           : debug_uart.h
 * @author         : Aleksandr Shabalin    <alexnv97@gmail.com>
 * @brief          : Header file of UART2
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin ------------------ *
 ******************************************************************************
 * This module is a confidential and proprietary property of Aleksandr Shabalin
 * and possession or use of this module requires written permission
 * of Aleksandr Shabalin.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUG_UART_H_
#define DEBUG_UART_H_


/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "log.h"
#include "uart.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/* Public defines ----------------------------------------------------------- */
/******************************************************************************/
#define DEBUG_RX_PIN          GPIO_PIN_3
#define DEBUG_TX_Pin          GPIO_PIN_2
#define DEBUG_GPIO_Port       GPIOA


/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/
extern struct uart debug_uart;
extern struct msg msg;

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;


/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
void debug_uart_init(void);
bool debug_uart_validate_msg(struct msg *msg_ptr);


/******************************************************************************/


#ifdef __cplusplus
}
#endif


#endif /* DEBUG_UART_H_ */
