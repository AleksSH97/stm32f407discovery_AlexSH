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

osThreadId_t LogsInput;
osThreadId_t ButtonInput;
osThreadId_t IndicationUpdate;

const osThreadAttr_t LogsTask_attributes = {
        .name = "LogsTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t ButtonTask_attributes = {
        .name = "ButtonTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t IndicationTask_attributes = {
        .name = "LedsUpdateTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};


void MX_FREERTOS_Init(void)
{
    LogsInput = osThreadNew(LogsTaskStart, NULL, &LogsTask_attributes);
    ButtonInput = osThreadNew(ButtonTaskStart, NULL, &ButtonTask_attributes);
    IndicationUpdate = osThreadNew(IndicationUpdateTaskStart, NULL, &IndicationTask_attributes);
}
/******************************************************************************/





/******************************************************************************/

















