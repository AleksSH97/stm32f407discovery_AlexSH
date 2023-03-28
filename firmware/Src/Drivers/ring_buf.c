/*
 * ring_buf.c
 *
 *  Created on: Jan 16, 2023
 *      Author: АлексанDOOR
 */

// EVT - EVENT

#include "ring_buf.h"

void RingBufUARTInit(void)
{
    lwrb_init(&data_uart.lwrb_rx, data_uart.buff_rx, sizeof(data_uart.buff_rx));
    lwrb_init(&data_uart.lwrb_tx, data_uart.buff_tx, sizeof(data_uart.buff_tx));

    if (!lwrb_is_ready(&data_uart.lwrb_rx)) {
        PrintfLogsCRLF("Error ring buf uart_rx init");
    }

    if (!lwrb_is_ready(&data_uart.lwrb_tx)) {
        PrintfLogsCRLF("Error ring buf uart_tx init");
    }

    lwrb_set_evt_fn(&data_uart.lwrb_rx, RingBufEvtCallback);
    lwrb_set_evt_fn(&data_uart.lwrb_tx, RingBufEvtCallback);
}

void RingBufAcceleroInit(void)
{
    lwrb_init(&accelero_spi.lwrb_rx, accelero_spi.buff_rx, sizeof(accelero_spi.buff_rx));
    lwrb_init(&accelero_spi.lwrb_tx, accelero_spi.buff_tx, sizeof(accelero_spi.buff_tx));

    if (!lwrb_is_ready(&accelero_spi.lwrb_rx)) {
        PrintfLogsCRLF("Error ring buf SPI rx init");
    }
    if (!lwrb_is_ready(&accelero_spi.lwrb_tx)) {
        PrintfLogsCRLF("Error ring buf SPI tx init");
    }


    lwrb_set_evt_fn(&accelero_spi.lwrb_rx, RingBufEvtCallback);
    lwrb_set_evt_fn(&accelero_spi.lwrb_tx, RingBufEvtCallback);
}




void RingBufMicrophoneInit(void)
{
    lwrb_init(&microphone.lwrb_rx, microphone.buff_rx, sizeof(microphone.buff_rx));
    lwrb_init(&microphone.lwrb_tx, microphone.buff_tx, sizeof(microphone.buff_tx));

    if (!lwrb_is_ready(&microphone.lwrb_rx)) {
        PrintfLogsCRLF("Error ring buf i2s2 rx init");
    }
    if (!lwrb_is_ready(&microphone.lwrb_tx)) {
        PrintfLogsCRLF("Error ring buf i2s2 tx init");
    }


    lwrb_set_evt_fn(&microphone.lwrb_rx, RingBufEvtCallback);
    lwrb_set_evt_fn(&microphone.lwrb_tx, RingBufEvtCallback);
}

void RingBufEvtCallback(struct uart *self, lwrb_evt_type_t evt, size_t bp)
{
    switch(evt) {
        case LWRB_EVT_READ:
           // log_printf_crlf("Read %d bytes from ring buff event!", (int)bp);
            break;
        case LWRB_EVT_WRITE:
           // log_printf_crlf("Write %d bytes from ring buff event!", (int)bp);
            break;
        case LWRB_EVT_RESET:
            //log_printf_crlf("Reset %d bytes from ring buff event!", (int)bp);
            break;
    }
}
