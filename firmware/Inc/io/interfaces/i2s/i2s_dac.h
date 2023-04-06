/**
 ******************************************************************************
 * @file           : i2c_dac.h
 * @author         : Aleksandr Shabalin    <alexnv97@gmail.com>
 * @brief          : Header file for DAC
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin ------------------ *
 ******************************************************************************
 * This module is a confidential and proprietary property of Aleksandr Shabalin
 * and possession or use of this module requires written permission
 * of Aleksandr Shabalin.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IO_INTERFACES_I2S_I2S_DAC_H_
#define IO_INTERFACES_I2S_I2S_DAC_H_


/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "microphone.h"
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/
extern I2S_HandleTypeDef hi2s3;


/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
bool I2S3TransmitDMA(I2S_HandleTypeDef *hi2s, uint16_t *buff);
void I2S3Init(void);


/******************************************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* IO_INTERFACES_I2S_I2S_DAC_H_ */
