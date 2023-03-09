/**
 ******************************************************************************
 * @file           : lis320dl.h
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Header for LIS302DL
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

#ifndef LIS302DL_H_
#define LIS302DL_H_

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "main.h"

/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/

/*******************************************************************************
*  WHO_AM_I Register: Device Identification Register
*  Read only register
*  Default value: 0x3B
*******************************************************************************/
#define LIS302DL_WHO_AM_I_ADDR                  0x0F

/*******************************************************************************
*  CTRL_REG1 Register: Control Register 1
*  Read Write register
*  Default value: 0x07
*  7 DR: Data Rate selection.
*        0 - 100 Hz output data rate
*        1 - 400 Hz output data rate
*  6 PD: Power Down control.
*        0 - power down mode
*        1 - active mode
*  5 FS: Full Scale selection.
*        0 - Typical measurement range 2.3
*        1 - Typical measurement range 9.2
*  4:3 STP-STM Self Test Enable:
*              STP |  STM |   mode
*            ----------------------------
*               0  |  0   |   Normal mode
*               0  |  1   |   Self Test M
*               1  |  0   |   Self Test P
*  2 Zen: Z LIS302DL enable.
*         0 - Z LIS302DL disabled
*         1- Z LIS302DL enabled
*  1 Yen: Y LIS302DL enable.
*         0 - Y LIS302DL disabled
*         1- Y LIS302DL enabled
*  0 Xen: X LIS302DL enable.
*         0 - X LIS302DL disabled
*         1- X LIS302DL enabled
*******************************************************************************/
#define LIS302DL_CTRL_REG1_ADDR                 0x20

/*******************************************************************************
*  CTRL_REG2 Regsiter: Control Register 2
*  Read Write register
*  Default value: 0x00
*  7 SIM: SPI Serial Interface Mode Selection.
*         0 - 4 wire interface
*         1 - 3 wire interface
*  6 BOOT: Reboot memory content
*          0 - normal mode
*          1 - reboot memory content
*  5 Reserved
*  4 FDS: Filtered data selection.
*         0 - internal filter bypassed
*         1 - data from internal filter sent to output register
*  3 HP FF_WU2: High pass filter enabled for FreeFall/WakeUp#2.
*               0 - filter bypassed
*               1 - filter enabled
*  2 HP FF_WU1: High pass filter enabled for FreeFall/WakeUp#1.
*               0 - filter bypassed
*               1 - filter enabled
*  1:0 HP coeff2-HP coeff1 High pass filter cut-off frequency (ft) configuration.
*                 ft= ODR[hz]/6*HP coeff
*            HP coeff2 | HP coeff1 |   HP coeff
*            -------------------------------------------
*                 0     |     0     |   8
*                 0     |     1     |   16
*                 1     |     0     |   32
*                 1     |     1     |   64
*            HP coeff |  ft[hz]   |  ft[hz]   |
*                     |ODR 100Hz | ODR 400Hz  |
*            --------------------------------------------
*              00     |    2      |     8     |
*              01     |    1      |     4     |
*              10     |    0.5    |     2     |
*              11     |    0.25   |     1     |
*******************************************************************************/
#define LIS302DL_CTRL_REG2_ADDR              0x21

/*******************************************************************************
*  CTRL_REG3 Register: Interrupt Control Register
*  Read Write register
*  Default value: 0x00
*  7 IHL active: Interrupt active high/low.
*                0 - active high
*                1 - active low
*  6 PP_OD: push-pull/open-drain.
*           0 - push-pull
*           1 - open-drain
*  5:3 I2_CFG2 - I2_CFG0 Data signal on INT2 pad control bits
*  2:0 I1_CFG2 - I1_CFG0 Data signal on INT1 pad control bits
*        I1(2)_CFG2  |  I1(2)_CFG1  |  I1(2)_CFG0  | INT1(2) Pad
*        ----------------------------------------------------------
*              0     |      0       |       0      | GND
*              0     |      0       |       1      | FreeFall/WakeUp#1
*              0     |      1       |       0      | FreeFall/WakeUp#2
*              0     |      1       |       1      | FreeFall/WakeUp#1 or FreeFall/WakeUp#2
*              1     |      0       |       0      | Data ready
*              1     |      1       |       1      | Click interrupt
*******************************************************************************/
#define LIS302DL_CTRL_REG3_ADDR              0x22

