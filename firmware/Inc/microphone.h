/*
 * microphone.h
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#ifndef MICROPHONE_H_
#define MICROPHONE_H_

#include "stm32f4xx_hal.h"
#include "i2s_microphone.h"
#include "i2s_dac.h"
#include "lwrb.h"
#include "tim.h"
#include "log.h"
#include "pdm2pcm.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MICROPHONE_BUFF_SIZE          128
#define MICROPHONE_HALF_BUFF_SIZE     64
#define MICROPHONE_MID_BUFF_SIZE      16

#define MICROPHONE_FIFO_BUFF_SIZE     256

#define MICROPHONE_TIMEOUT_MS         700

#define MICROPHONE_MAX_BARS           20
#define MICROPHONE_THRESHOLD          800

#define MICROPHONE_OFF                0
#define MICROPHONE_ON                 1

extern struct microphone microphone;
extern CRC_HandleTypeDef hcrc;
extern osMessageQueueId_t VisualQueueHandle;

typedef enum {
    MICROPHONE_TRANSMIT_BLOCKED = 0,
    MICROPHONE_TRANSMIT_READY = 1,
    MICROPHONE_TRANSMIT_ERROR = 2,
} microphone_transmit_t;

typedef enum {
    MICROPHONE_IDLE         = 0x00,
    MICROPHONE_RX_STATE_1    = 0x01,
    MICROPHONE_RX_STATE_2    = 0x02,
    MICROPHONE_TX_STATE_1    = 0x03,
    MICROPHONE_TX_STATE_2    = 0x04,
    MICROPHONE_INIT_ERROR    = 0x05,
    MICROPHONE_PROCESS_ERROR = 0x06,
    MICROPHONE_INIT          = 0x07,
} microphone_status_t;

struct microphone {
    lwrb_t   lwrb_rx;
    lwrb_t   lwrb_tx;

    uint16_t buff_rx[MICROPHONE_FIFO_BUFF_SIZE];
    uint16_t buff_tx[MICROPHONE_BUFF_SIZE];

    uint16_t rx[MICROPHONE_BUFF_SIZE];
    uint16_t mid_buff[MICROPHONE_MID_BUFF_SIZE];
    uint16_t tx[MICROPHONE_BUFF_SIZE];

    microphone_status_t status;

    bool activate;
    microphone_transmit_t transmit;

    struct timeout *timer;
};

void MicrophoneInit(void);
void MicrophoneTask(void *argument);
bool MicrophonePutDataToTxBuffer(const void* data, size_t len);
bool MicrophonePutDataToRxBuffer(const void* data, size_t len);
void MicrophoneGetDataFromRxBuffer(uint16_t *data);
void MicrophoneVisualizationTask(void *argument);
void MicrophoneSetStatus(microphone_status_t status);
void MicrophoneSetActivate(bool mode);
bool MicrophoneGetActivate(void);
bool MicrophoneDisable(void);
void MicrophoneVisualizationClearQueue(void);
microphone_status_t MicrophoneGetStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* MICROPHONE_H_ */
