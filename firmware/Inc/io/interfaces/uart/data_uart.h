/**
 ******************************************************************************
 * @file           : data_uart.h
 * @author         : Aleksandr Shabalin    <alexnv97@gmail.com>
 * @brief          : Header file of UART3
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin ------------------ *
 ******************************************************************************
 * This module is a confidential and proprietary property of Aleksandr Shabalin
 * and possession or use of this module requires written permission
 * of Aleksandr Shabalin.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DATA_UART_H_
#define DATA_UART_H_


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
#define DATA_TX_Pin          GPIO_PIN_10
#define DATA_RX_Pin          GPIO_PIN_11


/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/
extern UART_HandleTypeDef huart3;
extern struct uart data_uart;

enum uart_commands {
    UART_BLINK_LED    = 0x02,
    UART_LED_TOGGLE   = 0x03,
};


/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
void DataUartInit(void);
struct msg DataUartCreateMsg(enum uart_commands command);


/******************************************************************************/


#ifdef __cplusplus
}
#endif


#endif /* DATA_UART_H_ */
