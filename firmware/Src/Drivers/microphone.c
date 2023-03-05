/*
 * microphone.c
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#include "microphone.h"





static void prvMicrophoneCRCInit(void);
static void prvMicrophoneFifoWrite(uint16_t data);
uint16_t prvMicrophoneFifoRead(void);
/******************************************************************************/




CRC_HandleTypeDef hcrc;
microphone_status_t microphone_status;
osMessageQueueId_t VisualQueueHandle;
/******************************************************************************/




const osMessageQueueAttr_t VisualQueueAttributes = {
        .name = "VisualizationQueue",
};
/******************************************************************************/




/**
 * \brief           Microphone init
 * \param[in]
 */
void MicrophoneInit(void)
{
    I2S2Init();
    I2S3Init();
    prvMicrophoneCRCInit();
    PDM2PCM_init();

    microphone.fifo.w_ptr = 0;
    microphone.fifo.r_ptr = 0;
    microphone.timeout_ms = MICROPHONE_TIMEOUT_MS;

    VisualQueueHandle = osMessageQueueNew(128, sizeof(uint16_t), &VisualQueueAttributes);

    HAL_I2S_Transmit_DMA(&hi2s3, &microphone.tx_buff[0], 64);
    HAL_I2S_Receive_DMA(&hi2s2, &microphone.record[0], 64);
}
/******************************************************************************/




/**
 * \brief           RTOS microphone task
 * \param[in]
 */
void MicrophoneTask(void *argument)
{
    uint8_t ddd[128] = {0};
    UNUSED(ddd);

    for (;;)
    {
        switch (microphone.status) {
            case MICROPHONE_RX_STATE_1:
                PDM_Filter(&microphone.record[0], &microphone.mid_buff[0], &PDM1_filter_handler);

                for (int i = 0; i < 16; i++) {
                    prvMicrophoneFifoWrite(microphone.mid_buff[i]);
                    osMessageQueuePut(VisualQueueHandle, &microphone.mid_buff[i], 0, 100);
                }

                if ((microphone.fifo.w_ptr - microphone.fifo.r_ptr) > MICROPHONE_BUFF_SIZE) {
                    IndicationLedBottom();
                    microphone.read = true;
                }

                MicrophoneSetStatus(MICROPHONE_READY);
                break;
            case MICROPHONE_RX_STATE_2:
                PDM_Filter(&microphone.record[64], &microphone.mid_buff[0], &PDM1_filter_handler);

                for (int i = 0; i < 16; i++) {
                    prvMicrophoneFifoWrite(microphone.mid_buff[i]);
                    osMessageQueuePut(VisualQueueHandle, &microphone.mid_buff[i], 0, 100);
                }

                MicrophoneSetStatus(MICROPHONE_READY);
                break;
            case MICROPHONE_TX_STATE_1:
                if (microphone.read) {
                    for (int i = 0; i < MICROPHONE_HALF_BUFF_SIZE; i = i + 4) {
                        uint16_t data = prvMicrophoneFifoRead();
                        microphone.tx_buff[i] = data;
                        microphone.tx_buff[i + 2] = data;
                    }
                }
                MicrophoneSetStatus(MICROPHONE_READY);
            case MICROPHONE_TX_STATE_2:
                if (microphone.read) {
                    for (int i = MICROPHONE_HALF_BUFF_SIZE; i < MICROPHONE_BUFF_SIZE; i = i + 4) {
                        uint16_t data = prvMicrophoneFifoRead();
                        microphone.tx_buff[i] = data;
                        microphone.tx_buff[i + 2] = data;
                    }
                }
                MicrophoneSetStatus(MICROPHONE_READY);
            case MICROPHONE_READY:
                break;
            case MICROPHONE_PROCESS_ERROR:
                PrintfLogsCRLF("Error: microphone process error!");
                break;
            case MICROPHONE_INIT_ERROR:
                PrintfLogsCRLF("Error: initialization of DMA I2S for microphone");
                break;
            default:
                break;
        }
        osDelay(1);
    }
}
/******************************************************************************/




/**
 * \brief           Microphone get state
 * \param[in]
 */
microphone_status_t MicrophoneGetStatus(void)
{
    return microphone.status;
}
/******************************************************************************/





