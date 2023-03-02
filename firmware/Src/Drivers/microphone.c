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
                IndicationLedBottom();
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
//    uint16_t data = data;

    if ((HAL_GetTick() - microphone.timestamp_ms) > microphone.timeout_ms) {
        microphone.timestamp_ms = HAL_GetTick();
        console_clear_screen_setup();
    }

    if ((data > 800) && (data < 1200)) {
        console_printf_crlf("|");
    }
    else if ((data > 1200) && (data < 1300)) {
        console_printf_crlf("||");
    }
    else if ((data > 1300) && (data < 1500)) {
        console_printf_crlf("|||");
    }
    else if ((data > 1500) && (data < 1800)) {
        console_printf_crlf("||||");
    }
    else if ((data > 1800) && (data < 2000)) {
        console_printf_crlf("|||||");
    }
    else if ((data > 2000) && (data < 3000)) {
        console_printf_crlf("||||||");
    }
    else if ((data > 3000) && (data < 4000)) {
        console_printf_crlf("|||||||");
    }
    else if ((data > 4000) && (data < 5000)) {
        console_printf_crlf("||||||||");
    }
    else if ((data > 5000) && (data < 10000)) {
        console_printf_crlf("|||||||||");
    }
    else if ((data > 10000) && (data < 15000)) {
        console_printf_crlf("||||||||||");
    }
    else if ((data > 15000) && (data < 20000)) {
        console_printf_crlf("|||||||||||");
    }
    else if ((data > 20000) && (data < 25000)) {
        console_printf_crlf("||||||||||||");
    }
    else if ((data > 25000) && (data < 30000)) {
        console_printf_crlf("|||||||||||||");
    }
    else if ((data > 30000) && (data < 35000)) {
        console_printf_crlf("||||||||||||||");
    }
    else {
        console_printf_cont("");
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
