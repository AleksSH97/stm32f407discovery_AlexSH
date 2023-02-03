/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "led.h"

struct timeout {
	volatile bool start_flag;
	volatile uint32_t timeout_ms;
	volatile uint32_t timestamp_ms;
};

extern TIM_HandleTypeDef htim1;

extern void MX_TIM1_Init(void);
extern void timeout_start(struct timeout *tim, uint32_t timeout_ms);
extern bool timeout_started(struct timeout *tim);
extern void timeout_stop(struct timeout *timeout);
extern bool timeout_elapsed(struct timeout *tim);
extern bool timeout_check(struct timeout *timeout, uint32_t msTime);

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

