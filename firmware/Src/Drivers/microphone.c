/*
 * microphone.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "microphone.h"

static void microphone_crc_init(void);
static void microphone_fifo_write(uint16_t data);
static void i2s2_init(void);
static void i2s3_init(void);

CRC_HandleTypeDef hcrc;
microphone_status_t microphone_status;
I2S_HandleTypeDef hi2s2;
I2S_HandleTypeDef hi2s3;

//uint16_t fifobuf[256];
//uint8_t fifo_w_ptr = 0;
//uint8_t fifo_r_ptr = 0;
//uint8_t fifo_read_enabled = 0;

void microphone_init(void)
{
    i2s2_init();
    i2s3_init();
    microphone_crc_init();
    PDM2PCM_init();

    microphone.fifo.w_ptr = 0;
    microphone.fifo.r_ptr = 0;

    HAL_I2S_Transmit_DMA(&hi2s3, &microphone.tx_buff[0], 64);
    HAL_I2S_Receive_DMA(&hi2s2, &microphone.record[0], 64);
}

void MicrophoneTaskStart(void *argument)
{
    uint8_t ddd[128] = {0};
    UNUSED(ddd);

    for (;;)
    {
        microphone_process();
        osDelay(10);
    }
}

void microphone_process(void)
{
    switch (microphone.state) {
        case MICROPHONE_RX_STATE_1:
            PDM_Filter(&microphone.record[0], &microphone.mid_buff[0], &PDM1_filter_handler);

            for (int i = 0; i < 16; i++) {
                microphone_fifo_write(microphone.mid_buff[i]);
                //log_printf_crlf("%d ", microphone.mid_buff[i]);
            }

            if ((microphone.fifo.w_ptr - microphone.fifo.r_ptr) > I2S2_BUFF_SIZE) {
                indication_led_bottom();
                microphone.read = true;
            }
            microphone.state = MICROPHONE_READY;
            break;
        case MICROPHONE_RX_STATE_2:
            PDM_Filter(&microphone.record[64], &microphone.mid_buff[0], &PDM1_filter_handler);

            for (int i = 0; i < 16; i++) {
                microphone_fifo_write(microphone.mid_buff[i]);
                log_printf_crlf("%d ", microphone.mid_buff[i]);
            }

            microphone.state = MICROPHONE_READY;
            break;
        case MICROPHONE_TX_STATE_1:
            if (microphone.read) {
                for (int i = 0; i < I2S2_HALF_BUFF_SIZE; i = i + 4) {
                    uint16_t data = microphone_fifo_read();
                    microphone.tx_buff[i] = data;
                    //log_printf_crlf("%d ", microphone.tx_buff[i]);
                    microphone.tx_buff[i + 2] = data;
                    //log_printf_crlf("%d ", microphone.tx_buff[i+2]);
                }
            }
            microphone.state = MICROPHONE_READY;
        case MICROPHONE_TX_STATE_2:
            if (microphone.read) {
                for (int i = I2S2_HALF_BUFF_SIZE; i < I2S2_BUFF_SIZE; i = i + 4) {
                    uint16_t data = microphone_fifo_read();
                    microphone.tx_buff[i] = data;
                    //log_printf_crlf("%d ", microphone.tx_buff[i]);
                    microphone.tx_buff[i + 2] = data;
                    //log_printf_crlf("%d ", microphone.tx_buff[i+2]);
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

void microphone_crc_init(void)
{
    hcrc.Instance = CRC;

    if (HAL_CRC_Init(&hcrc) != HAL_OK) {
        Error_Handler();
    }

    __HAL_CRC_DR_RESET(&hcrc);
}

void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{
    if(hcrc->Instance==CRC) {
        __HAL_RCC_CRC_CLK_ENABLE();
    }
}

void microphone_fifo_write(uint16_t data)
{
    microphone.fifo.buff[microphone.fifo.w_ptr] = data;
    microphone.fifo.w_ptr++;
}

uint16_t microphone_fifo_read(void)
{
    uint16_t val = microphone.fifo.buff[microphone.fifo.r_ptr];
    microphone.fifo.r_ptr++;

    return val;
}

void i2s2_init(void)
{
    hi2s2.Instance = SPI2;
    hi2s2.Init.Mode = I2S_MODE_SLAVE_RX;
    hi2s2.Init.Standard = I2S_STANDARD_MSB;
    hi2s2.Init.DataFormat = I2S_DATAFORMAT_24B;
    hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_48K;
    hi2s2.Init.CPOL = I2S_CPOL_LOW;
    hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;

    if (HAL_I2S_Init(&hi2s2) != HAL_OK) {
      Error_Handler();
    }
}

void i2s3_init(void)
{
    hi2s3.Instance = SPI3;
    hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s3.Init.DataFormat = I2S_DATAFORMAT_24B;
    hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_48K;
    hi2s3.Init.CPOL = I2S_CPOL_LOW;
    hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;

    if (HAL_I2S_Init(&hi2s3) != HAL_OK)
    {
      Error_Handler();
    }
}

































