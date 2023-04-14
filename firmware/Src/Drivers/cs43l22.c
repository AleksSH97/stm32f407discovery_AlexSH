/**
 ******************************************************************************
 * @file           : cs43l22.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : CS43L22 driver
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "cs43l22.h"


/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
struct audio_drv cs43l22_drv =
{
    Cs43l22Init,
    Cs43l22DeInit,
    Cs43l22ReadID,

    Cs43l22Play,
    Cs43l22Pause,
    Cs43l22Resume,
    Cs43l22Stop,

    Cs43l22SetFrequency,
    Cs43l22SetVolume,
    Cs43l22SetMute,
    Cs43l22SetOutputMode,
    Cs43l22Reset,
};

volatile uint8_t OutputDev = 0;
static uint8_t Is_cs43l22_Stop = 1;


/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
uint8_t prvCodecIOWrite(uint8_t addr, uint8_t reg, uint8_t value);


/******************************************************************************/


/**
  * @brief  Set CS43L22 Initialization.
  * @param
  * @retval None
  */
uint32_t Cs43l22Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
    uint32_t counter = 0;

    /* Keep Codec powered OFF */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x01);

    /*Save Output device for mute ON/OFF procedure*/
    switch (OutputDevice) {
        case AUDIO_OUTPUT_DEVICE_SPEAKER:
          OutputDev = 0xFA;
          break;

        case AUDIO_OUTPUT_DEVICE_HEADPHONE:
          OutputDev = 0xAF;
          break;

        case AUDIO_OUTPUT_DEVICE_BOTH:
          OutputDev = 0xAA;
          break;

        case AUDIO_OUTPUT_DEVICE_AUTO:
          OutputDev = 0x05;
          break;

        default:
          OutputDev = 0x05;
          break;
    }

    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, OutputDev);

    /* Clock configuration: Auto detection */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_CLOCKING_CTL, 0x81);

    /* Set the Slave Mode and the audio Standard */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_INTERFACE_CTL1, AUDIO_CODEC_STANDARD);

    /* Set the Master volume */
    counter += Cs43l22SetVolume(DeviceAddr, Volume);

    /* If the Speaker is enabled, set the Mono mode and volume attenuation level */
    if(OutputDevice != AUDIO_OUTPUT_DEVICE_HEADPHONE) {
      /* Set the Speaker Mono mode */
      counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_PLAYBACK_CTL2, 0x06);

      /* Set the Speaker attenuation level */
      counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_SPEAKER_A_VOL, 0x00);
      counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_SPEAKER_B_VOL, 0x00);
    }

    /* Additional configuration for the CODEC. These configurations are done to reduce
    the time needed for the Codec to power off. If these configurations are removed,
    then a long delay should be added between powering off the Codec and switching
    off the I2S peripheral MCLK clock (which is the operating clock for Codec).
    If this delay is not inserted, then the codec will not shut down properly and
    it results in high noise after shut down. */

    /* Disable the analog soft ramp */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_ANALOG_ZC_SR_SETT, 0x00);
    /* Disable the digital soft ramp */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_MISC_CTL, 0x04);
    /* Disable the limiter attack level */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_LIMIT_CTL1, 0x00);
    /* Adjust Bass and Treble levels */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_TONE_CTL, 0x0F);
    /* Adjust PCM volume level */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_PCMA_VOL, 0x0A);
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_PCMB_VOL, 0x0A);

    /* Return communication control value */
    return counter;
}
/******************************************************************************/




/**
  * @brief  Deinitializes the audio codec.
  * @param  None
  * @retval  None
  */
void Cs43l22DeInit(void)
{
    /* Deinitialize Audio Codec interface */
    //AUDIO_IO_DeInit();
}
/******************************************************************************/




/**
  * @brief  Get the CS43L22 ID.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval The CS43L22 ID
  */
uint32_t Cs43l22ReadID(uint16_t DeviceAddr)
{
    uint8_t Value;
    /* Initialize the Control interface of the Audio Codec */
    AudioIoInit();

    Value = DacI2CReadData(DeviceAddr, CS43L22_CHIPID_ADDR);
    Value = (Value & CS43L22_ID_MASK);

    return((uint32_t) Value);
}
/******************************************************************************/




/**
  * @brief Start the audio Codec play feature.
  * @note For this codec no Play options are required.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size)
{
    uint32_t counter = 0;

    if (Is_cs43l22_Stop == 1)
    {
    /* Enable the digital soft ramp */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_MISC_CTL, 0x06);

    /* Enable Output device */
    counter += Cs43l22SetMute(DeviceAddr, AUDIO_MUTE_OFF);

    /* Power on the Codec */
    counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x9E);
    Is_cs43l22_Stop = 0;
    }

    /* Return communication control value */
    return counter;
}
/******************************************************************************/




/**
  * @brief Pauses playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22Pause(uint16_t DeviceAddr)
{
  uint32_t counter = 0;

  /* Pause the audio file playing */
  /* Mute the output first */
  counter += Cs43l22SetMute(DeviceAddr, AUDIO_MUTE_ON);

  /* Put the Codec in Power save mode */
  counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x01);

  return counter;
}
/******************************************************************************/




