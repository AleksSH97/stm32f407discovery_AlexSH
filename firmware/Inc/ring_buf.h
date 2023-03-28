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

void RingBufUARTInit(void);
void RingBufMicrophoneInit(void);
void RingBufAcceleroInit(void);
void RingBufEvtCallback(struct uart *self, lwrb_evt_type_t evt, size_t bp);

#endif /* RING_BUF_H_ */
