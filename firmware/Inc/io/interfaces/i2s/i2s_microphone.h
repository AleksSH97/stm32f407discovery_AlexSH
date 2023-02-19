/*
 * i2s_microphone.h
 *
 *  Created on: 5 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#ifndef I2S_MICROPHONE_H_
#define I2S_MICROPHONE_H_

#include <stdint.h>
#include <stdbool.h>

#include "microphone.h"
#include "stm32f4xx_hal.h"

#define I2S_MICROPHONE_PLLI2SN         192
#define I2S_MICROPHONE_PLLI2SR         2
#define I2S_MICROPHONE_PERIPH_CLOCK    RCC_PERIPHCLK_I2S

extern I2S_HandleTypeDef hi2s2;

void i2s_microphone_init(void);
void i2s2_init(void);
bool i2s2_receive_dma(I2S_HandleTypeDef *hi2s, uint16_t *buff);

#endif /* I2S_MICROPHONE_H_ */
