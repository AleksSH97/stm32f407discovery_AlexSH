/**
 ******************************************************************************
 * @file           : i2s_microphone.h
 * @author         : Aleksandr Shabalin    <alexnv97@gmail.com>
 * @brief          : Header file for microphone
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin ------------------ *
 ******************************************************************************
 * This module is a confidential and proprietary property of Aleksandr Shabalin
 * and possession or use of this module requires written permission
 * of Aleksandr Shabalin.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef I2S_MICROPHONE_H_
#define I2S_MICROPHONE_H_


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
/* Public defines ----------------------------------------------------------- */
/******************************************************************************/
#define I2S_MICROPHONE_PLLI2SN         192
#define I2S_MICROPHONE_PLLI2SR         2
#define I2S_MICROPHONE_PERIPH_CLOCK    RCC_PERIPHCLK_I2S


/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/
extern I2S_HandleTypeDef hi2s2;


/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
void i2s_microphone_init(void);
void I2S2Init(void);
bool I2S2ReceiveDMA(I2S_HandleTypeDef *hi2s, uint16_t *buff);


/******************************************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* I2S_MICROPHONE_H_ */
