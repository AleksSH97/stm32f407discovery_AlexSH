/**
 ******************************************************************************
 * @file           : cs43l22.h
 * @author         : Aleksandr Shabalin    <alexnv97@gmail.com>
 * @brief          : Header file for DAC
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin ------------------ *
 ******************************************************************************
 * This module is a confidential and proprietary property of Aleksandr Shabalin
 * and possession or use of this module requires written permission
 * of Aleksandr Shabalin.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CS43L22_H_
#define CS43L22_H_

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "audio.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/***************************  START REGISTER MAPPING **************************/
/******************************************************************************/
#define   CS43L22_REG_ID                  0x01
#define   CS43L22_REG_POWER_CTL1          0x02
#define   CS43L22_REG_POWER_CTL2          0x04
#define   CS43L22_REG_CLOCKING_CTL        0x05
#define   CS43L22_REG_INTERFACE_CTL1      0x06
#define   CS43L22_REG_INTERFACE_CTL2      0x07
#define   CS43L22_REG_PASSTHR_A_SELECT    0x08
#define   CS43L22_REG_PASSTHR_B_SELECT    0x09
#define   CS43L22_REG_ANALOG_ZC_SR_SETT   0x0A
#define   CS43L22_REG_PASSTHR_GANG_CTL    0x0C
#define   CS43L22_REG_PLAYBACK_CTL1       0x0D
#define   CS43L22_REG_MISC_CTL            0x0E
#define   CS43L22_REG_PLAYBACK_CTL2       0x0F
#define   CS43L22_REG_PASSTHR_A_VOL       0x14
#define   CS43L22_REG_PASSTHR_B_VOL       0x15
#define   CS43L22_REG_PCMA_VOL            0x1A
#define   CS43L22_REG_PCMB_VOL            0x1B
#define   CS43L22_REG_BEEP_FREQ_ON_TIME   0x1C
#define   CS43L22_REG_BEEP_VOL_OFF_TIME   0x1D
#define   CS43L22_REG_BEEP_TONE_CFG       0x1E
#define   CS43L22_REG_TONE_CTL            0x1F
#define   CS43L22_REG_MASTER_A_VOL        0x20
#define   CS43L22_REG_MASTER_B_VOL        0x21
#define   CS43L22_REG_HEADPHONE_A_VOL     0x22
#define   CS43L22_REG_HEADPHONE_B_VOL     0x23
#define   CS43L22_REG_SPEAKER_A_VOL       0x24
#define   CS43L22_REG_SPEAKER_B_VOL       0x25
#define   CS43L22_REG_CH_MIXER_SWAP       0x26
#define   CS43L22_REG_LIMIT_CTL1          0x27
#define   CS43L22_REG_LIMIT_CTL2          0x28
#define   CS43L22_REG_LIMIT_ATTACK_RATE   0x29
#define   CS43L22_REG_OVF_CLK_STATUS      0x2E
#define   CS43L22_REG_BATT_COMPENSATION   0x2F
#define   CS43L22_REG_VP_BATTERY_LEVEL    0x30
#define   CS43L22_REG_SPEAKER_STATUS      0x31
#define   CS43L22_REG_TEMPMONITOR_CTL     0x32
#define   CS43L22_REG_THERMAL_FOLDBACK    0x33
#define   CS43L22_REG_CHARGE_PUMP_FREQ    0x34

/**
  * @brief  CS43L22 ID
  */
#define  CS43L22_ID            0xE0
#define  CS43L22_ID_MASK       0xF8
/**
  * @brief Chip ID Register: Chip I.D. and Revision Register
  *  Read only register
  *  Default value: 0x01
  *  [7:3] CHIPID[4:0]: I.D. code for the CS43L22.
  *        Default value: 11100b
  *  [2:0] REVID[2:0]: CS43L22 revision level.
  *        Default value:
  *        000 - Rev A0
  *        001 - Rev A1
  *        010 - Rev B0
  *        011 - Rev B1
  */
#define CS43L22_CHIPID_ADDR    0x01

/**
  * @}
  */

/******************************************************************************/
/****************************** END REGISTER MAPPING **************************/
/******************************************************************************/


/******************************************************************************/
/* Public defines ----------------------------------------------------------- */
/******************************************************************************/
#define VOLUME_CONVERT(Volume)    (((Volume) > 100)? 100:((uint8_t)(((Volume) * 255) / 100)))


/******************************************************************************/
/* Public variables --------------------------------------------------------- */
/******************************************************************************/



/******************************************************************************/
/* Public functions --------------------------------------------------------- */
/******************************************************************************/
uint32_t Cs43l22Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
void Cs43l22DeInit(void);
uint32_t Cs43l22ReadID(uint16_t DeviceAddr);
uint32_t Cs43l22Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size);
uint32_t Cs43l22Pause(uint16_t DeviceAddr);
uint32_t Cs43l22Resume(uint16_t DeviceAddr);
uint32_t Cs43l22Stop(uint16_t DeviceAddr, uint32_t CodecPdwnMode);
uint32_t Cs43l22SetVolume(uint16_t DeviceAddr, uint8_t Volume);
uint32_t Cs43l22SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq);
uint32_t Cs43l22SetMute(uint16_t DeviceAddr, uint32_t Cmd);
uint32_t Cs43l22SetOutputMode(uint16_t DeviceAddr, uint8_t Output);
uint32_t Cs43l22Reset(uint16_t DeviceAddr);


/******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#endif /* CS43L22_H_ */