/**
  * @brief Resumes playing on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22Resume(uint16_t DeviceAddr)
{
  uint32_t counter = 0;
  volatile uint32_t index = 0x00;
  /* Resumes the audio file playing */
  /* Unmute the output first */
  counter += Cs43l22SetMute(DeviceAddr, AUDIO_MUTE_OFF);

  for(index = 0x00; index < 0xFF; index++);

  counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, OutputDev);

  /* Exit the Power save mode */
  counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x9E);

  return counter;
}
/******************************************************************************/




/**
  * @brief Stops audio Codec playing. It powers down the codec.
  * @param DeviceAddr: Device address on communication Bus.
  * @param CodecPdwnMode: selects the  power down mode.
  *          - CODEC_PDWN_HW: Physically power down the codec. When resuming from this
  *                           mode, the codec is set to default configuration
  *                           (user should re-Initialize the codec in order to
  *                            play again the audio stream).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22Stop(uint16_t DeviceAddr, uint32_t CodecPdwnMode)
{
  uint32_t counter = 0;

  /* Mute the output first */
  counter += Cs43l22SetMute(DeviceAddr, AUDIO_MUTE_ON);

  /* Disable the digital soft ramp */
  counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_MISC_CTL, 0x04);

  /* Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
  counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x9F);

  Is_cs43l22_Stop = 1;

  return counter;
}
/******************************************************************************/




/**
  * @brief Sets higher or lower the codec volume level.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Volume: a byte value from 0 to 255 (refer to codec registers
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22SetVolume(uint16_t DeviceAddr, uint8_t Volume)
{
    uint32_t counter = 0;
    uint8_t convertedvol = VOLUME_CONVERT(Volume);

    if(Volume > 0xE6) {
    /* Set the Master volume */
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_MASTER_A_VOL, convertedvol - 0xE7);
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_MASTER_B_VOL, convertedvol - 0xE7);
    }
    else {
    /* Set the Master volume */
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_MASTER_A_VOL, convertedvol + 0x19);
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_MASTER_B_VOL, convertedvol + 0x19);
    }

    return counter;
}
/******************************************************************************/




/**
  * @brief Sets new frequency.
  * @param DeviceAddr: Device address on communication Bus.
  * @param AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq)
{
    return 0;
}
/******************************************************************************/




/**
  * @brief Enables or disables the mute feature on the audio codec.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22SetMute(uint16_t DeviceAddr, uint32_t Cmd)
{
    uint32_t counter = 0;

    /* Set the Mute mode */
    if(Cmd == AUDIO_MUTE_ON) {
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xFF);
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_HEADPHONE_A_VOL, 0x01);
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_HEADPHONE_B_VOL, 0x01);
    }
    else {
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_HEADPHONE_A_VOL, 0x00);
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_HEADPHONE_B_VOL, 0x00);
        counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, OutputDev);
    }
    return counter;
}
/******************************************************************************/




/**
  * @brief Switch dynamically (while audio file is played) the output target
  *         (speaker or headphone).
  * @note This function modifies a global variable of the audio codec driver: OutputDev.
  * @param DeviceAddr: Device address on communication Bus.
  * @param Output: specifies the audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22SetOutputMode(uint16_t DeviceAddr, uint8_t Output)
{
    uint32_t counter = 0;

    switch (Output)
    {
    case AUDIO_OUTPUT_DEVICE_SPEAKER:
      counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xFA); /* SPK always ON & HP always OFF */
      OutputDev = 0xFA;
      break;

    case AUDIO_OUTPUT_DEVICE_HEADPHONE:
      counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xAF); /* SPK always OFF & HP always ON */
      OutputDev = 0xAF;
      break;

    case AUDIO_OUTPUT_DEVICE_BOTH:
      counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xAA); /* SPK always ON & HP always ON */
      OutputDev = 0xAA;
      break;

    case AUDIO_OUTPUT_DEVICE_AUTO:
      counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;

    default:
      counter += prvCodecIOWrite(DeviceAddr, CS43L22_REG_POWER_CTL2, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;
    }
    return counter;
}
/******************************************************************************/




/**
  * @brief Resets cs43l22 registers.
  * @param DeviceAddr: Device address on communication Bus.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Cs43l22Reset(uint16_t DeviceAddr)
{
    return 0;
}
/******************************************************************************/




/**
  * @brief  Writes/Read a single data.
  * @param  addr: I2C address
  * @param  reg: Register address
  * @param  value: Data to be written
  * @retval None
  */
uint8_t prvCodecIOWrite(uint8_t addr, uint8_t reg, uint8_t value)
{
    uint8_t res = 0;

    DacI2CWriteData(addr, reg, value);

    #ifdef VERIFY_WRITTENDATA
    /* Verify that the data has been correctly written */
    result = (DacI2CReadData(Addr, Reg) == Value)? 0:1;
    #endif /* VERIFY_WRITTENDATA */

    return res;
}
/******************************************************************************/
