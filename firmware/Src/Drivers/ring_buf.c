/*
 * ring_buf.c
 *
 *  Created on: Jan 16, 2023
 *      Author: АлексанDOOR
 */

// EVT - EVENT

#include "ring_buf.h"

void ring_buf_init()
{
    lwrb_init(&data_uart.lwrb, data_uart.buff, sizeof(data_uart.buff));

    if (!lwrb_is_ready(&data_uart.lwrb)) {
        log_printf_crlf("Error ring buf init");
    }

    lwrb_set_evt_fn(&data_uart.lwrb, ring_buf_evt_callback);
}

void ring_buf_write(struct uart *self, const void *data, size_t num_byte_to_write)
{
    lwrb_write(&self->lwrb, data, num_byte_to_write);
}

void ring_buf_read(struct uart *self, void *data, size_t num_byte_to_read)
{
    lwrb_read(&self->lwrb, data, num_byte_to_read);
}

void ring_buf_free(struct uart *self)
{
    lwrb_free(&self->lwrb);
}

bool ring_buf_ready(struct uart *self)
{
    return lwrb_is_ready(&self->lwrb);
}

void ring_buf_peek(const struct uart *self, size_t skip_count, void *data, size_t byte_to_peek)
{
    lwrb_peek(&self->lwrb, skip_count, data, byte_to_peek);
}

void ring_buf_evt_callback(struct uart *self, lwrb_evt_type_t evt, size_t bp)
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
