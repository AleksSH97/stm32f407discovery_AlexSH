/*
 * accelerometer.h
 *
 *  Created on: 13 янв. 2023 г.
 *      Author: АлексанDOOR
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "lis302dl.h"

enum accelerometer_status
{
    ACCELEROMETER_OK = 0,
    ACCELEROMETER_ERROR = 1,
    ACCELEROMETER_TIMEOUT = 2
};

struct accelerometer_drv {
    void      (*init)(uint16_t);
    void      (*deinit)(void);
    uint8_t   (*read_id)(void);
    void      (*reset)(void);
    void      (*low_power)(uint16_t);
    void      (*config_it)(void);
    void      (*enable_it)(uint8_t);
    void      (*disable_it)(uint8_t);
    uint8_t   (*it_status)(uint16_t);
    void      (*clear_it)(void);
    void      (*filter_config)(uint8_t);
    void      (*filter_cmd)(uint8_t);
    void      (*get_xyz)(int16_t *);
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

uint8_t AccelerometerInit(void);
void AccelerometerReset(void);
void AccelerometerClickItConfig(void);
void AccelerometerClickItClear(void);
void AccelerometerGetXyz(int16_t *Data_xyz_ptr);

#endif /* ACCELEROMETER_H_ */
