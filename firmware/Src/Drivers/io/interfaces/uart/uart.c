/**
 ******************************************************************************
 * @file           : uart.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Uart usage file
 ******************************************************************************
 * ----------------- Copyright (c) 2022 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "indication.h"
#include "uart.h"

struct uart debug_uart;
struct uart data_uart;

/**
 * @brief          Initialization of all uarts
 */
void UARTAllInit(void)
{
    //DebugUartInit();
    DataUartInit();
}
/******************************************************************************/

/**
 * @brief          Check and validate message
 */
void UARTCheckMsg(void)
{
    if (debug_uart.flag) {
        if(!debug_uart_validate_msg(&debug_uart.msg)) {
            PrintfLogsCRLF("Received incorrect msg");
            for (int i = 0; i < sizeof(struct msg); i++) {

            }
        }
        else {
            switch (debug_uart.msg.payload) {
                case UART_LED_TOGGLE:
                    HAL_GPIO_TogglePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin);
                    break;
                case UART_BLINK_LED:
                    break; // TODO implement command
                default:
                    PrintfLogsCRLF("Received incorrect command 0x%x", debug_uart.msg.payload);
                    break;
            }
        }
        debug_uart.flag = false;
        memset(&debug_uart.msg, 0x00, sizeof(struct msg));
        uart_setup_receive_msg_dma(&huart2, &debug_uart.msg);
    }
}
/******************************************************************************/

/**
 * @brief          Enable interrupt receiving message
 */
bool UARTSetupReceiveMsg(UART_HandleTypeDef *huart, struct msg *msg_ptr)
{
    return HAL_UART_Receive_IT(huart, (uint8_t*)msg_ptr, sizeof(msg_ptr)) == HAL_OK;
}

bool UARTSetupReceiveByte(UART_HandleTypeDef *huart, uint8_t *byte)
{
    return HAL_UART_Receive_IT(huart, (uint8_t*)byte, sizeof(byte)) == HAL_OK;
}

bool UARTSetupReceiveChar(UART_HandleTypeDef *huart, uint8_t *byte)
{
    return HAL_UART_Receive_IT(huart, (uint8_t*)byte, sizeof(char)) == HAL_OK;
}
/******************************************************************************/




void UARTSendPacket(void)
{
    msg = DataUartCreateMsg(UART_LED_TOGGLE);
    if (!UARTSendMsg(&huart3, &msg)) {
        PrintfLogsCRLF("Sending msg error!");
        IndicationLedError();
    }
    PrintfLogsCRLF("Here's your msg: %d %d %d", msg.start_byte, msg.payload, msg.stop_byte);
}
/******************************************************************************/




/**
 * @brief          Blocking USART2 send/receive functions
 */
bool UARTSendMsg(UART_HandleTypeDef *huart, struct msg *msg_ptr)
{
    return HAL_UART_Transmit(huart, (uint8_t*)msg_ptr, sizeof(msg_ptr), HAL_MAX_DELAY) == HAL_OK;
}

bool UARTSendByte(UART_HandleTypeDef *huart, uint8_t byte)
{
    return HAL_UART_Transmit(huart, &byte, sizeof(byte), HAL_MAX_DELAY) == HAL_OK;
}

bool UARTReceiveByte(UART_HandleTypeDef *huart, uint8_t byte)
{
    return HAL_UART_Receive(huart, &byte, sizeof(byte), HAL_MAX_DELAY) == HAL_OK;
}
/******************************************************************************/



bool UARTSendByteTxBuff(UART_HandleTypeDef *huart)
{
    uint8_t msg;

    lwrb_read(&data_uart.lwrb_tx, &msg, sizeof(char));

    return HAL_UART_Transmit(huart, &msg, sizeof(uint8_t), HAL_MAX_DELAY) == HAL_OK;
}
/******************************************************************************/

/**
 * @brief          Interrupt callbacks
 *
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        IndicationLedTop();
        IoSystemPutDataToRxBuffer(&data_uart.keyboarb_input, sizeof(uint8_t));
        data_uart.flag = true;
        UARTSetupReceiveChar(&huart3, &data_uart.keyboarb_input);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}
