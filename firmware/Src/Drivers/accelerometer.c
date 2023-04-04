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
#include "accelerometer.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
enum accelero_status accelero_status;
struct accelero_spi accelero_spi;
static struct accelerometer_drv *accelerometer_drv_ptr;

/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
static void prvAccelerometerSetDrv(struct accelerometer_drv *accelerometer_drv);
static struct lis302dl_config prvAccelerometerSetConf(void);
static void prvAccelerometerErrorHandler(enum accelero_error error);



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
    prvAccelerometerSetDrv(&lis302dl_drv);

    if (accelerometer_drv_ptr == NULL) {
        return ACCELERO_INIT_ERROR;
    }
    if (accelerometer_drv_ptr->init == NULL || accelerometer_drv_ptr->filter_config == NULL) {
        return ACCELERO_INIT_ERROR;
    }

    struct lis302dl_config lis302dl_conf = prvAccelerometerSetConf();

    uint8_t ctrl = 0x0000;

    ctrl |= *(uint8_t*)&lis302dl_conf.power_mode;
    ctrl |= *(uint8_t*)&lis302dl_conf.output_data_rate;
    ctrl |= *(uint8_t*)&lis302dl_conf.axes_enable;
    ctrl |= *(uint8_t*)&lis302dl_conf.full_scale;
    ctrl |= *(uint8_t*)&lis302dl_conf.self_test;
    accelerometer_drv_ptr->init(ctrl);

    ctrl |= *(uint8_t*)&lis302dl_conf.highpass_filter_data_select;
    ctrl |= *(uint8_t*)&lis302dl_conf.highpass_filter_cutoff_freq;
    ctrl |= *(uint8_t*)&lis302dl_conf.highpass_filter_interrupt;
    accelerometer_drv_ptr->filter_config(ctrl);

    PrintfLogsCRLF(CLR_DEF"");
    PrintfLogsCRLF("\t\tACCELEROMETER INIT ENDED");
    PrintfLogsCRLF("");

    return ACCELERO_OK;
}
/******************************************************************************/




/**
 * @brief          Accelerometer RTOS task
 */
void AccelerometerTask(void* argument)
{
    accelero_spi.error = AccelerometerInit();

    for(;;)
    {
        if (AccelerometerGetError() != 0) {
            prvAccelerometerErrorHandler(AccelerometerGetError());
            continue;
        }

        if (AccelerometerGetStatus() == ACCELERO_XYZ) {

            if (MicrophoneGetActivate()) {
                MicrophoneSetActivate(MICROPHONE_OFF);
            }

            AcceleroLedIndication();
        }

        if (AccelerometerGetStatus() == ACCELERO_IDLE) {
            __NOP();
        }

        osDelay(1);
    }
}
/******************************************************************************/




/**
 * @brief          Accelerometer set driver pointer
 */
void prvAccelerometerSetDrv(struct accelerometer_drv *accelerometer_drv)
{
    accelerometer_drv_ptr = accelerometer_drv;
}
/******************************************************************************/




/**
 * @brief          Accelerometer set config to lis302dl
 */
struct lis302dl_config prvAccelerometerSetConf(void)
{
    struct lis302dl_config lis302dl_config = {
      .power_mode = LIS302DL_LOWPOWERMODE_ACTIVE,
      .output_data_rate = LIS302DL_DATARATE_100,
      .axes_enable = LIS302DL_XYZ_ENABLE,
      .full_scale = LIS302DL_FULLSCALE_2_3,
      .self_test = LIS302DL_SELFTEST_NORMAL,
      .highpass_filter_data_select = LIS302DL_FILTERDATASELECTION_OUTPUTREGISTER,
      .highpass_filter_cutoff_freq = LIS302DL_HIGHPASSFILTER_LEVEL_1,
      .highpass_filter_interrupt = LIS302DL_HIGHPASSFILTERINTERRUPT_1_2
    };

    return lis302dl_config;
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
 * @brief          Accelerometer get current error state
 */
enum accelero_error AccelerometerGetError(void)
{
    return accelero_spi.error;
}
/******************************************************************************/




/**
 * @brief          Accelerometer set current error state
 */
void AccelerometerSetError(enum accelero_error error)
{
    accelero_spi.error = error;
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
    if (accelerometer_drv_ptr->clear_it != NULL) {
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

    if(accelerometer_drv_ptr->get_xyz != NULL && Data_xyz_ptr != NULL) {
        accelerometer_drv_ptr->get_xyz(Data_xyz_ptr);

        /* Switch X and Y Axes in case of LIS302DL MEMS */
        if(accelerometer_drv_ptr == &lis302dl_drv && Data_xyz_ptr != NULL) {
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




/**
 * @brief          Accelerometer error handler
 */
void prvAccelerometerErrorHandler(enum accelero_error error)
{
    switch (error) {
        case ACCELERO_INIT_ERROR:
            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
            AccelerometerSetError(ACCELERO_OK);
            break;
        case ACCELERO_TX_BUFFER_READ_ERROR:
            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
            AccelerometerSetError(ACCELERO_OK);
            break;
        case ACCELERO_TX_BUFFER_WRITE_ERROR:
            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
            AccelerometerSetError(ACCELERO_OK);
            break;
        case ACCELERO_RX_BUFFER_READ_ERROR:
            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
            AccelerometerSetError(ACCELERO_OK);
            break;
        case ACCELERO_RX_BUFFER_WRITE_ERROR:
            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
            AccelerometerSetError(ACCELERO_OK);
            break;
        case ACCELERO_DELETE_TRASH_ERROR:
            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
            AccelerometerSetError(ACCELERO_OK);
            break;
        case ACCELERO_SPI_SETUP_ERROR:
            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
            AccelerometerSetError(ACCELERO_OK);
            break;
        default:
            PrintfLogsCRLF("Undefined error SPI");
            AccelerometerSetError(ACCELERO_OK);
            break;
    }
}
/******************************************************************************/
