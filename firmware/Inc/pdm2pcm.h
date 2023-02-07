/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : pdm2pcm.h
  * Description        : This file provides code for the configuration
  *                      of the pdm2pcm instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __pdm2pcm_H
#define __pdm2pcm_H
#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "pdm2pcm_glo.h"


/* Global variables ---------------------------------------------------------*/
extern PDM_Filter_Handler_t PDM1_filter_handler;
extern PDM_Filter_Config_t PDM1_filter_config;


void PDM2PCM_init(void);
uint8_t PDM2PCM_process(uint16_t *PDMBuf, uint16_t *PCMBuf);


#ifdef __cplusplus
}
#endif
#endif /*__pdm2pcm_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
