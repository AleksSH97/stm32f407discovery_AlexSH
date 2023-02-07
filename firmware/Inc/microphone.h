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
#include "lwrb.h"
#include "log.h"
#include "pdm2pcm.h"

#define I2S2_BUFF_SIZE    128

extern struct microphone microphone;
extern CRC_HandleTypeDef hcrc;

typedef enum {
    MICROPHONE_OK            = 0x00,
    MICROPHONE_RX_STATE_1    = 0x01,
    MICROPHONE_RX_STATE_2    = 0x02,
    MICROPHONE_INIT_ERROR    = 0x04,
    MICROPHONE_PROCESS_ERROR = 0x08,
} microphone_status_t;

struct microphone {
    lwrb_t   lwrb;
    uint16_t buff[I2S2_BUFF_SIZE];

    microphone_status_t state;
    bool started;
};

void microphone_init(void);

#endif /* MICROPHONE_H_ */
