/*
 * microphone.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "microphone.h"

void microphone_init(void)
{
    i2s_microphone_init();
    i2s_setup_receive_dma();

    if (microphone.state == MICROPHONE_PROCESS_ERROR) {
        log_printf_crlf("Error: initialization of DMA I2S for microphone");
    }
}








































