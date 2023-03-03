/*
 * uart.h
 *
 *  Created on: Dec 27, 2022
 *      Author: АлексанDOOR
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "debug_uart.h"
#include "data_uart.h"
#include "lwrb.h"

#define START_BYTE    0x3A
#define STOP_BYTE     0x68

extern struct uart debug_uart;
extern struct uart data_uart;
extern struct msg msg;

#define UART_BUFF_SIZE    8u

struct __attribute__ ((__packed__)) msg {
    uint8_t start_byte;
    uint8_t payload;
    uint8_t stop_byte;
};

struct uart {
    lwrb_t        lwrb_rx;
    lwrb_t        lwrb_tx;

    uint8_t       buff_rx[UART_BUFF_SIZE];
    uint8_t       buff_tx[UART_BUFF_SIZE];

    uint8_t       keyboarb_input;
    uint8_t       console_input;

    struct msg    msg;
    volatile bool flag;
};

void UARTAllInit(void);
void UARTCheckMsg(void);
void UARTSendPacket(void);
bool UARTSetupReceiveMsg(UART_HandleTypeDef *huart, struct msg *msg_ptr);
bool UARTSetupReceiveByte(UART_HandleTypeDef *huart, uint8_t *byte);
bool UARTSetupReceiveChar(UART_HandleTypeDef *huart, uint8_t *byte);
bool UARTSendByte(UART_HandleTypeDef *huart, uint8_t byte);
bool UARTSendByteTxBuff(UART_HandleTypeDef *huart);
bool UARTReceiveByte(UART_HandleTypeDef *huart,uint8_t byte);
bool UARTSendMsg(UART_HandleTypeDef *huart, struct msg *msg_ptr);

#endif /* UART_H_ */
