/**
 ******************************************************************************
 * @file           : accelerometer.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Accelerometer driver file
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "main.h"
#include "accelerometer.h"
#include "accelero_spi.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
enum accelerometer_status accelerometer_status;
struct accelero_spi accelero_spi;
static struct accelerometer_drv *accelerometer_drv_ptr;

/******************************************************************************/




/**
 * @brief          Accelerometer initialization
 */
uint8_t AccelerometerInit(void)
{
    AcceleroIoItConfig();
    AcceleroSpiInit();

    uint8_t ret = ACCELEROMETER_ERROR;
    uint8_t ctrl = 0x0000;

    struct lis302dl_init_conf lis302dl_init;
    struct lis302dl_filter_conf lis302dl_filter;

    accelerometer_drv_ptr = &lis302dl_drv;

    lis302dl_init.power_mode = LIS302DL_LOWPOWERMODE_ACTIVE;
    lis302dl_init.output_data_rate = LIS302DL_DATARATE_100;
    lis302dl_init.axes_enable = LIS302DL_XYZ_ENABLE;
    lis302dl_init.full_scale = LIS302DL_FULLSCALE_2_3;
    lis302dl_init.self_test = LIS302DL_SELFTEST_NORMAL;

    ctrl = (uint16_t)(lis302dl_init.output_data_rate | lis302dl_init.power_mode | \
                      lis302dl_init.full_scale | lis302dl_init.self_test | \
                      lis302dl_init.axes_enable);

    accelerometer_drv_ptr->init(ctrl);

    lis302dl_filter.highpass_filter_data_select = LIS302DL_FILTERDATASELECTION_OUTPUTREGISTER;
    lis302dl_filter.highpass_filter_cutoff_freq = LIS302DL_HIGHPASSFILTER_LEVEL_1;
    lis302dl_filter.highpass_filter_interrupt = LIS302DL_HIGHPASSFILTERINTERRUPT_1_2;

    ctrl = (uint8_t)(lis302dl_filter.highpass_filter_data_select | \
                     lis302dl_filter.highpass_filter_cutoff_freq | \
                     lis302dl_filter.highpass_filter_interrupt);

    accelerometer_drv_ptr->filter_config(ctrl);

    ret = ACCELEROMETER_OK;

    return ret;
}
/******************************************************************************/


/**
 * @brief          Accelerometer RTOS task
 */
void AccelerometerTask(void* argument)
{
    for(;;)
    {
        if (accelero_spi.enabled) {
            AcceleroLedIndication();
        }
    }
}
/******************************************************************************/




/**
 * @brief          Accelerometer reset fn
 */
void AccelerometerReset(void)
{
     if (accelerometer_drv_ptr->reset != NULL) {
         accelerometer_drv_ptr->reset();
     }
}
/******************************************************************************/




/**
 * @brief          Accelerometer IT conf fn
 */
void AccelerometerClickItConfig(void)
{
    if (accelerometer_drv_ptr->config_it != NULL) {
        accelerometer_drv_ptr->config_it();
    }
}
/******************************************************************************/




/**
 * @brief          Accelerometer IT clear fn
 */
void AccelerometerClickItClear(void)
{
  if(accelerometer_drv_ptr->clear_it != NULL)
  {
      accelerometer_drv_ptr->clear_it();
  }
}
/******************************************************************************/




/**
  * @brief  Get XYZ axes acceleration.
  * @param  Data_xyz_ptr: Pointer to 3 angular acceleration axes.
  * Data_xyz_ptr[0] = X axis, Data_xyz_ptr[1] = Y axis, Data_xyz_ptr[2] = Z axis
  * @retval None
  */
void AccelerometerGetXyz(int16_t *Data_xyz_ptr)
{
  int16_t switch_xy = 0;

  if(accelerometer_drv_ptr->get_xyz != NULL)
  {
      accelerometer_drv_ptr->get_xyz(Data_xyz_ptr);

    /* Switch X and Y Axes in case of LIS302DL MEMS */
    if(accelerometer_drv_ptr == &lis302dl_drv)
    {
      switch_xy  = Data_xyz_ptr[0];
      Data_xyz_ptr[0] = Data_xyz_ptr[1];
      /* Invert Y Axis to be compliant with LIS3DSH MEMS */
      Data_xyz_ptr[1] = -switch_xy;
    }
  }
}
/******************************************************************************/
