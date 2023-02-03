/*
 * ring_buf.h
 *
 *  Created on: Jan 16, 2023
 *      Author: АлексанDOOR
 */

#ifndef RING_BUF_H_
#define RING_BUF_H_

#include "lwrb.h"
#include "uart.h"

void ring_buf_init();
void ring_buf_write(struct uart *self, const void *data, size_t num_byte_to_write);
void ring_buf_read(struct uart *self, void *data, size_t num_byte_to_read);
void ring_buf_evt_callback(struct uart *self, lwrb_evt_type_t evt, size_t bp);
void ring_buf_free(struct uart *self);
bool ring_buf_ready(struct uart *self);
void ring_buf_peek(const struct uart *self, size_t skip_count, void *data, size_t byte_to_peek);

#endif /* RING_BUF_H_ */
