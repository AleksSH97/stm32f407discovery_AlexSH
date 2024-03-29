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
/***************************   START BOARD PINNING   **************************/
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
/* USART3
 * PB11 - USART3 RX /////////////////
 * PC10 - USART3 TX /////////////////
 *
 * I2S2
 * PB12 - I2S2 WS  /////////////////
 * PB10 - I2S2 CK  /////////////////
 * PC3  - I2S2 SD  /////////////////
 *
 * I2S3
 * PA4  - I2S3 WS  /////////////////
 * PB3  - I2S3 CK  /////////////////
 * PC12 - I2S3 SD  /////////////////
 * PC7  - I2S3 MCK /////////////////
 *
 * SPI1
 * PA5  - SPI1 SCK /////////////////
 * PA6  - SPI1 MISO ////////////////
 * PA7  - SPI1 MOSI ////////////////
 * PE3  - Chip Select //////////////
 * PE1  - INT2 /////////////////////
 *
 * SYSTEM
 * PA14 - SWCLK ////////////////////
 * PA13 - SWDIO ////////////////////
 *
 * USER
 * PD15 - LED_BOTTOM ///////////////
 * PD14 - LED_RIGHT ////////////////
 * PD13 - LED_TOP   ////////////////
 * PD12 - LED_LEFT /////////////////
 * PE0  - BUTTON   /////////////////
 *//////////////////////////////////////////////////////////////////////////////

/******************************************************************************/
/***************************   END BOARD PINNING   **************************/
/******************************************************************************/

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"


/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
static void prvInitializeMCU(void);
static void prvSystemClockConfig(void);

/******************************************************************************/


/**
 * @brief          Main endless cycle
 */
int main(void)
{
    prvInitializeMCU();
    osKernelInitialize();
    IoSystemInit();
    MX_FREERTOS_Init();
    osKernelStart();

    for(;;) {}
}
/******************************************************************************/




/**
 * @brief          MCU initialization fns
 */
void prvInitializeMCU(void)
{
    HAL_Init();
    prvSystemClockConfig();
    MX_TIM1_Init();
}
/******************************************************************************/




/**
 * @brief          System clock configuration
 */
void prvSystemClockConfig(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
      Error_Handler();
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
    PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
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
