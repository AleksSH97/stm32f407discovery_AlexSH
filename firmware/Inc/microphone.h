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

#define MICROPHONE_BUFF_SIZE          128
#define MICROPHONE_HALF_BUFF_SIZE     64
#define MICROPHONE_MID_BUFF_SIZE      16

#define MICROPHONE_FIFO_BUFF_SIZE     256

#define MICROPHONE_TIMEOUT_MS       1000

extern struct microphone microphone;
extern CRC_HandleTypeDef hcrc;

typedef enum {
    MICROPHONE_READY         = 0x00,
    MICROPHONE_RX_STATE_1    = 0x01,
    MICROPHONE_RX_STATE_2    = 0x02,
    MICROPHONE_TX_STATE_1    = 0x03,
    MICROPHONE_TX_STATE_2    = 0x04,
    MICROPHONE_INIT_ERROR    = 0x05,
    MICROPHONE_PROCESS_ERROR = 0x06,
} microphone_status_t;

struct fifo_buff {
    uint16_t buff[MICROPHONE_FIFO_BUFF_SIZE];
    uint8_t  w_ptr;
    uint8_t  r_ptr;
};

struct microphone {
    lwrb_t   lwrb;
    uint16_t buff[MICROPHONE_BUFF_SIZE];
    uint16_t record[MICROPHONE_BUFF_SIZE];
    uint16_t mid_buff[MICROPHONE_MID_BUFF_SIZE];
    uint16_t tx_buff[MICROPHONE_BUFF_SIZE];

    uint32_t timeout_ms;
    uint32_t timestamp_ms;

    microphone_status_t status;
    struct fifo_buff fifo;

    bool visualize;
    bool read;
};

void MicrophoneInit(void);
void MicrophoneTask(void *argument);
void MicrophoneVisualizationTask(void *argument);
void MicrophoneSetStatus(microphone_status_t status);
void MicrophoneSetVisualizer(bool mode);
bool MicrophoneGetVisualizerStatus(void);
microphone_status_t MicrophoneGetStatus(void);

#endif /* MICROPHONE_H_ */
