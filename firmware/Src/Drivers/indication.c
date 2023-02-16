/**
 ******************************************************************************
 * @file           : indication.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Indication file
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "led.h"
#include "indication.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
mculed_t mculed[N_LED];

/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
static void indication_led_loading_setup(mculed_t *led_ptr, int led_index);
static void indication_led_button_setup(mculed_t *led_ptr, int led_index);
static void indication_led_button_hold_setup(mculed_t *led_ptr, int led_index);
static void indication_led_error_setup(mculed_t *led_ptr);
static void indication_led_button_double_click_setup(mculed_t *led_ptr, int led_index);
static void indication_led_top_setup(mculed_t *led_ptr, int led_index);
static void indication_led_bottom_setup(mculed_t *led_ptr, int led_index);

/**
 * @brief          Initialization off led CLK, Pins, hardware, fns and init of each led fns
 */
void initialize_led_indication(void)
{
	mculed_ctrl_t fns = {0};

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOD, LED_LEFT_Pin|LED_TOP_Pin|LED_RIGHT_Pin|LED_BOTTOM_Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = LED_LEFT_Pin|LED_TOP_Pin|LED_RIGHT_Pin|LED_BOTTOM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	mculed[LED_TOP].hardware.port    =    (uint32_t) LED_TOP_GPIO_Port;
	mculed[LED_TOP].hardware.pin     =    (uint32_t) LED_TOP_Pin;
	mculed[LED_LEFT].hardware.port   =    (uint32_t) LED_LEFT_GPIO_Port;
	mculed[LED_LEFT].hardware.pin    =    (uint32_t) LED_LEFT_Pin;
	mculed[LED_BOTTOM].hardware.port =    (uint32_t) LED_BOTTOM_GPIO_Port;
	mculed[LED_BOTTOM].hardware.pin  =    (uint32_t) LED_BOTTOM_Pin;
	mculed[LED_RIGHT].hardware.port  =    (uint32_t) LED_RIGHT_GPIO_Port;
	mculed[LED_RIGHT].hardware.pin   =    (uint32_t) LED_RIGHT_Pin;

	fns.turn_on = indication_led_turn_on;
	fns.turn_off = indication_led_turn_off;
	fns.toggle = indication_led_toggle;

	mculed_init(&mculed[LED_TOP], &fns);
	mculed_init(&mculed[LED_LEFT], &fns);
	mculed_init(&mculed[LED_BOTTOM], &fns);
	mculed_init(&mculed[LED_RIGHT], &fns);
}
/******************************************************************************/




void IndicationUpdateTaskStart(void *argument)
{
    uint8_t ddd[128] = {0};
    UNUSED(ddd);

    for (;;)
    {
        indication_leds_update();

        osDelay(10);
    }
}
/******************************************************************************/




/**
 * @brief          Led actions fns
 */
void indication_led_turn_on(mculed_t *self)
{
	HAL_GPIO_WritePin((GPIO_TypeDef *) self->hardware.port, self->hardware.pin, GPIO_PIN_SET);
}

void indication_led_turn_off(mculed_t *self)
{
	HAL_GPIO_WritePin((GPIO_TypeDef *) self->hardware.port, self->hardware.pin, GPIO_PIN_RESET);
}

void indication_led_toggle(mculed_t *self)
{
	HAL_GPIO_TogglePin((GPIO_TypeDef *)self->hardware.port, self->hardware.pin);
}
/******************************************************************************/




/**
 * @brief          Loading led animation function
 */
void indication_led_loading(void)
{
	for (int led_index = 0; led_index < N_LED; led_index++) {
		indication_led_loading_setup(&mculed[led_index], led_index);
		led_function(&mculed[led_index]);
	}
}
/******************************************************************************/




/**
 * @brief          LED top on
 */
void indication_led_top(void)
{
	for (int led_index = 0; led_index < N_LED; led_index++) {
	    indication_led_top_setup(&mculed[led_index], led_index);
		led_function(&mculed[led_index]);
	}
}
/******************************************************************************/




