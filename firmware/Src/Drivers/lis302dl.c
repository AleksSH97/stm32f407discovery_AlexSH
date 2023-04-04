/**
 ******************************************************************************
 * @file           : lis302dl.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : LIS302dl driver
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "accelero_spi.h"
#include "lis302dl.h"
#include "accelerometer.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
struct accelerometer_drv lis302dl_drv = {
        lis302dl_init,
        lis302dl_deinit,
        lis302dl_read_id,
        lis302dl_reboot_cmd,
        lis302dl_click_int_config,
        0,
        0,
        0,
        0,
        lis302dl_click_int_clear,
        lis302dl_filter_config,
        0,
        lis302dl_get_xyz,
};

/******************************************************************************/




/**
  * @brief  Set lis302dl Initialization.
  * @param  InitStruct: contains mask of different init parameters
  * @retval None
  */
uint8_t lis302dl_init(uint16_t init_struct)
{
    uint8_t ctrl = 0x00;
    uint8_t res = ACCELERO_OK;

    AcceleroCsInit();

    ctrl = (uint8_t)init_struct;

    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CTRL_REG1_ADDR, 1);

    return res;
}
/******************************************************************************/




/**
  * @brief  Read lis302dl device ID.
  * @param  None
  * @retval The Device ID (two bytes).
  */
uint8_t lis302dl_deinit(void)
{
    // TODO deinit fn
    return ACCELERO_OK;
}
/******************************************************************************/




uint8_t lis302dl_read_id(void)
{
    uint8_t tmp = 0;

    AcceleroCsInit();

    AcceleroIoReadIT(&tmp, LIS302DL_WHO_AM_I_ADDR, 1);

    return (uint16_t)tmp;
}
/******************************************************************************/




/**
  * @brief  Set lis302dl Internal High Pass Filter configuration.
  * @param  FilterStruct: contains data for filter config
  * @retval None
  */
uint8_t lis302dl_filter_config(uint8_t filter_struct)
{
    uint8_t ctrl = 0x00;
    uint8_t res = ACCELERO_OK;

    res = AcceleroIoReadIT(&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);

    ctrl &= (uint8_t)~(LIS302DL_FILTERDATASELECTION_OUTPUTREGISTER | \
            LIS302DL_HIGHPASSFILTER_LEVEL_3 | \
            LIS302DL_HIGHPASSFILTERINTERRUPT_1_2);

    ctrl |= filter_struct;

    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);

    return res;
}
/******************************************************************************/




/**
  * @brief  Set lis302dl Interrupt configuration
  * @param  None
  * @retval None
  */
uint8_t lis302dl_click_int_config(void)
{
    uint8_t ctrl = 0x00;
    uint8_t res = ACCELERO_OK;
    struct lis302dl_interrupt_conf lis302dl_interrupt_conf;
    AcceleroIoItConfig();

    lis302dl_interrupt_conf.latch_request = LIS302DL_INTERRUPTREQUEST_LATCHED;
    lis302dl_interrupt_conf.single_click_axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
    lis302dl_interrupt_conf.double_click_axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;

    res = lis302dl_interrupt_config(&lis302dl_interrupt_conf);

    /* Configure Interrupt control register: enable Click interrupt on INT1 and
    INT2 on Z LIS302DL high event */
    ctrl = 0x3F;
    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1);

    /* Enable Interrupt generation on click on Z LIS302DL */
    ctrl = 0x50;
    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);

    /* Configure Click Threshold on X/Y LIS302DL (10 x 0.5g) */
    ctrl = 0xAA;
    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CLICK_THSY_X_REG_ADDR, 1);

    /* Configure Click Threshold on Z LIS302DL (10 x 0.5g) */
    ctrl = 0x0A;
    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CLICK_THSZ_REG_ADDR, 1);

    /* Enable interrupt on Y LIS302DL high event */
    ctrl = 0x4C;
    res = AcceleroIoWriteIT(&ctrl, LIS302DL_FF_WU_CFG1_REG_ADDR, 1);

    /* Configure Time Limit */
    ctrl = 0x03;
    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1);

    /* Configure Latency */
    ctrl = 0x7F;
    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CLICK_LATENCY_REG_ADDR, 1);

    /* Configure Click Window */
    ctrl = 0x7F;
    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CLICK_WINDOW_REG_ADDR, 1);

    return res;
}
/******************************************************************************/




/**
  * @brief  Perform lis302dl Interrupt configuration
  * @param  pointer to struct LIS302DL_spi with parameters
  * @retval None
  */
uint8_t lis302dl_interrupt_config(struct lis302dl_interrupt_conf *lis302dl_interrupt_conf_ptr)
{
    uint8_t ctrl = 0x00;
    uint8_t res = ACCELERO_OK;

    res = AcceleroIoReadIT(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);

    ctrl = (uint8_t)(lis302dl_interrupt_conf_ptr->latch_request| \
            lis302dl_interrupt_conf_ptr->single_click_axes | \
            lis302dl_interrupt_conf_ptr->double_click_axes);

    res = AcceleroIoWriteIT(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);

    return res;
}
/******************************************************************************/




/**
  * @brief  Clear lis302dl click Interrupt
  * @param  None
  * @retval None
  */
