/*
 * indication.h
 *
 *  Created on: 15 дек. 2022 г.
 *      Author: АлексанDOOR
 */

#ifndef INDICATION_H_
#define INDICATION_H_

#include <stdbool.h>

#include "main.h"
#include "log.h"
#include "stm32f4xx_hal.h"

#define INDICATION_LED_LOADING_NUM                (10000u)
#define INDICATION_LED_ERROR_NUM                  (3u)
#define INDICATION_LED_BUTTON_NUM                 (1u)

#define N_LED                          (4u)

#define LED_LEFT_Pin                   GPIO_PIN_12
#define LED_LEFT_GPIO_Port             GPIOD
#define LED_TOP_Pin                    GPIO_PIN_13
#define LED_TOP_GPIO_Port              GPIOD
#define LED_RIGHT_Pin                  GPIO_PIN_14
#define LED_RIGHT_GPIO_Port            GPIOD
#define LED_BOTTOM_Pin                 GPIO_PIN_15
#define LED_BOTTOM_GPIO_Port           GPIOD

#define ZERO_MS                        (0u)

#define LED_TOP_TIME_ON                (50u)
#define LED_LEFT_TIME_ON               (70u)
#define LED_BOTTOM_TIME_ON             (90u)
#define LED_RIGHT_TIME_ON              (110u)

#define LED_TOP_TIME_OFF               (70u)
#define LED_LEFT_TIME_OFF              (90u)
#define LED_BOTTOM_TIME_OFF            (110u)
#define LED_RIGHT_TIME_OFF             (130u)

#define LED_TIME_ON                    (50u)
#define LED_TIME_OFF                   (70u)

enum indication_led_id {
	LED_TOP = 0,
	LED_LEFT,
	LED_BOTTOM,
	LED_RIGHT
};

enum indication_led_speed {
	INDICATION_LED_SPEED_VERY_FAST = 1,
	INDICATION_LED_SPEED_FAST,
	INDICATION_LED_SPEED_MIDDLE,
	INDICATION_LED_SPEED_SLOW,
	INDICATION_LED_SPEED_VERY_SLOW = 6,
	INDICATION_LED_SPEED_HOLD = 20
};

extern enum indication_led_id id;
extern enum indication_led_mode mode;
extern enum indication_led_speed speed;

extern mculed_t mculed[N_LED];

void IndicationInit(void);
void IndicationLedTop(void);
void IndicationLedBottom(void);
void IndicationLedLoading(void);
void IndicationLedError(void);
void IndicationLedButton(void);

void IndicationLedTurnOn(mculed_t *self);
void IndicationLedTurnOff(mculed_t *self);
void IndicationLedToggle(mculed_t *self);

void IndicationLedsUpdate(void);
void IndicationLedButtonHold(void);
void IndicationLedButtonDoubleClick(void);
void IndicationUpdateTask(void *argument);

#endif /* INDICATION_H_ */