/*******************************************************************************
*  HP_FILTER_RESET Register: Dummy register. Reading at this address zeroes
*  instantaneously the content of the internal high pass filter. If the high pass
*  filter is enabled all three axes are instantaneously set to 0g.
*  This allows to overcome the settling time of the high pass filter.
*  Read only register
*  Default value: Dummy
*******************************************************************************/
#define LIS302DL_HP_FILTER_RESET_REG_ADDR     0x23

/*******************************************************************************
*  STATUS_REG Register: Status Register
*  Default value: 0x00
*  7 ZYXOR: X, Y and Z LIS302DL data overrun.
*           0: no overrun has occurred
*           1: new data has overwritten the previous one before it was read
*  6 ZOR: Z LIS302DL data overrun.
*         0: no overrun has occurred
*         1: new data for Z-LIS302DL has overwritten the previous one before it was read
*  5 yOR: y LIS302DL data overrun.
*         0: no overrun has occurred
*         1: new data for y-LIS302DL has overwritten the previous one before it was read
*  4 XOR: X LIS302DL data overrun.
*         0: no overrun has occurred
*         1: new data for X-LIS302DL has overwritten the previous one before it was read
*  3 ZYXDA: X, Y and Z LIS302DL new data available
*           0: a new set of data is not yet available
*           1: a new set of data is available
*  2 ZDA: Z LIS302DL new data available.
*         0: a new set of data is not yet available
*         1: a new data for Z LIS302DL is available
*  1 YDA: Y LIS302DL new data available
*         0: a new set of data is not yet available
*         1: a new data for Y LIS302DL is available
*  0 XDA: X LIS302DL new data available
*         0: a new set of data is not yet available
*         1: a new data for X LIS302DL is available
*******************************************************************************/
#define LIS302DL_STATUS_REG_ADDR             0x27

/*******************************************************************************
*  OUT_X Register: X-LIS302DL output Data
*  Read only register
*  Default value: output
*  7:0 XD7-XD0: X-LIS302DL output Data
*******************************************************************************/
#define LIS302DL_OUT_X_ADDR                  0x29

/*******************************************************************************
*  OUT_Y Register: Y-LIS302DL output Data
*  Read only register
*  Default value: output
*  7:0 YD7-YD0: Y-LIS302DL output Data
*******************************************************************************/
#define LIS302DL_OUT_Y_ADDR                  0x2B

/*******************************************************************************
*  OUT_Z Register: Z-LIS302DL output Data
*  Read only register
*  Default value: output
*  7:0 ZD7-ZD0: Z-LIS302DL output Data
*******************************************************************************/
#define LIS302DL_OUT_Z_ADDR                  0x2D

/*******************************************************************************
*  FF_WW_CFG_1 Register: Configuration register for Interrupt 1 source.
*  Read write register
*  Default value: 0x00
*  7 AOI: AND/OR combination of Interrupt events.
*         0: OR combination of interrupt events
*         1: AND combination of interrupt events
*  6 LIR: Latch/not latch interrupt request
*         0: interrupt request not latched
*         1: interrupt request latched
*  5 ZHIE: Enable interrupt generation on Z high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  4 ZLIE: Enable interrupt generation on Z low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*  3 YHIE: Enable interrupt generation on Y high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  2 YLIE: Enable interrupt generation on Y low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*  1 XHIE: Enable interrupt generation on X high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  0 XLIE: Enable interrupt generation on X low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*******************************************************************************/
#define LIS302DL_FF_WU_CFG1_REG_ADDR         0x30

/*******************************************************************************
*  FF_WU_SRC_1 Register: Interrupt 1 source register.
*  Reading at this address clears FF_WU_SRC_1 register and the FF, WU 1 interrupt
*  and allow the refreshment of data in the FF_WU_SRC_1 register if the latched option
*  was chosen.
*  Read only register
*  Default value: 0x00
*  7 Reserved
*  6 IA: Interrupt active.
*        0: no interrupt has been generated
*        1: one or more interrupts have been generated
*  5 ZH: Z high.
*        0: no interrupt
*        1: ZH event has occurred
*  4 ZL: Z low.
*        0: no interrupt
*        1: ZL event has occurred
*  3 YH: Y high.
*        0: no interrupt
*        1: YH event has occurred
*  2 YL: Y low.
*        0: no interrupt
*        1: YL event has occurred
*  1 YH: X high.
*        0: no interrupt
*        1: XH event has occurred
*  0 YL: X low.
*        0: no interrupt
*        1: XL event has occurred
*******************************************************************************/
#define LIS302DL_FF_WU_SRC1_REG_ADDR           0x31

