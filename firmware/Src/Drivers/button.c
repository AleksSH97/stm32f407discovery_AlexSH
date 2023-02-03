/**
 ******************************************************************************
 * @file           : button.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Button usage file
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "button.h"
#include "log.h"
#include "indication.h"
#include "uart.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
struct user_button {
	enum button_mode mode;
	bool activate;
	uint8_t held_pressed_counter;
	struct timeout debounce_timeout;
};

/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
static void button_check_algorithm(struct user_button *user_button_ptr);
static void button_activate(struct user_button *user_button_ptr);

struct user_button user_button;
//struct msg msg;




/**
 * @brief          Button initialization (CLK, GPIO, NVIC)
 */
void initialize_button(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

   __HAL_RCC_GPIOH_CLK_ENABLE();
   __HAL_RCC_GPIOD_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();

   HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(EXTI0_IRQn);

   GPIO_InitStruct.Pin = BUTTON_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);
}
/******************************************************************************/



void ButtonTaskStart(void *argumet)
{
    uint8_t ddd[128] = {0};
    UNUSED(ddd);

    for (;;)
    {
        button_check_mode();
        osDelay(1);
    }
}
/**
 * @brief          Button bool pushed status. Returns false if button has not been pushed,
 *                 returns 1 if button has been pushed
 */
bool button_is_pushed(void)
{
	if (!timeout_started(&user_button.debounce_timeout)) {
		return false;
	}

	if (!timeout_elapsed(&user_button.debounce_timeout)) {
		return false;
	}

	return HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET;
}
/******************************************************************************/




/**
 * @brief          Button activation from EXTI Callback
 */
void button_activate(struct user_button *user_button_ptr)
{
	user_button_ptr->activate = true;
}
/******************************************************************************/




/**
 * @brief          This functions calls from SysTick constantly to check button mode
 */
void button_check_mode(void)
{
	button_check_algorithm(&user_button);
}
/******************************************************************************/




/**
 * @brief          Button check algorithm (for singlne, double and hold modes)
 */
void button_check_algorithm(struct user_button *user_button_ptr)
{
	if (user_button_ptr->activate) {
		if (!timeout_started(&user_button.debounce_timeout)) {
			user_button_ptr->mode = BUTTON_ONE_CLICK;
			user_button_ptr->activate = false;
			timeout_start(&user_button.debounce_timeout, BUTTON_DEBOUNCE_TIME_MS);
		}
		else {
			user_button_ptr->mode = BUTTON_DOUBLE_CLICK;
		}
	}

	if ((timeout_started(&user_button.debounce_timeout)) && (timeout_check(&user_button.debounce_timeout, (5 * user_button_ptr->held_pressed_counter)))) {
		if (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET) {
			++user_button_ptr->held_pressed_counter;
		}
		user_button_ptr->mode = (user_button_ptr->held_pressed_counter >= 71) ? BUTTON_HELD_PRESSED : user_button_ptr->mode;
	}

	if ((timeout_started(&user_button.debounce_timeout)) && (timeout_check(&user_button.debounce_timeout, 350))) {
		switch (user_button_ptr->mode) {
		    case BUTTON_ONE_CLICK:
		        if(!uart_send_byte(&huart3, 0x02)) {
		            indication_led_error();
		        }
		    	indication_led_button();
			   	break;
		    case BUTTON_DOUBLE_CLICK:
			 	indication_led_button_double_click();
			   	break;
			case BUTTON_HELD_PRESSED:
			    indication_led_button_hold();
			    break;
		}
		user_button_ptr->held_pressed_counter = 0;
		timeout_stop(&user_button.debounce_timeout);
		user_button_ptr->activate = false;
	}
}
/******************************************************************************/




/**
 * @brief          EXTI0 IRQHandler - for interrupt with button
 */
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(BUTTON_Pin);
}
/******************************************************************************/




/**
 * @brief          Callback of EXTI0
 */
void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
	if (gpio_pin == BUTTON_Pin) {
		button_activate(&user_button);
	}
}
/******************************************************************************/
