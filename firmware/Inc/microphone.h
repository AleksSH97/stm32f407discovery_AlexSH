/*
 * microphone.h
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#ifndef MICROPHONE_H_
#define MICROPHONE_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "i2s_microphone.h"

typedef enum {
    MICROPHONE_OK            = 0x00,
    MICROPHONE_INIT_ERROR    = 0x01,
    MICROPHONE_PROCESS_ERROR = 0x02,
} microphone_status_t;

typedef struct {
    microphone_status_t status;
    bool started;
} microphone_t;

extern microphone_status_t microphone_status;

void microphone_init(void);

#endif /* MICROPHONE_H_ */
