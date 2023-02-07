/*
 * microphone.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "microphone.h"

static void microphone_crc_init(void);

CRC_HandleTypeDef hcrc;

void microphone_init(void)
{
    i2s_microphone_init();
    i2s_setup_receive_dma();
    microphone_crc_init();
    PDM2PCM_init();

    if (microphone.state == MICROPHONE_PROCESS_ERROR) {
        log_printf_crlf("Error: initialization of DMA I2S for microphone");
    }
}

void microphone_crc_init(void)
{
    __HAL_RCC_CRC_CLK_ENABLE();
    hcrc.Instance = CRC;

    if (HAL_CRC_Init(&hcrc) != HAL_OK) {
        Error_Handler();
    }
}






