uint8_t lis302dl_click_int_clear(void)
{
    uint8_t buffer[6], clickreg = 0;
    uint8_t res = ACCELERO_OK;

    res = AcceleroIoReadIT(&clickreg, LIS302DL_CLICK_SRC_REG_ADDR, 1);
    res = AcceleroIoReadIT(buffer, LIS302DL_STATUS_REG_ADDR, 6);

    return res;
}
/******************************************************************************/




/**
  * @brief  Change the lowpower mode for lis302dl
  * @param  low_power_mode: New state for the low power mode.
  * This parameter can be one of the following values:
  * @arg LIS302DL_LOWPOWERMODE_POWERDOWN: Power down mode
  * @arg LIS302DL_LOWPOWERMODE_ACTIVE: Active mode
  * @retval None
  */
uint8_t lis302dl_low_power_cmd(uint8_t low_power_mode)
{
    uint8_t tmpreg;
    uint8_t res = ACCELERO_OK;

    res = AcceleroIoReadIT(&tmpreg, LIS302DL_CTRL_REG1_ADDR, 1);

    /* Set new low power mode configuration */
    tmpreg &= (uint8_t)~LIS302DL_LOWPOWERMODE_ACTIVE;
    tmpreg |= low_power_mode;

    res = AcceleroIoWriteIT(&tmpreg, LIS302DL_CTRL_REG1_ADDR, 1);

    return res;
}
/******************************************************************************/




/**
  * @brief  Data Rate command
  * @param  data_rate_value: Data rate value
  * This parameter can be one of the following values:
  * @arg LIS302DL_DATARATE_100: 100 Hz output data rate
  * @arg LIS302DL_DATARATE_400: 400 Hz output data rate
  * @retval None
  */
uint8_t lis302dl_data_rate_cmd(uint8_t data_rate_value)
{
    uint8_t tmpreg;
    uint8_t res = ACCELERO_OK;

    res = AcceleroIoReadIT(&tmpreg, LIS302DL_CTRL_REG1_ADDR, 1);

    tmpreg &= (uint8_t)~LIS302DL_DATARATE_400;
    tmpreg |= data_rate_value;

    res = AcceleroIoWriteIT(&tmpreg, LIS302DL_CTRL_REG1_ADDR, 1);

    return res;
}
/******************************************************************************/




/**
  * @brief  Change the Full Scale of lis302dl
  * @param  fs_value: new full scale value.
  * This parameter can be one of the following values:
  * @arg LIS302DL_FULLSCALE_2_3: +-2.3g
  * @arg LIS302DL_FULLSACLE_9_2: +-9.2g
  * @retval None
  */
uint8_t lis302dl_full_scale_cmd(uint8_t fs_value)
{
    uint8_t tmpreg;
    uint8_t res = ACCELERO_OK;

    res = AcceleroIoReadIT(&tmpreg, LIS302DL_CTRL_REG1_ADDR, 1);

    tmpreg &= (uint8_t)~LIS302DL_FULLSACLE_9_2;
    tmpreg |= fs_value;

    res = AcceleroIoWriteIT(&tmpreg, LIS302DL_CTRL_REG1_ADDR, 1);

    return res;
}
/******************************************************************************/




/**
  * @brief  Reboot memory content of lis302dl.
  * @param  None
  * @retval None
  */
uint8_t lis302dl_reboot_cmd(void)
{
    uint8_t tmpreg;
    uint8_t res = ACCELERO_OK;

    res = AcceleroIoReadIT(&tmpreg, LIS302DL_CTRL_REG2_ADDR, 1);

    tmpreg |= LIS302DL_BOOT_REBOOTMEMORY;

    res = AcceleroIoWriteIT(&tmpreg, LIS302DL_CTRL_REG2_ADDR, 1);

    return res;
}
/******************************************************************************/




/**
  * @brief  Read lis302dl output register, and calculate the acceleration
  *         ACC[mg]=SENSITIVITY* (out_h*256+out_l)/16 (12 bit representation)
  * @param  pfData: Data out pointer
  * @retval None
  */
uint8_t lis302dl_get_xyz(int16_t *pData)
{
    int8_t  buffer[6];
    int16_t pn_raw_data[3];
    uint8_t sensitivity = LIS302DL_SENSITIVITY_2_3G;
    uint8_t ctrl;
    uint8_t res = ACCELERO_OK;

    res = AcceleroIoReadIT(&ctrl, LIS302DL_CTRL_REG1_ADDR, 1);
    res = AcceleroIoReadIT((uint8_t*)buffer, LIS302DL_OUT_X_ADDR, 6);

    for (uint8_t i = 0; i < 3; i++) { // TODO get rid of magic numbers
        pn_raw_data[i] = buffer[2 * i];
    }

    switch (ctrl & LIS302DL_FULLSACLE_9_2) {
        case LIS302DL_FULLSCALE_2_3:
            sensitivity = LIS302DL_SENSITIVITY_2_3G;
            break;
        case LIS302DL_FULLSACLE_9_2:
            sensitivity = LIS302DL_SENSITIVITY_9_2G;
            break;
        default:
            break;
    }

    for (int i = 0; i < 3; i++) { // TODO get rid of magic numbers
        pData[i] = (pn_raw_data[i] * sensitivity);
    }

    return res;
}
/******************************************************************************/
