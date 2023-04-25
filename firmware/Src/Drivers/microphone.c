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
osThreadId_t VisualizationUpdate;

const osMessageQueueAttr_t VisualQueueAttributes = {
        .name = "VisualizationQueue",
};

const osThreadAttr_t VisualizationTask_attributes = {
        .name = "VisualizationTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
static void prvMicrophoneCRCInit(void);
void prvMicrophoneSetTransmit(microphone_transmit_t transmit);
microphone_transmit_t prvMicrophoneGetTransmit(void);
void prvMicrophoneRxState1(void);
void prvMicrophoneRxState2(void);
void prvMicrophoneTxState1(void);
void prvMicrophoneTxState2(void);
void prvMicrophoneActivateDMA(void);
void prvMicrophoneDMAInit(void);

/******************************************************************************/




/**
 * \brief           Microphone init
 * \param[in]
 */
void MicrophoneInit(void)
{
    prvMicrophoneDMAInit();
    prvMicrophoneCRCInit();
    I2S2Init();
    I2S3Init();
    RingBufMicrophoneInit();
    PDM2PCM_init();

    microphone.rxstate = 0;
    microphone.txstate = 0;

    prvMicrophoneSetTransmit(MICROPHONE_TRANSMIT_BLOCKED);
    MicrophoneSetActivate(MICROPHONE_ON);
}
/******************************************************************************/




/**
 * @brief          DMA Microphone Init
 */
void prvMicrophoneDMAInit(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();

    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0x06, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0x06, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}
/******************************************************************************/




/**
 * \brief           DMA Microphone activate
 * \param[in]
 */
void prvMicrophoneActivateDMA(void)
{
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
    MicrophoneInit();
    PrintfLogsCRLF(CLR_GR"MICROPHONE INIT..."CLR_DEF);

    for (;;)
    {
        if (!MicrophoneGetActivate()) {
            continue;
        }

        switch (microphone.status) {
            case MICROPHONE_INIT:
                prvMicrophoneActivateDMA();
                AccelerometerSetStatus(ACCELERO_IDLE);

                microphone.timer = malloc(sizeof(struct timeout));
                microphone.timer->timeout_ms = MICROPHONE_TIMEOUT_MS;

                VisualQueueHandle = osMessageQueueNew(128, sizeof(uint16_t), &VisualQueueAttributes);
                VisualizationUpdate = osThreadNew(MicrophoneVisualizationTask, NULL, &VisualizationTask_attributes);
            case MICROPHONE_RX_STATE_1:
                if (microphone.rxstate == 1) {
                    prvMicrophoneRxState1();
                    MicrophoneSetStatus(MICROPHONE_IDLE);
                }
                break;
            case MICROPHONE_RX_STATE_2:
                if (microphone.rxstate == 2) {
                    prvMicrophoneRxState2();
                    MicrophoneSetStatus(MICROPHONE_IDLE);
                }
                break;
            case MICROPHONE_TX_STATE_1:
                if (microphone.txstate == 1) {
                    prvMicrophoneTxState1();
                    MicrophoneSetStatus(MICROPHONE_IDLE);
                }
            case MICROPHONE_TX_STATE_2:
                if (microphone.txstate == 2) {
                    prvMicrophoneTxState2();
                    MicrophoneSetStatus(MICROPHONE_IDLE);
                }
            case MICROPHONE_IDLE:
                break;
            case MICROPHONE_PROCESS_ERROR:
                PrintfLogsCRLF("\tError: microphone process error!");
                break;
            case MICROPHONE_INIT_ERROR:
                PrintfLogsCRLF("\tError: initialization of DMA I2S for microphone");
                break;
            default:
                break;
        }
    }
}
/******************************************************************************/




/**
 * \brief           Microphone RX_STATE_1
 * \param[in]
 */
void prvMicrophoneRxState1(void)
{
    PDM_Filter(&microphone.rx[0], &microphone.mid_buff[0], &PDM1_filter_handler);

    if (!MicrophonePutDataToRxBuffer(microphone.mid_buff, sizeof(microphone.mid_buff))) {
        MicrophoneSetStatus(MICROPHONE_PROCESS_ERROR);
    }

    osMessageQueuePut(VisualQueueHandle, microphone.mid_buff, 0, 100);

    if ((microphone.lwrb_rx.w - microphone.lwrb_rx.r) > MICROPHONE_BUFF_SIZE) {
        IndicationLedButton();
        prvMicrophoneSetTransmit(MICROPHONE_TRANSMIT_READY);
    }
    microphone.rxstate = 0;
}
/******************************************************************************/




/**
 * \brief           Microphone RX_STATE_2
 * \param[in]
 */
void prvMicrophoneRxState2(void)
{
    PDM_Filter(&microphone.rx[64], &microphone.mid_buff[0], &PDM1_filter_handler);

    if (!MicrophonePutDataToRxBuffer(microphone.mid_buff, sizeof(microphone.mid_buff))) {
        MicrophoneSetStatus(MICROPHONE_PROCESS_ERROR);
    }

    osMessageQueuePut(VisualQueueHandle, microphone.mid_buff, 0, 100);

    microphone.rxstate = 0;
}
/******************************************************************************/




/**
 * \brief           Microphone TX_STATE_1
 * \param[in]
 */
void prvMicrophoneTxState1(void)
{
    if (prvMicrophoneGetTransmit() == MICROPHONE_TRANSMIT_READY) {

        uint16_t data[MICROPHONE_HALF_BUFF_SIZE] = {0};
        MicrophoneGetDataFromRxBuffer(data);

        for (int i = 0; i < MICROPHONE_HALF_BUFF_SIZE; i = i + 4) {
            microphone.tx[i] = data[i];
            MicrophonePutDataToTxBuffer(&microphone.tx[i], microphone.tx[i]);
            microphone.tx[i + 2] = data[i];
            MicrophonePutDataToTxBuffer(&microphone.tx[i + 2], microphone.tx[i]);
        }
    }
}
/******************************************************************************/




/**
 * \brief           Microphone TX_STATE_2
 * \param[in]
 */
void prvMicrophoneTxState2(void)
{
    if (prvMicrophoneGetTransmit() == MICROPHONE_TRANSMIT_READY) {

        uint16_t data[MICROPHONE_HALF_BUFF_SIZE] = {0};
        MicrophoneGetDataFromRxBuffer(data);

        for (int i = MICROPHONE_HALF_BUFF_SIZE; i < MICROPHONE_BUFF_SIZE; i = i + 4) {
            microphone.tx[i] = data[i];
            MicrophonePutDataToTxBuffer(&microphone.tx[i], sizeof(microphone.tx[i]));
            microphone.tx[i + 2] = data[i];
            MicrophonePutDataToTxBuffer(&microphone.tx[i + 2], microphone.tx[i]);
        }
    }
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
 * \brief           Microphone get data from TX ring buffer
 * \param[in]
 */
void MicrophoneGetDataFromTxBuffer(uint16_t *data)
{
    lwrb_read(&microphone.lwrb_tx, data, sizeof(data));
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
 * \brief           Microphone get state
 * \param[in]
 */
microphone_status_t MicrophoneGetStatus(void)
{
    return microphone.status;
}
/******************************************************************************/




/**
 * \brief           Microphone disable
 * \param[in]
 */
bool MicrophoneDisable(void)
{
    MicrophoneSetActivate(MICROPHONE_OFF);
    prvMicrophoneSetTransmit(MICROPHONE_TRANSMIT_BLOCKED);

    if (HAL_I2S_DMAStop(&hi2s3) != HAL_OK) {
        return false;
    }

    if (HAL_I2S_DMAStop(&hi2s2) != HAL_OK) {
        return false;
    }

    return true;
}
/******************************************************************************/




/**
 * \brief           Microphone set read state
 * \param[in]
 */
void prvMicrophoneSetTransmit(microphone_transmit_t transmit)
{
    microphone.transmit = transmit;
}
/******************************************************************************/




/**
 * \brief           Microphone set read state
 * \param[in]
 */
microphone_transmit_t prvMicrophoneGetTransmit(void)
{
    return microphone.transmit;
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
void MicrophoneSetActivate(bool mode)
{
    microphone.activate = mode;
}
/******************************************************************************/




/**
 * \brief           Microphone get visualizer status
 * \param[in]
 */
bool MicrophoneGetActivate(void)
{
    return microphone.activate;
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
        if (!MicrophoneGetActivate()) {
            continue;
        }

        if (TimCheck(microphone.timer, MICROPHONE_TIMEOUT_MS)) {
            microphone.timer->timestamp_ms = HAL_GetTick();
            PrintfLogsCont(CLR_CLR);
        }

        for (uint32_t i = 0; i < MICROPHONE_MID_BUFF_SIZE; i++) {
            osStatus_t event = osMessageQueueGet(VisualQueueHandle, &msg[i], NULL, 200);

            if (event == osOK && msg[i] >= MICROPHONE_THRESHOLD && msg[i] <= 60000) {

                uint8_t bars = (msg[i] * MICROPHONE_MAX_BARS) / 0xFFFF;
                bars = bars > MICROPHONE_MAX_BARS ? MICROPHONE_MAX_BARS : bars;

                if (bars > 0) {
                    PrintfLogsCont("|");
                }
                for (uint32_t j = 1; j < bars; j++) {
                    PrintfLogsCont("|");
                }
                PrintfLogsCRLF("|");
            }
        }
    }
}
/******************************************************************************/