/**
 * @brief          LED bottom on
 */
void indication_led_bottom(void)
{
    for (int led_index = 0; led_index < N_LED; led_index++) {
        indication_led_bottom_setup(&mculed[led_index], led_index);
        led_function(&mculed[led_index]);
    }
}
/******************************************************************************/




/**
 * @brief          Button led function
 */
void indication_led_button(void)
{
    for (int led_index = 0; led_index < N_LED; led_index++) {
        indication_led_button_setup(&mculed[led_index], led_index);
        led_function(&mculed[led_index]);
    }
}




/**
 * @brief          Hold button led function
 */
void indication_led_button_hold(void)
{
	for (int led_index = 0; led_index < N_LED; led_index++) {
		indication_led_button_hold_setup(&mculed[led_index], led_index);
		led_function(&mculed[led_index]);
	}
}
/******************************************************************************/




/**
 * @brief          Double click button led function
 */
void indication_led_button_double_click(void)
{
	for (int led_index = 0; led_index < N_LED; led_index++) {
		indication_led_button_double_click_setup(&mculed[led_index], led_index);
		led_function(&mculed[led_index]);
	}
}
/******************************************************************************/




/**
 * @brief          Error led function
 */
void indication_led_error(void)
{
	for (int led_index = 0; led_index < N_LED; led_index++) {
		indication_led_error_setup(&mculed[led_index]);
		led_function(&mculed[led_index]);
	}
}
/******************************************************************************/




void indication_led_accelerometer(void)
{

}




/**
 * @brief          Loading led top animation setup
 */
void indication_led_top_setup(mculed_t *led_ptr, int led_index)
{
    led_ptr->setup.iterations_num = INDICATION_LED_BUTTON_NUM;

    switch (led_index) {
        case LED_TOP:
            led_ptr->hardware.mode = MCULED_ON_STATE;
            led_ptr->setup.on_ms = (INDICATION_LED_SPEED_FAST * LED_RIGHT_TIME_ON);
            led_ptr->setup.off_ms = (INDICATION_LED_SPEED_FAST * LED_RIGHT_TIME_OFF);
            led_ptr->setup.delay_ms = ZERO_MS;
            break;
        case LED_LEFT:
            led_ptr->hardware.mode = MCULED_OFF_STATE;
            led_ptr->setup.on_ms = ZERO_MS;
            led_ptr->setup.delay_ms = ZERO_MS;
            led_ptr->setup.off_ms = ZERO_MS;
            break;
        case LED_BOTTOM:
            led_ptr->hardware.mode = MCULED_OFF_STATE;
            led_ptr->setup.on_ms = ZERO_MS;
            led_ptr->setup.off_ms = ZERO_MS;
            led_ptr->setup.delay_ms = ZERO_MS;
            break;
        case LED_RIGHT:
            led_ptr->hardware.mode = MCULED_OFF_STATE;
            led_ptr->setup.on_ms = ZERO_MS;
            led_ptr->setup.delay_ms = ZERO_MS;
            led_ptr->setup.off_ms = ZERO_MS;
            break;
        default:
            break;
    }

    led_ptr->status.iterations_counter = 0;
    led_ptr->status.on_timeout = led_ptr->setup.on_ms;
    led_ptr->status.off_timeout = led_ptr->setup.off_ms;
}
/******************************************************************************/



/**
 * @brief          Loading led bottom animation setup
 */
