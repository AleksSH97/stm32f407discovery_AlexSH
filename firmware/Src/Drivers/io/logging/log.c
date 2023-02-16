/*
 * log.c
 *
 *  Created on: 20 дек. 2022 г.
 *      Author: АлексанDOOR
 */

#include "log.h"
#include "uart.h"
#include "lwprintf/lwprintf.h"

static int lwprintf_logs_out(int ch, lwprintf_t* p);




void LogsTaskStart(void *argument)
{
    uint8_t ddd[128] = {0};
    UNUSED(ddd);

    for (;;)
    {
        if (lwrb_get_full(&data_uart.lwrb) != 0) {
            lwrb_read(&data_uart.lwrb, &data_uart.console_input, sizeof(char));
            console_insert_char(data_uart.console_input);
        }
        osDelay(10);
    }
}
/******************************************************************************/




void log_init(void)
{
    lwprintf_init(lwprintf_logs_out);
}
/******************************************************************************/




int log_printf(const char *fmt, ...)
{
    va_list args;
    int len;

    va_start(args, fmt);
    len = lwprintf_vprintf(fmt, args);
    va_end(args);

    return len;
}
/******************************************************************************/




static int lwprintf_logs_out(int ch, lwprintf_t* p)
{
    uint8_t c = (uint8_t)ch;

    if (c == '\0') {
        return ch;           //to prevent printing '0' in the end of any (char*)
    }

    return uart_send_byte(&huart3, ch);
}
/******************************************************************************/




void log_print_welcome_msg(void)
{
    log_printf_crlf("Welcome to STM32F407 Discovery firmware with FreeRTOS");
}
/******************************************************************************/

