/*
 * io_system.h
 *
 *  Created on: 26 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#ifndef IO_LOGGING_IO_SYSTEM_H_
#define IO_LOGGING_IO_SYSTEM_H_

#include <stdarg.h>

#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "log.h"
#include "FreeRTOS.h"
#include "task.h"


#include "lwprintf/lwprintf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    IO_IDLE = 0x00,
    IO_LOGS = 0x01,
    IO_CONSOLE = 0x02,
} IOSYS_MODE;

typedef void (*rx_handler_fn)(char rx);

typedef struct {
    rx_handler_fn rx_handler;
    IOSYS_MODE mode;
    osTimerId_t  soft_timeout;
} IO_SYSTEM;

void io_set_mode(IOSYS_MODE mode);
void io_init(void);
IOSYS_MODE io_get_mode(void);
void io_clear_rx_queue(void);
bool io_get_byte(uint8_t *data, uint32_t timeout_ms);
void IO_TxTask(void *argument);
void IO_RxTask(void *argument);

#endif /* IO_LOGGING_IO_SYSTEM_H_ */
