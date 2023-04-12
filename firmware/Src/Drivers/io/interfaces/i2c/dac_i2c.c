/**
 ******************************************************************************
 * @file           : dac_i2c.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : I2C of DAC.
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "dac_i2c.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
I2C_HandleTypeDef hi2c1;

/******************************************************************************/

/**
  * @brief  I2C of DAC initialization
  * @param
  * @retval None
  */
void DacI2cInit(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}




/**
  * @brief  Writes/Read a single data.
  * @param  addr: I2C address
  * @param  reg: Register address
  * @param  value: Data to be written
  * @retval None
  */
void DacI2CWriteData(uint8_t addr, uint8_t reg, uint8_t value)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write(&hi2c1, addr, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, &value, 1, DAC_I2C_TIMEOUT_MAX);

    /* Check the communication status */
    if(status != HAL_OK) {
        Error_Handler();
    }
}
/******************************************************************************/




uint8_t DacI2CReadData(uint8_t addr, uint8_t reg)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t value = 0;

    status = HAL_I2C_Mem_Read(&hi2c1, addr, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, &value, 1, DAC_I2C_TIMEOUT_MAX);

    if(status != HAL_OK) {
        Error_Handler();
    }

    return value;
}
/******************************************************************************/