void indication_led_bottom_setup(mculed_t *led_ptr, int led_index)
{
    led_ptr->setup.iterations_num = INDICATION_LED_BUTTON_NUM;

    switch (led_index) {
        case LED_TOP:
            led_ptr->hardware.mode = MCULED_OFF_STATE;
            led_ptr->setup.on_ms = ZERO_MS;
            led_ptr->setup.delay_ms = ZERO_MS;
            led_ptr->setup.off_ms = ZERO_MS;
            break;
        case LED_LEFT:
            led_ptr->hardware.mode = MCULED_OFF_STATE;
            led_ptr->setup.on_ms = ZERO_MS;
            led_ptr->setup.delay_ms = ZERO_MS;
            led_ptr->setup.off_ms = ZERO_MS;
            break;
        case LED_BOTTOM:
            led_ptr->hardware.mode = MCULED_ON_STATE;
            led_ptr->setup.on_ms = (INDICATION_LED_SPEED_FAST * LED_RIGHT_TIME_ON);
            led_ptr->setup.off_ms = (INDICATION_LED_SPEED_FAST * LED_RIGHT_TIME_OFF);
            led_ptr->setup.delay_ms = ZERO_MS;
            break;
        case LED_RIGHT:
            led_ptr->hardware.mode = MCULED_OFF_STATE;
            led_ptr->setup.on_ms = ZERO_MS;
            led_ptr->setup.delay_ms = ZERO_MS;
            led_ptr->setup.off_ms = ZERO_MS;
            break;
        default:
            break;
    }

    led_ptr->status.iterations_counter = 0;
    led_ptr->status.on_timeout = led_ptr->setup.on_ms;
    led_ptr->status.off_timeout = led_ptr->setup.off_ms;
}
/******************************************************************************/




/**
 * @brief          Loading led animation setup
 */
void indication_led_loading_setup(mculed_t *led_ptr, int led_index)
{
	led_ptr->hardware.mode = MCULED_LED_LOADING;
	led_ptr->setup.iterations_num = INDICATION_LED_LOADING_NUM;

	switch (led_index) {
		case LED_TOP:
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TOP_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TOP_TIME_OFF);
			led_ptr->setup.delay_ms = (INDICATION_LED_SPEED_MIDDLE * (LED_RIGHT_TIME_OFF - LED_TOP_TIME_ON));
			break;
		case LED_LEFT:
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_LEFT_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_LEFT_TIME_OFF);
			led_ptr->setup.delay_ms = (INDICATION_LED_SPEED_MIDDLE * (LED_RIGHT_TIME_OFF - LED_LEFT_TIME_ON));
			break;
		case LED_BOTTOM:
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_BOTTOM_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_BOTTOM_TIME_OFF);
			led_ptr->setup.delay_ms = (INDICATION_LED_SPEED_MIDDLE * (LED_RIGHT_TIME_OFF - LED_BOTTOM_TIME_ON));
			break;
		case LED_RIGHT:
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_RIGHT_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_RIGHT_TIME_OFF);
			led_ptr->setup.delay_ms = (INDICATION_LED_SPEED_MIDDLE * (LED_RIGHT_TIME_OFF - LED_RIGHT_TIME_ON));
			break;
		default:
			break;
 	}

	led_ptr->status.iterations_counter = 0;
	led_ptr->status.on_timeout = led_ptr->setup.on_ms;
	led_ptr->status.off_timeout = led_ptr->setup.off_ms;
}




/**
 * @brief          Button led setup
 */
void indication_led_button_setup(mculed_t *led_ptr, int led_index)
{
	switch (led_index) {
		case LED_TOP:
			led_ptr->hardware.mode = MCULED_ON_STATE;
			led_ptr->setup.iterations_num = INDICATION_LED_BUTTON_NUM;
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TOP_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TOP_TIME_OFF);
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		case LED_LEFT:
			led_ptr->hardware.mode = MCULED_OFF_STATE;
			led_ptr->setup.on_ms = ZERO_MS;
			led_ptr->setup.off_ms = ZERO_MS;
			break;
		case LED_BOTTOM:
			led_ptr->hardware.mode = MCULED_OFF_STATE;
			led_ptr->setup.on_ms = ZERO_MS;
			led_ptr->setup.off_ms = ZERO_MS;
			break;
		case LED_RIGHT:
			led_ptr->hardware.mode = MCULED_OFF_STATE;
			led_ptr->setup.on_ms = ZERO_MS;
			led_ptr->setup.off_ms = ZERO_MS;
			break;
		default:
			break;
 	}

	led_ptr->status.iterations_counter = 0;
	led_ptr->status.on_timeout = led_ptr->setup.on_ms;
	led_ptr->status.off_timeout = led_ptr->setup.off_ms;
}




