/**
 ******************************************************************************
 * @file           : accelero_spi.h
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : SPI1 header
 ******************************************************************************
 * ----------------- Copyright (c) 2022 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ACCELERO_SPI_H__
#define __ACCELERO_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lis302dl.h"
#include "lwrb.h"

#define ACCELERO_SPI_CS_PIN            GPIO_PIN_3                   /* PE.03 */
#define ACCELERO_SPI_CS_GPIO_PORT      GPIOE

#define ACCELERO_SPI_SCK_PIN           GPIO_PIN_5
#define ACCELERO_SPI_MISO_PIN          GPIO_PIN_6
#define ACCELERO_SPI_MOSI_PIN          GPIO_PIN_7
#define ACCELERO_SPI_GPIO_PORT         GPIOA

#define ACCELERO_SPI_CS_LOW()          HAL_GPIO_WritePin(ACCELERO_SPI_CS_GPIO_PORT, ACCELERO_SPI_CS_PIN, GPIO_PIN_RESET)
#define ACCELERO_SPI_CS_HIGH()         HAL_GPIO_WritePin(ACCELERO_SPI_CS_GPIO_PORT, ACCELERO_SPI_CS_PIN, GPIO_PIN_SET)

#define ACCELERO_SPI_INT1_PI           GPIO_PIN_0                 /* PE.00 */
#define ACCELERO_SPI_INT1_EXTI_IRQn    EXTI0_IRQn
#define ACCELERO_SPI_INT1_PORT         GPIOE

#define ACCELERO_SPI_INT2_PIN          GPIO_PIN_1                 /* PE.01 */
#define ACCELERO_SPI_INT2_EXTI_IRQn    EXTI1_IRQn
#define ACCELERO_SPI_INT2_PORT         GPIOE

#define ACCELERO_SPI_TIMEOUT           (uint32_t)0x1000
#define ACCELERO_SPI_NUM_OF_AXES       3u

#define ACCELERO_SPI_X                 0u
#define ACCELERO_SPI_Y                 1u
#define ACCELERO_SPI_Z                 2u

#define ACCELERO_SPI_BOUNDARY_POS      200
#define ACCELERO_SPI_BOUNDARY_NEG      -200

#define ACCELERO_SPI_BUFF_SIZE         8u

extern SPI_HandleTypeDef hspi1;
extern struct accelero_spi accelero_spi;

enum accelero_status
{
    ACCELERO_OK = 0,
    ACCELERO_INIT_ERROR,
    ACCELERO_TIMEOUT,
    ACCELERO_XYZ,
    ACCELERO_ERROR,
    ACCELERO_BLOCKED = 5,
};

struct accelero_spi {
    lwrb_t           lwrb_rx;
    lwrb_t           lwrb_tx;
    uint8_t          buff_rx[ACCELERO_SPI_BUFF_SIZE];
    uint8_t          buff_tx[ACCELERO_SPI_BUFF_SIZE];

    uint8_t          receive;
    uint8_t          transmit;

    uint8_t          receive_buff[6];
    uint8_t          transmit_buff[6];

    uint8_t          got_data; //rx buff from ISR

    int16_t          xyz_buf[ACCELERO_SPI_NUM_OF_AXES];

    enum accelero_status status;
};

void AcceleroSpiInit(void);
void AcceleroCsInit(void);
void AcceleroIoRead(uint8_t *buf_ptr, uint8_t read_addr, uint16_t num_byte_to_read);
void AcceleroIoWrite(uint8_t *buf_ptr, uint8_t write_addr, uint16_t num_byte_to_write);
void AcceleroIoItConfig(void);
void AcceleroLedIndication(void);

bool AcceleroSpiSetupTransmitIT(void);
bool AcceleroSpiSetupReceiveIT(void);
bool AcceleroSpiSetupWriteReadIT(void);
bool AcceleroSpiSetupWriteReadIT_BUFF(uint8_t num_byte);

void AcceleroSetMode(bool mode);
bool AcceleroGetMode(void);
bool prvAcceleroSpiWrite(uint8_t byte);
void AcceleroIoWriteIT(uint8_t *buf_ptr, uint8_t write_addr, uint16_t num_byte_to_write);
void AcceleroIoReadIT(uint8_t *buf_ptr, uint8_t read_addr, uint16_t num_byte_to_read);

#ifdef __cplusplus
}
#endif

#endif /* __ACCELERO_SPI_H__ */

