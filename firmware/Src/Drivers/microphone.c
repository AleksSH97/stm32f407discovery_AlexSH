/*
 * microphone.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "microphone.h"

static void microphone_crc_init(void);
static void microphone_fifo_write(uint16_t data);
static void microphone_visualization(uint16_t data);

CRC_HandleTypeDef hcrc;
microphone_status_t microphone_status;




/**
 * \brief           Microphone init
 * \param[in]
 */
void microphone_init(void)
{
    i2s2_init();
    i2s3_init();
    microphone_crc_init();
    PDM2PCM_init();

    microphone.fifo.w_ptr = 0;
    microphone.fifo.r_ptr = 0;
    microphone.timeout_ms = MICROPHONE_TIMEOUT_MS;
    //microphone.visualize = true;

    HAL_I2S_Transmit_DMA(&hi2s3, &microphone.tx_buff[0], 64);
    HAL_I2S_Receive_DMA(&hi2s2, &microphone.record[0], 64);
}
/******************************************************************************/



/**
 * \brief           RTOS microphone task
 * \param[in]
 */
void MicrophoneTaskStart(void *argument)
{
    uint8_t ddd[128] = {0};
    UNUSED(ddd);

    for (;;)
    {
        microphone_process();
        osDelay(1);
    }
}
/******************************************************************************/




/**
 * \brief           Main microphone process
 * \param[in]
 */
void microphone_process(void)
{
    switch (microphone.state) {
        case MICROPHONE_RX_STATE_1:
            PDM_Filter(&microphone.record[0], &microphone.mid_buff[0], &PDM1_filter_handler);

            for (int i = 0; i < 16; i++) {
                microphone_fifo_write(microphone.mid_buff[i]);
                if (microphone.visualize) {
                    microphone_visualization(microphone.mid_buff[i]);
                }
            }

            if ((microphone.fifo.w_ptr - microphone.fifo.r_ptr) > MICROPHONE_BUFF_SIZE) {
                indication_led_bottom();
                microphone.read = true;
            }

            microphone.state = MICROPHONE_READY;
            break;
        case MICROPHONE_RX_STATE_2:
            PDM_Filter(&microphone.record[64], &microphone.mid_buff[0], &PDM1_filter_handler);

            for (int i = 0; i < 16; i++) {
                microphone_fifo_write(microphone.mid_buff[i]);
                if (microphone.visualize) {
                    microphone_visualization(microphone.mid_buff[i]);
                }
            }

            microphone.state = MICROPHONE_READY;
            break;
        case MICROPHONE_TX_STATE_1:
            if (microphone.read) {
                for (int i = 0; i < MICROPHONE_HALF_BUFF_SIZE; i = i + 4) {
                    uint16_t data = microphone_fifo_read();
                    microphone.tx_buff[i] = data;
                    microphone.tx_buff[i + 2] = data;
                }
            }
            microphone.state = MICROPHONE_READY;
        case MICROPHONE_TX_STATE_2:
            if (microphone.read) {
                for (int i = MICROPHONE_HALF_BUFF_SIZE; i < MICROPHONE_BUFF_SIZE; i = i + 4) {
                    uint16_t data = microphone_fifo_read();
                    microphone.tx_buff[i] = data;
                    microphone.tx_buff[i + 2] = data;
                }
            }
            microphone.state = MICROPHONE_READY;
        case MICROPHONE_READY:
            break;
        case MICROPHONE_PROCESS_ERROR:
            log_printf_crlf("Error: microphone process error!");
            break;
        case MICROPHONE_INIT_ERROR:
            log_printf_crlf("Error: initialization of DMA I2S for microphone");
            break;
    }
}
/******************************************************************************/




/**
 * \brief           CRC init
 * \param[in]
 */
void microphone_crc_init(void)
{
    hcrc.Instance = CRC;

    if (HAL_CRC_Init(&hcrc) != HAL_OK) {
        Error_Handler();
    }

    __HAL_CRC_DR_RESET(&hcrc);
}
/******************************************************************************/




/**
 * \brief           FIFO buff write
 * \param[in]
 */
void microphone_fifo_write(uint16_t data)
{
    microphone.fifo.buff[microphone.fifo.w_ptr] = data;
    microphone.fifo.w_ptr++;
}
/******************************************************************************/




/**
 * \brief           Audio visualization
 * \param[in]
 */
void microphone_visualization(uint16_t data)
{
    uint16_t volume = data;

    if ((HAL_GetTick() - microphone.timestamp_ms) > microphone.timeout_ms) {
        microphone.timestamp_ms = HAL_GetTick();
        console_clear_screen_setup();
    }

//    if (volume < 100) {
//        console_printf_cont("");
//    }
    if ((volume > 800) && (volume < 1200)) {
        log_printf_crlf("|");
    }
    else if ((volume > 1200) && (volume < 1300)) {
        log_printf_crlf("||");
    }
    else if ((volume > 1300) && (volume < 1500)) {
        log_printf_crlf("|||");
    }
    else if ((volume > 1500) && (volume < 1800)) {
        log_printf_crlf("||||");
    }
    else if ((volume > 1800) && (volume < 2000)) {
        log_printf_crlf("|||||");
    }
    else if ((volume > 2000) && (volume < 3000)) {
        log_printf_crlf("||||||");
    }
    else if ((volume > 3000) && (volume < 4000)) {
        log_printf_crlf("|||||||");
    }
    else if ((volume > 4000) && (volume < 5000)) {
        log_printf_crlf("||||||||");
    }
    else if ((volume > 5000) && (volume < 10000)) {
        log_printf_crlf("|||||||||");
    }
    else if ((volume > 10000) && (volume < 15000)) {
        log_printf_crlf("||||||||||");
    }
    else if ((volume > 15000) && (volume < 20000)) {
        log_printf_crlf("|||||||||||");
    }
    else if ((volume > 20000) && (volume < 25000)) {
        log_printf_crlf("||||||||||||");
    }
    else if ((volume > 25000) && (volume < 30000)) {
        log_printf_crlf("|||||||||||||");
    }
    else if ((volume > 30000) && (volume < 35000)) {
        log_printf_crlf("||||||||||||||");
    }
    else {
        log_printf_cont("");
    }
}
/******************************************************************************/




/**
 * \brief           FIFO buff read
 * \param[in]
 */
uint16_t microphone_fifo_read(void)
{
    uint16_t val = microphone.fifo.buff[microphone.fifo.r_ptr];
    microphone.fifo.r_ptr++;

    return val;
}
/******************************************************************************/
