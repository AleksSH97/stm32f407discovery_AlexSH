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
enum accelero_status accelero_status;
struct accelero_spi accelero_spi;
static struct accelerometer_drv *accelerometer_drv_ptr;

/******************************************************************************/




/**
 * @brief          Accelerometer initialization
 */
uint8_t AccelerometerInit(void)
{
    PrintfLogsCRLF(CLR_DEF"");
    PrintfLogsCRLF("\t\tACCELEROMETER INIT STARTED");
    PrintfLogsCRLF("");

    AcceleroIoItConfig();
    AcceleroSpiInit();

    uint8_t ret = ACCELERO_INIT_ERROR;
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

    PrintfLogsCRLF(CLR_GR"ACCELEROMETER INIT CTRL: %d", ctrl);

    accelerometer_drv_ptr->init(ctrl);

    lis302dl_filter.highpass_filter_data_select = LIS302DL_FILTERDATASELECTION_OUTPUTREGISTER;
    lis302dl_filter.highpass_filter_cutoff_freq = LIS302DL_HIGHPASSFILTER_LEVEL_1;
    lis302dl_filter.highpass_filter_interrupt = LIS302DL_HIGHPASSFILTERINTERRUPT_1_2;

    ctrl = (uint8_t)(lis302dl_filter.highpass_filter_data_select | \
                     lis302dl_filter.highpass_filter_cutoff_freq | \
                     lis302dl_filter.highpass_filter_interrupt);

    PrintfLogsCRLF(CLR_GR"ACCELEROMETER FILTER CONFIG CTRL: %d", ctrl);

    accelerometer_drv_ptr->filter_config(ctrl);

    ret = ACCELERO_OK;

    PrintfLogsCRLF(CLR_DEF"");
    PrintfLogsCRLF("\t\tACCELEROMETER INIT ENDED");
    PrintfLogsCRLF("");

    return ret;
}
/******************************************************************************/




/**
 * @brief          Accelerometer RTOS task
 */
void AccelerometerTask(void* argument)
{
    AccelerometerInit();

    if (!lwrb_is_ready(&accelero_spi.lwrb_rx)) {
        PrintfLogsCRLF("Error ring buf SPI rx init");
    }
    if (!lwrb_is_ready(&accelero_spi.lwrb_tx)) {
        PrintfLogsCRLF("Error ring buf SPI tx init");
    }

    for(;;)
    {
        if (AccelerometerGetStatus() == ACCELERO_XYZ) {

            if (MicrophoneGetStatus() != MICROPHONE_DEINIT) {
                MicrophoneSetStatus(MICROPHONE_DEINIT);
            }

            AcceleroLedIndication();
        }

        if (AccelerometerGetStatus() == ACCELERO_OK) {
            __NOP();
        }

        if (AccelerometerGetStatus() == ACCELERO_INIT_ERROR) {
            PrintfLogsCRLF("ERROR INIT ACCELEROMETER");
        }

        if (AccelerometerGetStatus() == ACCELERO_BLOCKED) {
            osDelay(1);
        }

        osDelay(1);
    }
}
/******************************************************************************/




/**
 * @brief          Accelerometer get current status
 */
enum accelero_status AccelerometerGetStatus(void)
{
    return accelero_spi.status;
}
/******************************************************************************/




/**
 * @brief          Accelerometer set current status
 */
void AccelerometerSetStatus(enum accelero_status status)
{
    accelero_spi.status = status;
}
/******************************************************************************/



/**
 * @brief          Accelerometer reset
 */
void AccelerometerReset(void)
{
     if (accelerometer_drv_ptr->reset != NULL) {
         accelerometer_drv_ptr->reset();
     }
}
/******************************************************************************/




/**
 * @brief          Accelerometer IT configuration
 */
void AccelerometerClickItConfig(void)
{
    if (accelerometer_drv_ptr->config_it != NULL) {
        accelerometer_drv_ptr->config_it();
    }
}
/******************************************************************************/




/**
 * @brief          Accelerometer IT clear
 */
void AccelerometerClickItClear(void)
{
  if (accelerometer_drv_ptr->clear_it != NULL)
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




/**
 * @brief          Put data to RX ring buffer
 */
bool AccelerometerPutDataToRxBuffer(const void* data, size_t len)
{
    if (lwrb_get_free(&accelero_spi.lwrb_rx) == 0) {
        return false;
    }

    return (lwrb_write(&accelero_spi.lwrb_rx, data, len) > 0 ? true : false);
}
/******************************************************************************/




/**
 * @brief          Read data from RX ring buffer
 */
bool AccelerometerReadDataFromRxBuffer(void* data, size_t len)
{
    if (lwrb_get_free(&accelero_spi.lwrb_rx) == 0) {
        return false;
    }

    return (lwrb_read(&accelero_spi.lwrb_rx, data, len) > 0 ? true : false);
}
/******************************************************************************/




/**
 * @brief          Put data to TX ring buffer
 */
bool AccelerometerPutDataToTxBuffer(const void* data, size_t len)
{
    if (lwrb_get_free(&accelero_spi.lwrb_tx) == 0) {
        return false;
    }

    return (lwrb_write(&accelero_spi.lwrb_tx, data, len) > 0 ? true : false);
}
/******************************************************************************/




/**
 * @brief          Read data from TX ring buffer
 */
bool AccelerometerReadDataFromTxBuffer(void* data, size_t len)
{
    if (lwrb_get_free(&accelero_spi.lwrb_tx) == 0) {
        return false;
    }

    return (lwrb_read(&accelero_spi.lwrb_tx, data, len) > 0 ? true : false);
}
/******************************************************************************/