/*******************************************************************************
*  FF_WU_THS_1 Register: Threshold register
*  Read Write register
*  Default value: 0x00
*  7 DCRM: Reset mode selection.
*          0 - counter resetted
*          1 - counter decremented
*  6 THS6-THS0: Free-fall/wake-up threshold value.
*******************************************************************************/
#define LIS302DL_FF_WU_THS1_REG_ADDR          0x32

/*******************************************************************************
*  FF_WU_DURATION_1 Register: duration Register
*  Read Write register
*  Default value: 0x00
*  7:0 D7-D0 Duration value. (Duration steps and maximum values depend on the ODR chosen)
 ******************************************************************************/
#define LIS302DL_FF_WU_DURATION1_REG_ADDR     0x33

/*******************************************************************************
*  FF_WW_CFG_2 Register: Configuration register for Interrupt 2 source.
*  Read write register
*  Default value: 0x00
*  7 AOI: AND/OR combination of Interrupt events.
*         0: OR combination of interrupt events
*         1: AND combination of interrupt events
*  6 LIR: Latch/not latch interrupt request
*         0: interrupt request not latched
*         1: interrupt request latched
*  5 ZHIE: Enable interrupt generation on Z high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  4 ZLIE: Enable interrupt generation on Z low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*  3 YHIE: Enable interrupt generation on Y high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  2 YLIE: Enable interrupt generation on Y low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*  1 XHIE: Enable interrupt generation on X high event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value higher than preset threshold
*  0 XLIE: Enable interrupt generation on X low event.
*          0: disable interrupt request
*          1: enable interrupt request on measured accel. value lower than preset threshold
*******************************************************************************/
#define LIS302DL_FF_WU_CFG2_REG_ADDR         0x34

/*******************************************************************************
*  FF_WU_SRC_2 Register: Interrupt 2 source register.
*  Reading at this address clears FF_WU_SRC_2 register and the FF, WU 2 interrupt
*  and allow the refreshment of data in the FF_WU_SRC_2 register if the latched option
*  was chosen.
*  Read only register
*  Default value: 0x00
*  7 Reserved
*  6 IA: Interrupt active.
*        0: no interrupt has been generated
*        1: one or more interrupts have been generated
*  5 ZH: Z high.
*        0: no interrupt
*        1: ZH event has occurred
*  4 ZL: Z low.
*        0: no interrupt
*        1: ZL event has occurred
*  3 YH: Y high.
*        0: no interrupt
*        1: YH event has occurred
*  2 YL: Y low.
*        0: no interrupt
*        1: YL event has occurred
*  1 YH: X high.
*        0: no interrupt
*        1: XH event has occurred
*  0 YL: X low.
*        0: no interrupt
*        1: XL event has occurred
*******************************************************************************/
#define LIS302DL_FF_WU_SRC2_REG_ADDR           0x35

/*******************************************************************************
*  FF_WU_THS_2 Register: Threshold register
*  Read Write register
*  Default value: 0x00
*  7 DCRM: Reset mode selection.
*          0 - counter resetted
*          1 - counter decremented
*  6 THS6-THS0: Free-fall/wake-up threshold value.
*******************************************************************************/
#define LIS302DL_FF_WU_THS2_REG_ADDR          0x36

/*******************************************************************************
*  FF_WU_DURATION_2 Register: duration Register
*  Read Write register
*  Default value: 0x00
*  7:0 D7-D0 Duration value. (Duration steps and maximum values depend on the ODR chosen)
 ******************************************************************************/
#define LIS302DL_FF_WU_DURATION2_REG_ADDR     0x37

/******************************************************************************
*  CLICK_CFG Register: click Register
*  Read Write register
*  Default value: 0x00
*  7 Reserved
*  6 LIR: Latch Interrupt request.
*         0: interrupt request not latched
*         1: interrupt request latched
*  5 Double_Z: Enable interrupt generation on double click event on Z LIS302DL.
*              0: disable interrupt request
*              1: enable interrupt request
*  4 Single_Z: Enable interrupt generation on single click event on Z LIS302DL.
*              0: disable interrupt request
*              1: enable interrupt request
*  3 Double_Y: Enable interrupt generation on double click event on Y LIS302DL.
*              0: disable interrupt request
*              1: enable interrupt request
*  2 Single_Y: Enable interrupt generation on single click event on Y LIS302DL.
*              0: disable interrupt request
*              1: enable interrupt request
*  1 Double_X: Enable interrupt generation on double click event on X LIS302DL.
*              0: disable interrupt request
*              1: enable interrupt request
*  0 Single_y: Enable interrupt generation on single click event on X LIS302DL.
*              0: disable interrupt request
*              1: enable interrupt request
 ******************************************************************************/
