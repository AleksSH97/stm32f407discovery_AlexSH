/**
 ******************************************************************************
 * @file           : audio.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : Driver for audio.
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "audio.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
static struct audio_drv *audio_drv_ptr;
struct audio audio;

/******************************************************************************/
void prvAudioErrorHandler(audio_error_t error);



/**
 * \brief           Audio init
 * \param[in]
 */
uint8_t AudioInit(void)
{
    uint8_t res = AUDIO_OK;
    uint32_t deviceid = 0x00;

    audio.output_device = AUDIO_OUTPUT_DEVICE_AUTO;

    res = DacI2cInit();

    PrintfLogsCRLF(CLR_GR"AUDIO INIT..."CLR_DEF);

    deviceid = cs43l22_drv.ReadID(AUDIO_I2C_ADDRESS);

    if ((deviceid & CS43L22_ID_MASK) == CS43L22_ID) {
        audio_drv_ptr = &cs43l22_drv;
    }
    else {
        return AUDIO_INIT_ERROR;
    }

    if (audio_drv_ptr == NULL) {
        return AUDIO_INIT_ERROR;
    }

    if (audio_drv_ptr->Init == NULL || audio_drv_ptr->ReadID == NULL) {
        return AUDIO_INIT_ERROR;
    }

    if (res == AUDIO_OK) {
        res = audio_drv_ptr->Init(AUDIO_I2C_ADDRESS, audio.output_device, AUDIO_DEFAULT_VOLMAX, AUDIO_FREQUENCY_48K);
        AudioSetStatus(AUDIO_IDLE);
    }

    return res;
}
/******************************************************************************/




/**
 * \brief           Audio task
 * \param[in]
 */
void AudioTask(void *argument)
{
    audio.error = AudioInit();

    for (;;) {
        if (AudioGetError() != 0) {
            prvAudioErrorHandler(AudioGetError());
            continue;
        }

        switch (audio.status) {
            case AUDIO_IDLE:
                break;
            case AUDIO_INIT:
                MicrophoneSetStatus(MICROPHONE_INIT);
                AudioSetStatus(AUDIO_PLAY);
                break;
            case AUDIO_PLAY:
                AudioSetError(AudioPlay());
                break;
            case AUDIO_STOP:
                break;
            case AUDIO_ERROR:
                PrintfLogsCRLF(CLR_RD"AUDIO ERROR..."CLR_DEF);
                break;
            default:
                AudioSetError(AUDIO_UNDEFINED_ERROR);
                break;
        }

    }
}
/******************************************************************************/




/**
 * \brief           Audio play sound state
 * \param[in]
 */
uint8_t AudioPlay(void)
{
    uint16_t buffer = 0x00;

    MicrophoneGetDataFromTxBuffer(&buffer);

    if (audio_drv_ptr->Play == NULL) {
        return AUDIO_PLAY_ERROR;
    }

    if(audio_drv_ptr->Play(AUDIO_I2C_ADDRESS, &buffer, 1) != 0) {
        return AUDIO_PLAY_ERROR;
    }
    else {
        HAL_I2S_Transmit_DMA(&hi2s3, &buffer, 1);
    }

    return AUDIO_OK;
}
/******************************************************************************/




/**
 * \brief           Audio set state
 * \param[in]
 */
void AudioSetStatus(audio_status_t status)
{
    audio.status = status;
}
/******************************************************************************/




/**
 * \brief           Audio get state
 * \param[in]
 */
audio_status_t AudioGetStatus(void)
{
    return audio.status;
}
/******************************************************************************/




/**
 * \brief           Audio get error
 * \param[in]
 */
audio_error_t AudioGetError(void)
{
    return audio.error;
}
/******************************************************************************/




/**
 * @brief          Audio set current error state
 */
void AudioSetError(audio_error_t error)
{
    audio.error = error;
}
/******************************************************************************/



/**
 * @brief          Audio I/O init
 */
void AudioIoInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable Reset GPIO Clock */
    AUDIO_RESET_GPIO_CLK_ENABLE();

    /* Audio reset pin configuration */
    GPIO_InitStruct.Pin = AUDIO_RESET_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(AUDIO_RESET_GPIO, &GPIO_InitStruct);

    /* Power Down the codec */
    HAL_GPIO_WritePin(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, GPIO_PIN_RESET);

    /* Wait for a delay to insure registers erasing */
    HAL_Delay(5);

    /* Power on the codec */
    HAL_GPIO_WritePin(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, GPIO_PIN_SET);

    /* Wait for a delay to insure registers erasing */
    HAL_Delay(5);
}
/******************************************************************************/




/**
 * @brief          Audio error handler
 */
void prvAudioErrorHandler(audio_error_t error)
{
    switch (error) {
        case AUDIO_INIT_ERROR:
            PrintfLogsCRLF("Error AUDIO: " CLR_RD"0%d" CLR_DEF, error);
            AudioSetError(AUDIO_OK);
            break;
        case AUDIO_PLAY_ERROR:
            PrintfLogsCRLF("Error AUDIO: " CLR_RD"0%d" CLR_DEF, error);
            AudioSetError(AUDIO_OK);
            break;
        case AUDIO_UNDEFINED_ERROR:
            PrintfLogsCRLF("Error AUDIO: " CLR_RD"0%d" CLR_DEF, error);
            AudioSetError(AUDIO_OK);
            break;
//        case ACCELERO_RX_BUFFER_READ_ERROR:
//            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
//            AccelerometerSetError(ACCELERO_OK);
//            break;
//        case ACCELERO_RX_BUFFER_WRITE_ERROR:
//            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
//            AccelerometerSetError(ACCELERO_OK);
//            break;
//        case ACCELERO_DELETE_TRASH_ERROR:
//            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
//            AccelerometerSetError(ACCELERO_OK);
//            break;
//        case ACCELERO_SPI_SETUP_ERROR:
//            PrintfLogsCRLF("Error SPI: " CLR_RD"0%d" CLR_DEF, error);
//            AccelerometerSetError(ACCELERO_OK);
//            break;
        default:
            PrintfLogsCRLF(CLR_RD"Undefined error AUDIO"CLR_DEF);
            AudioSetError(AUDIO_OK);
            break;
    }
}
/******************************************************************************/