/**
 * @brief          Button hold led setup
 */
void indication_led_button_hold_setup(mculed_t *led_ptr, int led_index)
{
	switch (led_index) {
		case LED_TOP:
			led_ptr->hardware.mode = MCULED_OFF_STATE;
			led_ptr->setup.on_ms = ZERO_MS;
			led_ptr->setup.off_ms = ZERO_MS;
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		case LED_LEFT:
			led_ptr->hardware.mode = MCULED_ON_STATE;
			led_ptr->setup.iterations_num = INDICATION_LED_BUTTON_NUM;
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_OFF);
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		case LED_BOTTOM:
			led_ptr->hardware.mode = MCULED_ON_STATE;
			led_ptr->setup.iterations_num = INDICATION_LED_BUTTON_NUM;
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_OFF);
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		case LED_RIGHT:
			led_ptr->hardware.mode = MCULED_ON_STATE;
			led_ptr->setup.iterations_num = INDICATION_LED_BUTTON_NUM;
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_OFF);
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		default:
			break;
 	}

	led_ptr->status.iterations_counter = 0;
	led_ptr->status.on_timeout = led_ptr->setup.on_ms;
	led_ptr->status.off_timeout = led_ptr->setup.off_ms;
}




void indication_led_button_double_click_setup(mculed_t *led_ptr, int led_index)
{
	switch (led_index) {
		case LED_TOP:
			led_ptr->hardware.mode = MCULED_OFF_STATE;
			led_ptr->setup.on_ms = ZERO_MS;
			led_ptr->setup.off_ms = ZERO_MS;
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		case LED_LEFT:
			led_ptr->hardware.mode = MCULED_ON_STATE;
			led_ptr->setup.iterations_num = INDICATION_LED_BUTTON_NUM;
			led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_ON);
			led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_OFF);
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		case LED_BOTTOM:
			led_ptr->hardware.mode = MCULED_OFF_STATE;
			led_ptr->setup.on_ms = ZERO_MS;
			led_ptr->setup.off_ms = ZERO_MS;
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		case LED_RIGHT:
			led_ptr->hardware.mode = MCULED_OFF_STATE;
			led_ptr->setup.on_ms = ZERO_MS;
			led_ptr->setup.off_ms = ZERO_MS;
			led_ptr->setup.delay_ms = ZERO_MS;
			break;
		default:
			break;
 	}

	led_ptr->status.iterations_counter = 0;
	led_ptr->status.on_timeout = led_ptr->setup.on_ms;
	led_ptr->status.off_timeout = led_ptr->setup.off_ms;
}




/**
 * @brief          Error led setup
 */
void indication_led_error_setup(mculed_t *led_ptr)
{
	led_ptr->hardware.mode = MCULED_ON_STATE;
	led_ptr->setup.iterations_num = INDICATION_LED_ERROR_NUM;

	led_ptr->setup.on_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_ON);
	led_ptr->setup.off_ms = (INDICATION_LED_SPEED_MIDDLE * LED_TIME_OFF);
	led_ptr->setup.delay_ms = ZERO_MS;

	led_ptr->status.iterations_counter = 0;
	led_ptr->status.on_timeout = led_ptr->setup.on_ms;
	led_ptr->status.off_timeout = led_ptr->setup.off_ms;
}




/**
 * @brief          Led update from SysTick_Handler
 */
void indication_leds_update(void)
{
	for (int led_index = 0; led_index < N_LED; led_index++) {
		led_update(&mculed[led_index]);
	}
}
/******************************************************************************/
