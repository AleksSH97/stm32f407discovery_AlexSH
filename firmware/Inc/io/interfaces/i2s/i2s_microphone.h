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

void i2s_microphone_init(void);
void i2s_setup_receive_dma(void);

#endif /* I2S_MICROPHONE_H_ */