/**
 * \brief           Microphone set state
 * \param[in]
 */
void MicrophoneSetStatus(microphone_status_t status)
{
    microphone.status = status;
}
/******************************************************************************/




/**
 * \brief           CRC init
 * \param[in]
 */
void prvMicrophoneCRCInit(void)
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
void prvMicrophoneFifoWrite(uint16_t data)
{
    microphone.fifo.buff[microphone.fifo.w_ptr] = data;
    microphone.fifo.w_ptr++;
}
/******************************************************************************/




/**
 * \brief           FIFO buff read
 * \param[in]
 */
uint16_t prvMicrophoneFifoRead(void)
{
    uint16_t val = microphone.fifo.buff[microphone.fifo.r_ptr];
    microphone.fifo.r_ptr++;

    return val;
}
/******************************************************************************/




/**
 * \brief           Microphone set visualizer off/on
 * \param[in]
 */
void MicrophoneSetVisualizer(bool mode)
{
    microphone.visualize = mode;
}
/******************************************************************************/




/**
 * \brief           Microphone get visualizer status
 * \param[in]
 */
bool MicrophoneGetVisualizerStatus(void)
{
    return microphone.visualize;
}
/******************************************************************************/


/**
 * \brief           Clear visualization queue
 * \param[in]
 */
void MicrophoneVisualizationClearQueue(void)
{
    osMessageQueueReset(VisualQueueHandle);
}
/******************************************************************************/



/**
 * \brief           Visualization of microphone task RTOS
 * \param[in]
 */
void MicrophoneVisualizationTask(void *argument)
{
    uint16_t msg[MICROPHONE_MID_BUFF_SIZE] = {0};

    for (;;)
    {
        if (MicrophoneGetVisualizerStatus()) {

            if ((HAL_GetTick() - microphone.timestamp_ms) > microphone.timeout_ms) {
                microphone.timestamp_ms = HAL_GetTick();
                PrintfConsoleCont(CLR_CLR);
            }

            for (int i = 0; i < MICROPHONE_MID_BUFF_SIZE; i++) {

                osStatus_t event = osMessageQueueGet(VisualQueueHandle, &msg[i], NULL, 200);

                if (event != osOK) {
                    continue;
                }

//                if ((msg[i] > 800) && (msg[i] < 1200)) {
//                    PrintfConsoleCRLF("|");
//                }
//                if ((msg[i] > 1200) && (msg[i] < 1300)) {
//                    PrintfConsoleCRLF("||");
//                }
                if ((msg[i] > 1300) && (msg[i] < 1500)) {
                    PrintfConsoleCRLF("|");
                }
                else if ((msg[i] > 1500) && (msg[i] < 1800)) {
                    PrintfConsoleCRLF("||");
                }
                else if ((msg[i] > 1800) && (msg[i] < 2000)) {
                    PrintfConsoleCRLF("|||");
                }
                else if ((msg[i] > 2000) && (msg[i] < 3000)) {
                    PrintfConsoleCRLF("||||");
                }
                else if ((msg[i] > 3000) && (msg[i] < 4000)) {
                    PrintfConsoleCRLF("|||||");
                }
                else if ((msg[i] > 4000) && (msg[i] < 5000)) {
                    PrintfConsoleCRLF("||||||");
                }
                else if ((msg[i] > 5000) && (msg[i] < 10000)) {
                    PrintfConsoleCRLF("|||||||");
                }
                else if ((msg[i] > 10000) && (msg[i] < 15000)) {
                    PrintfConsoleCRLF("||||||||");
                }
                else if ((msg[i] > 15000) && (msg[i] < 20000)) {
                    PrintfConsoleCRLF("|||||||||");
                }
                else if ((msg[i] > 20000) && (msg[i] < 25000)) {
                    PrintfConsoleCRLF("||||||||||");
                }
                else if ((msg[i] > 25000) && (msg[i] < 30000)) {
                    PrintfConsoleCRLF("|||||||||||");
                }
                else if ((msg[i] > 30000) && (msg[i] < 35000)) {
                    PrintfConsoleCRLF("||||||||||||");
                }
                else {
                    continue;
                }
            }
        }
    }
}
/******************************************************************************/
