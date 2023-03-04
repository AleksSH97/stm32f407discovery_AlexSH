/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"




osThreadId_t ButtonInput;
osThreadId_t IndicationUpdate;
osThreadId_t MicrophoneInput;
osThreadId_t VisualizationUpdate;




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




void MX_FREERTOS_Init(void)
{
    ButtonInput = osThreadNew(ButtonTask, NULL, &ButtonTask_attributes);

    IndicationUpdate = osThreadNew(IndicationUpdateTask, NULL, &IndicationTask_attributes);

    MicrophoneInput = osThreadNew(MicrophoneTask, NULL, &MicrophoneTask_attributes);

    VisualizationUpdate = osThreadNew(MicrophoneVisualizationTask, NULL, &VisualizationTask_attributes);
}
/******************************************************************************/






/******************************************************************************/

















