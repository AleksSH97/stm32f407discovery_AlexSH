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
#include "i2s_dac.h"
#include "lwrb.h"
#include "log.h"
#include "pdm2pcm.h"

#define I2S2_BUFF_SIZE         128
#define I2S2_HALF_BUFF_SIZE    64
#define I2S2_MID_BUFF_SIZE     16

#define I2S2_FIFO_BUFF_SIZE    256

extern struct microphone microphone;
extern CRC_HandleTypeDef hcrc;

struct fifo_buff {
    uint16_t buff[I2S2_FIFO_BUFF_SIZE];
    uint8_t  w_ptr;
    uint8_t  r_ptr;
};

typedef enum {
    MICROPHONE_READY         = 0x00,
    MICROPHONE_RX_STATE_1    = 0x01,
    MICROPHONE_RX_STATE_2    = 0x02,
    MICROPHONE_TX_STATE_1    = 0x03,
    MICROPHONE_TX_STATE_2    = 0x04,
    MICROPHONE_INIT_ERROR    = 0x05,
    MICROPHONE_PROCESS_ERROR = 0x06,
} microphone_status_t;

struct microphone {
    lwrb_t   lwrb;
    uint16_t buff[I2S2_BUFF_SIZE];
    uint16_t record[I2S2_BUFF_SIZE];
    uint16_t mid_buff[I2S2_MID_BUFF_SIZE];
    uint16_t tx_buff[I2S2_BUFF_SIZE];

    microphone_status_t state;
    struct fifo_buff fifo;

    bool visualize;
    bool read;
};

void microphone_init(void);
uint16_t microphone_fifo_read(void);
void microphone_process(void);
void MicrophoneTaskStart(void *argument);

#endif /* MICROPHONE_H_ */