#define LIS302DL_CLICK_CFG_REG_ADDR     0x38

/******************************************************************************
*  CLICK_SRC Register: click status Register
*  Read only register
*  Default value: 0x00
*  7 Reserved
*  6 IA: Interrupt active.
*        0: no interrupt has been generated
*        1: one or more interrupts have been generated
*  5 Double_Z: Double click on Z LIS302DL event.
*        0: no interrupt
*        1: Double Z event has occurred
*  4 Single_Z: Z low.
*        0: no interrupt
*        1: Single Z event has occurred
*  3 Double_Y: Y high.
*        0: no interrupt
*        1: Double Y event has occurred
*  2 Single_Y: Y low.
*        0: no interrupt
*        1: Single Y event has occurred
*  1 Double_X: X high.
*        0: no interrupt
*        1: Double X event has occurred
*  0 Single_X: X low.
*        0: no interrupt
*        1: Single X event has occurred
*******************************************************************************/
#define LIS302DL_CLICK_SRC_REG_ADDR        0x39

/*******************************************************************************
*  CLICK_THSY_X Register: Click threshold Y and X register
*  Read Write register
*  Default value: 0x00
*  7:4 THSy3-THSy0: Click threshold on Y LIS302DL, step 0.5g
*  3:0 THSx3-THSx0: Click threshold on X LIS302DL, step 0.5g
*******************************************************************************/
#define LIS302DL_CLICK_THSY_X_REG_ADDR        0x3B

/*******************************************************************************
*  CLICK_THSZ Register: Click threshold Z register
*  Read Write register
*  Default value: 0x00
*  7:4 Reserved
*  3:0 THSz3-THSz0: Click threshold on Z LIS302DL, step 0.5g
*******************************************************************************/
#define LIS302DL_CLICK_THSZ_REG_ADDR         0x3C

/*******************************************************************************
*  CLICK_TimeLimit Register: Time Limit register
*  Read Write register
*  Default value: 0x00
*  7:0 Dur7-Dur0: Time Limit value, step 0.5g
*******************************************************************************/
#define LIS302DL_CLICK_TIMELIMIT_REG_ADDR        0x3D

/*******************************************************************************
*  CLICK_Latency Register: Latency register
*  Read Write register
*  Default value: 0x00
*  7:0 Lat7-Lat0: Latency value, step 1msec
*******************************************************************************/
#define LIS302DL_CLICK_LATENCY_REG_ADDR        0x3E

/*******************************************************************************
*  CLICK_Window Register: Window register
*  Read Write register
*  Default value: 0x00
*  7:0 Win7-Win0: Window value, step 1msec
*******************************************************************************/
#define LIS302DL_CLICK_WINDOW_REG_ADDR        0x3F

/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/

/* Defines ------------------------------------------------------------------*/
#define LIS302DL_CALIBRATION_BUF_LENGHT                ((uint32_t) 2000u)
#define LIS302DL_SENSITIVITY                           ((float) 0.07)
#define LIS302DL_I_AM_REG                              0x3B

#define LIS302DL_SENSITIVITY_2_3G                      18u /* mg/digit */
#define LIS302DL_SENSITIVITY_9_2G                      72u /* mg/digit */

#define LIS302DL_DATARATE_100                          ((uint8_t)0x00)
#define LIS302DL_DATARATE_400                          ((uint8_t)0x80)

#define LIS302DL_LOWPOWERMODE_POWERDOWN                ((uint8_t)0x00)
#define LIS302DL_LOWPOWERMODE_ACTIVE                   ((uint8_t)0x40)

#define LIS302DL_FULLSCALE_2_3                         ((uint8_t)0x00)
#define LIS302DL_FULLSACLE_9_2                         ((uint8_t)0x20)

#define LIS302DL_SELFTEST_NORMAL                       ((uint8_t)0x00)
#define LIS302DL_SELFTEST_P                            ((uint8_t)0x10)
#define LIS302DL_SELFTEST_M                            ((uint8_t)0x08)

