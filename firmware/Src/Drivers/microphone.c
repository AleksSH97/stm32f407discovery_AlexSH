/**
 ******************************************************************************
 * @file           : microphone.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Microphone input/output system
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "microphone.h"




/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
CRC_HandleTypeDef hcrc;
microphone_status_t microphone_status;
osMessageQueueId_t VisualQueueHandle;

const osMessageQueueAttr_t VisualQueueAttributes = {
        .name = "VisualizationQueue",
};




/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
static void prvMicrophoneCRCInit(void);
void prvMicrophoneSetRead(bool activate);
bool prvMicrophoneGetRead(void);

/******************************************************************************/




/**
 * \brief           Microphone init
 * \param[in]
 */
void MicrophoneInit(void)
{
    I2S2Init();
    I2S3Init();
    RingBufMicrophoneInit();
    prvMicrophoneCRCInit();
    PDM2PCM_init();

    prvMicrophoneSetRead(false);

    microphone.timeout_ms = MICROPHONE_TIMEOUT_MS;

    VisualQueueHandle = osMessageQueueNew(128, sizeof(uint16_t), &VisualQueueAttributes);

    HAL_I2S_Transmit_DMA(&hi2s3, &microphone.tx[0], 64);
    HAL_I2S_Receive_DMA(&hi2s2, &microphone.rx[0], 64);
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
                PDM_Filter(&microphone.rx[0], &microphone.mid_buff[0], &PDM1_filter_handler);

                if (!MicrophonePutDataToRxBuffer(&microphone.mid_buff, sizeof(microphone.mid_buff))) {
                    MicrophoneSetStatus(MICROPHONE_PROCESS_ERROR);
                }

                osMessageQueuePut(VisualQueueHandle, microphone.mid_buff, 0, 100);

                if ((microphone.lwrb_rx.w - microphone.lwrb_rx.r) > MICROPHONE_BUFF_SIZE) {
                    IndicationLedBottom();
                    prvMicrophoneSetRead(true);
                }

                MicrophoneSetStatus(MICROPHONE_READY);
                break;
            case MICROPHONE_RX_STATE_2:
                PDM_Filter(&microphone.rx[64], &microphone.mid_buff[0], &PDM1_filter_handler);

                if (!MicrophonePutDataToRxBuffer(&microphone.mid_buff, sizeof(microphone.mid_buff))) {
                    MicrophoneSetStatus(MICROPHONE_PROCESS_ERROR);
                }

                osMessageQueuePut(VisualQueueHandle, microphone.mid_buff, 0, 100);

                MicrophoneSetStatus(MICROPHONE_READY);
                break;
            case MICROPHONE_TX_STATE_1:
                if (prvMicrophoneGetRead()) {

                    uint16_t data[MICROPHONE_HALF_BUFF_SIZE] = {0};
                    MicrophoneGetDataFromRxBuffer(data);

                    for (int i = 0; i < MICROPHONE_HALF_BUFF_SIZE; i = i + 4) {
                        microphone.tx[i] = data[i];
                        microphone.tx[i + 2] = data[i];
                    }
                }
                MicrophoneSetStatus(MICROPHONE_READY);
            case MICROPHONE_TX_STATE_2:
                if (prvMicrophoneGetRead()) {

                    uint16_t data[MICROPHONE_HALF_BUFF_SIZE] = {0};
                    MicrophoneGetDataFromRxBuffer(data);

                    for (int i = MICROPHONE_HALF_BUFF_SIZE; i < MICROPHONE_BUFF_SIZE; i = i + 4) {
                        microphone.tx[i] = data[i];
                        microphone.tx[i + 2] = data[i];
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
 * \brief           Microphone write TX ring buffer
 * \param[in]
 */
bool MicrophonePutDataToTxBuffer(const void* data, size_t len)
{
    if (lwrb_get_free(&microphone.lwrb_tx) == 0) {
        return false;
    }

    return (lwrb_write(&microphone.lwrb_tx, data, len) > 0 ? true : false);
}
/******************************************************************************/




/**
 * \brief           Microphone write RX ring buffer
 * \param[in]
 */
bool MicrophonePutDataToRxBuffer(const void* data, size_t len)
{
    if (lwrb_get_free(&microphone.lwrb_rx) == 0) {
        return false;
    }

    return (lwrb_write(&microphone.lwrb_rx, data, len) > 0 ? true : false);
}
/******************************************************************************/



/**
 * \brief           Microphone get data from RX ring buffer
 * \param[in]
 */
void MicrophoneGetDataFromRxBuffer(uint16_t *data)
{
    lwrb_read(&microphone.lwrb_rx, data, sizeof(data));
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
 * \brief           Microphone set read state
 * \param[in]
 */
void prvMicrophoneSetRead(bool activate)
{
    microphone.read = activate;
}
/******************************************************************************/




/**
 * \brief           Microphone set read state
 * \param[in]
 */
bool prvMicrophoneGetRead(void)
{
    return microphone.read;
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
