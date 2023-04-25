/**
 ******************************************************************************
 * @file           : main.h
 * @author         : Aleksandr Shabalin    <alexnv97@gmail.com>
 * @brief          : Header file of main
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin ------------------ *
 ******************************************************************************
 * This module is a confidential and proprietary property of Aleksandr Shabalin
 * and possession or use of this module requires written permission
 * of Aleksandr Shabalin.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

#include "button.h"
#include "tim.h"
#include "led.h"
#include "indication.h"

#include "log.h"

#include "uart.h"
#include "debug_uart.h"
#include "data_uart.h"

#include "lwprintf/lwprintf.h"
#include "console.h"
#include "lwrb.h"
#include "ring_buf.h"

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "semphr.h"

#include "microphone.h"
#include "pdm2pcm.h"

#include "accelerometer.h"
#include "accelero_spi.h"

#include "audio.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */


/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
void Error_Handler(void);
void MX_TIM1_Init(void);
void HAL_RCC_CSSCallBack(void);
void MX_FREERTOS_Init(void);


/******************************************************************************/
/* Public defines --------------------------------------------------------- */
/******************************************************************************/
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif


/******************************************************************************/


#ifdef __cplusplus
}
#endif


#endif /* __MAIN_H */