#define LIS302DL_X_ENABLE                              ((uint8_t)0x01)
#define LIS302DL_Y_ENABLE                              ((uint8_t)0x02)
#define LIS302DL_Z_ENABLE                              ((uint8_t)0x04)
#define LIS302DL_XYZ_ENABLE                            ((uint8_t)0x07)

#define LIS302DL_SERIALINTERFACE_4WIRE                 ((uint8_t)0x00)
#define LIS302DL_SERIALINTERFACE_3WIRE                 ((uint8_t)0x80)

#define LIS302DL_BOOT_NORMALMODE                       ((uint8_t)0x00)
#define LIS302DL_BOOT_REBOOTMEMORY                     ((uint8_t)0x40)

#define LIS302DL_FILTERDATASELECTION_BYPASSED          ((uint8_t)0x00)
#define LIS302DL_FILTERDATASELECTION_OUTPUTREGISTER    ((uint8_t)0x20)

#define LIS302DL_HIGHPASSFILTERINTERRUPT_OFF           ((uint8_t)0x00)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_1             ((uint8_t)0x04)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_2             ((uint8_t)0x08)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_1_2           ((uint8_t)0x0C)

#define LIS302DL_HIGHPASSFILTER_LEVEL_0                ((uint8_t)0x00)
#define LIS302DL_HIGHPASSFILTER_LEVEL_1                ((uint8_t)0x01)
#define LIS302DL_HIGHPASSFILTER_LEVEL_2                ((uint8_t)0x02)
#define LIS302DL_HIGHPASSFILTER_LEVEL_3                ((uint8_t)0x03)

#define LIS302DL_INTERRUPTREQUEST_NOTLATCHED           ((uint8_t)0x00)
#define LIS302DL_INTERRUPTREQUEST_LATCHED              ((uint8_t)0x40)

#define LIS302DL_CLICKINTERRUPT_XYZ_DISABLE            ((uint8_t)0x00)
#define LIS302DL_CLICKINTERRUPT_X_ENABLE               ((uint8_t)0x01)
#define LIS302DL_CLICKINTERRUPT_Y_ENABLE               ((uint8_t)0x04)
#define LIS302DL_CLICKINTERRUPT_Z_ENABLE               ((uint8_t)0x10)
#define LIS302DL_CLICKINTERRUPT_XYZ_ENABLE             ((uint8_t)0x15)

#define LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE      ((uint8_t)0x00)
#define LIS302DL_DOUBLECLICKINTERRUPT_X_ENABLE         ((uint8_t)0x02)
#define LIS302DL_DOUBLECLICKINTERRUPT_Y_ENABLE         ((uint8_t)0x08)
#define LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE         ((uint8_t)0x20)
#define LIS302DL_DOUBLECLICKINTERRUPT_XYZ_ENABLE       ((uint8_t)0x2A)

#define DUMMY_BYTE             ((uint8_t)0x00)
#define READWRITE_CMD          ((uint8_t)0x80)
#define MULTIPLEBYTE_CMD       ((uint8_t)0x40)

struct lis302dl_init_conf {
    uint8_t power_mode;
    uint8_t output_data_rate;
    uint8_t axes_enable;
    uint8_t full_scale;
    uint8_t self_test;
};

/* Interrupt struct */
struct lis302dl_interrupt_conf {
    uint8_t latch_request;
    uint8_t single_click_axes;
    uint8_t double_click_axes;
};

/* High Pass Filter struct */
struct lis302dl_filter_conf {
    uint8_t highpass_filter_data_select;
    uint8_t highpass_filter_cutoff_freq;
    uint8_t highpass_filter_interrupt;
};

extern struct accelerometer_drv lis302dl_drv;

void lis302dl_init(uint16_t init_struct);
uint8_t lis302dl_read_id(void);
void lis302dl_deinit(void);
uint8_t lis302dl_read_id(void);
void lis302dl_filter_config(uint8_t filter_struct);
void lis302dl_interrupt_config(struct lis302dl_interrupt_conf *lis302dl_interrupt_conf_ptr);
void lis302dl_click_int_clear(void);
void lis302dl_click_int_config(void);
void lis302dl_low_power_cmd(uint8_t low_power_mode);
void lis302dl_data_rate_cmd(uint8_t data_rate_value);
void lis302dl_full_scale_cmd(uint8_t fs_value);
void lis302dl_reboot_cmd(void);
void lis302dl_read_acc(int16_t *pData);

#endif /* LIS302DL_H_ */
