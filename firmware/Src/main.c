/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Main file
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"

static void initialize_mcu(void);
static void system_clock_config(void);
static void initialize_rtos(void);
static void initialize_drivers(void);




/**
 * @brief          Main endless cycle
 */
int main(void)
{
    initialize_mcu();
    initialize_drivers();

    log_print_welcome_msg();

    initialize_rtos();

    for(;;);
}
/******************************************************************************/




/**
 * @brief          MCU initialization fns
 */
void initialize_mcu(void)
{
    HAL_Init();
    system_clock_config();
    MX_TIM1_Init();
}
/******************************************************************************/




/**
 * @brief          FreeRTOS init fns
 */
void initialize_rtos(void)
{
    osKernelInitialize();
    MX_FREERTOS_Init();
    osKernelStart();
}
/******************************************************************************/




/**
 * @brief          Drivers init fns
 */
void initialize_drivers(void)
{
    initialize_led_indication();
    indication_led_loading();
    initialize_button();

    microphone_init();

    uart_all_init();
    ring_buf_uart_init();
    ring_buf_i2s2_init();
    uart_setup_receive_char(&huart3, &data_uart.keyboard);

    log_init();
    console_init();
}
/******************************************************************************/




/**
 * @brief          System clock configuration
 */
void system_clock_config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 84;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      Error_Handler();
    }

    HAL_RCC_EnableCSS();
}
/******************************************************************************/



/**
 * @brief          Error handler
 */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
/******************************************************************************/




void HAL_RCC_CSSCallBack(void)
{

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
