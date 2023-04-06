/**
 ******************************************************************************
 * @file           : accelerometer.h
 * @author         : Aleksandr Shabalin    <alexnv97@gmail.com>
 * @brief          : Header file for accelerometer
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin ------------------ *
 ******************************************************************************
 * This module is a confidential and proprietary property of Aleksandr Shabalin
 * and possession or use of this module requires written permission
 * of Aleksandr Shabalin.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_


/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "lis302dl.h"
#include "accelero_spi.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/
struct accelerometer_drv {
    uint8_t      (*init)(uint16_t);
    uint8_t      (*deinit)(void);
    uint8_t      (*read_id)(void);
    uint8_t      (*reset)(void);
    uint8_t      (*low_power)(uint16_t);
    uint8_t      (*config_it)(void);
    uint8_t      (*enable_it)(uint8_t);
    uint8_t      (*disable_it)(uint8_t);
    uint8_t      (*it_status)(uint16_t);
    uint8_t      (*clear_it)(void);
    uint8_t      (*filter_config)(uint8_t);
    uint8_t      (*filter_cmd)(uint8_t);
    uint8_t      (*get_xyz)(int16_t *);
};

/* ACCELERO struct */
struct accelerometer_init {
    uint8_t Power_Mode;                         /* Power-down/Normal Mode */
    uint8_t AccOutput_DataRate;                 /* OUT data rate */
    uint8_t Axes_Enable;                        /* Axes enable */
    uint8_t High_Resolution;                    /* High Resolution enabling/disabling */
    uint8_t BlockData_Update;                   /* Block Data Update */
    uint8_t Endianness;                         /* Endian Data selection */
    uint8_t AccFull_Scale;                      /* Full Scale selection */
    uint8_t Communication_Mode;
};

/* ACCELERO High Pass Filter struct */
struct accelerometerac_filter_config {
    uint8_t HighPassFilter_Mode_Selection;      /* Internal filter mode */
    uint8_t HighPassFilter_CutOff_Frequency;    /* High pass filter cut-off frequency */
    uint8_t HighPassFilter_AOI1;                /* HPF_enabling/disabling for AOI function on interrupt 1 */
    uint8_t HighPassFilter_AOI2;                /* HPF_enabling/disabling for AOI function on interrupt 2 */
    uint8_t HighPassFilter_Data_Sel;
    uint8_t HighPassFilter_Stat;
};


/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
uint8_t AccelerometerInit(void);
void AccelerometerTask(void* argument);
void AccelerometerReset(void);

enum accelero_status AccelerometerGetStatus(void);
void AccelerometerSetStatus(enum accelero_status status);

enum accelero_error AccelerometerGetError(void);
void AccelerometerSetError(enum accelero_error error);

void AccelerometerClickItConfig(void);
void AccelerometerClickItClear(void);
void AccelerometerGetXyz(int16_t *Data_xyz_ptr);
bool AccelerometerPutDataToRxBuffer(const void* data, size_t len);
bool AccelerometerReadDataFromRxBuffer(void* data, size_t len);
bool AccelerometerPutDataToTxBuffer(const void* data, size_t len);
bool AccelerometerReadDataFromTxBuffer(void* data, size_t len);


/******************************************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* ACCELEROMETER_H_ */
