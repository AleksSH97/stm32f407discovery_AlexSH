/*
 * i2s_dac.h
 *
 *  Created on: 14 февр. 2023 г.
 *      Author: АлексанDOOR
 */

#ifndef IO_INTERFACES_I2S_I2S_DAC_H_
#define IO_INTERFACES_I2S_I2S_DAC_H_

#include <stdint.h>
#include <stdbool.h>

#include "microphone.h"
#include "stm32f4xx_hal.h"

extern I2S_HandleTypeDef hi2s3;

bool i2s3_transmit_dma(I2S_HandleTypeDef *hi2s, uint16_t *buff);
void i2s3_init(void);

#endif /* IO_INTERFACES_I2S_I2S_DAC_H_ */
