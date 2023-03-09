/**
 ******************************************************************************
 * @file           : freertos.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : FreeRTOS inits (delete in future)
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"



/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
osThreadId_t ButtonInput;
osThreadId_t IndicationUpdate;
osThreadId_t MicrophoneInput;
osThreadId_t VisualizationUpdate;
osThreadId_t AccelerometerUpdate;

const osThreadAttr_t ButtonTask_attributes = {
        .name = "ButtonTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t IndicationTask_attributes = {
        .name = "IndicationUpdateTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t MicrophoneTask_attributes = {
        .name = "MicrophoneTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t VisualizationTask_attributes = {
        .name = "VisualizationTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t AccelerometerTask_attributes = {
        .name = "AccelerometerTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};
/******************************************************************************/


/**
 * @brief          FreeRTOS tasks init
 */
void MX_FREERTOS_Init(void)
{
    ButtonInput = osThreadNew(ButtonTask, NULL, &ButtonTask_attributes);

    IndicationUpdate = osThreadNew(IndicationUpdateTask, NULL, &IndicationTask_attributes);

    MicrophoneInput = osThreadNew(MicrophoneTask, NULL, &MicrophoneTask_attributes);

    VisualizationUpdate = osThreadNew(MicrophoneVisualizationTask, NULL, &VisualizationTask_attributes);

    AccelerometerUpdate = osThreadNew(AccelerometerTask, NULL, &AccelerometerTask_attributes);
}
/******************************************************************************/
