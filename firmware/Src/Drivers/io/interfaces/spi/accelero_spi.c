/**
 ******************************************************************************
 * @file           : Accelero_spi.c
 * @author         : aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : SPI1 usage file
 ******************************************************************************
 * ----------------- Copyright (c) 2022 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "accelero_spi.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
SPI_HandleTypeDef hspi1;

/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
static uint8_t prvAcceleroSpiWriteRead(uint8_t byte);




/**
 * @brief           SPI1 init fn
 */
void AcceleroSpiInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = ACCELERO_SPI_SCK_PIN | ACCELERO_SPI_MISO_PIN | ACCELERO_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(ACCELERO_SPI_GPIO_PORT, &GPIO_InitStruct);

    __HAL_RCC_SPI1_CLK_ENABLE();

    HAL_NVIC_SetPriority(SPI1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    hspi1.Instance                = SPI1;
    hspi1.Init.Mode               = SPI_MODE_MASTER;
    hspi1.Init.Direction          = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize           = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity        = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase           = SPI_PHASE_1EDGE;
    hspi1.Init.NSS                = SPI_NSS_SOFT; // Check this out again. Was configured with error! SPI was not transferring
    hspi1.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode             = SPI_TIMODE_DISABLED;
    hspi1.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLED;
    hspi1.Init.CRCPolynomial      = 7; // Check this out again. Was configured with error! SPI was not transferring

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
      Error_Handler();
    }
}
/******************************************************************************/




/**
 * @brief          CS pin init
 */
void AcceleroCsInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin = ACCELERO_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(ACCELERO_SPI_CS_GPIO_PORT, &GPIO_InitStruct);

    ACCELERO_SPI_CS_HIGH();
}
/******************************************************************************/




/**
  * @brief  Configures the Accelero INT2.
  *         EXTI0 is already used by user button so INT1 is not configured here.
  * @param  None
  * @retval None
  */
void AcceleroIoItConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin = ACCELERO_SPI_INT2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ACCELERO_SPI_INT2_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority((IRQn_Type)ACCELERO_SPI_INT2_EXTI_IRQn, 0x07, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)ACCELERO_SPI_INT2_EXTI_IRQn);
}
/******************************************************************************/




/**
  * @brief  Writes one byte to the Accelero.
  * @param  buf_ptr: pointer to the buffer containing the data to be written to the Accelero.
  * @param  write_addr: Accelero's internal address to write to.
  * @param  num_byte_to_write: Number of bytes to write.
  * @retval None
  */
void AcceleroIoWrite(uint8_t *buf_ptr, uint8_t write_addr, uint16_t num_byte_to_write)
{
    if (num_byte_to_write > 0x01) {
        write_addr |= (uint8_t)MULTIPLEBYTE_CMD;
    }

    ACCELERO_SPI_CS_LOW();

    //PrintfLogsCRLF("\tWriting: ");

    prvAcceleroSpiWriteRead(write_addr);

    while (num_byte_to_write >= 0x01) {
        prvAcceleroSpiWriteRead(*buf_ptr);
        num_byte_to_write--;
        buf_ptr++;
    }

    ACCELERO_SPI_CS_HIGH();
}
/******************************************************************************/




/**
  * @brief  Writes one byte to the Accelero interrupt
  * @param  buf_ptr: pointer to the buffer containing the data to be written to the Accelero.
  * @param  write_addr: Accelero's internal address to write to.
  * @param  num_byte_to_write: Number of bytes to write.
  * @retval None
  */
void AcceleroIoWriteIT(uint8_t *buf_ptr, uint8_t write_addr, uint16_t num_byte_to_write)
{
    PrintfLogsCRLF("");
    PrintfLogsCRLF(CLR_MG"\tStarted writing..."CLR_DEF);
    PrintfLogsCRLF("");

    uint8_t data = 0x0000;
    uint8_t trash = 0x00;

    if (num_byte_to_write > 0x01) {
        write_addr |= (uint8_t)MULTIPLEBYTE_CMD;
    }

    if(!AccelerometerPutDataToTxBuffer(&write_addr, 1)) {
        PrintfLogsCRLF(CLR_RD"1.\tPUT DATA TO TX BUFFER FROM WRITE IT FUNCTION ERROR"CLR_DEF);
    }

    ACCELERO_SPI_CS_LOW();

    if (!AccelerometerReadDataFromTxBuffer(&accelero_spi.transmit, 1)) {
        PrintfLogsCRLF(CLR_RD"2.\tREAD DATA FROM TX BUFFER FROM WRITE IT FUNCTION ERROR"CLR_DEF);
    }

    if (!AcceleroSpiSetupWriteReadIT()) {
        HardFault_Handler();
    }

    if (!AccelerometerReadDataFromRxBuffer(&trash, 1)) {
        PrintfLogsCRLF(CLR_RD"5.\tREAD DATA FROM RX BUFFER FROM READ IT FUNCTION IN CYCLE ERROR"CLR_DEF);
    }

    while (num_byte_to_write >= 0x01) {

        data = *buf_ptr;

        if(!AccelerometerPutDataToTxBuffer(&data, 1)) {
            PrintfLogsCRLF(CLR_RD"3.\tPUT DATA TO TX BUFFER FROM WRITE IT FUNCTION IN CYCLE ERROR"CLR_DEF);
        }

        if (!AccelerometerReadDataFromTxBuffer(&accelero_spi.transmit, 1)) {
            PrintfLogsCRLF(CLR_RD"4.\tREAD DATA FROM TX BUFFER FROM WRITE IT FUNCTION IN CYCLE ERROR"CLR_DEF);
        }

        if (!AcceleroSpiSetupWriteReadIT()) {
            HardFault_Handler();
        }

        if (!AccelerometerReadDataFromRxBuffer(&trash, 1)) {
            PrintfLogsCRLF(CLR_RD"5.\tREAD DATA FROM RX BUFFER FROM READ IT FUNCTION IN CYCLE ERROR"CLR_DEF);
        }

        num_byte_to_write--;
        buf_ptr++;
    }

    ACCELERO_SPI_CS_HIGH();
}
/******************************************************************************/



/**
  * @brief  Reads a block of data from the Accelero.
  * @param  buf_ptr: pointer to the buffer that receives the data read from the Accelero.
  * @param  read_addr: Accelero's internal address to read from.
  * @param  num_byte_to_read: number of bytes to read from the Accelero.
  * @retval None
  */
void AcceleroIoRead(uint8_t *buf_ptr, uint8_t read_addr, uint16_t num_byte_to_read)
{
    if (num_byte_to_read > 0x01) {
        read_addr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
    }
    else {
        read_addr |= (uint8_t)READWRITE_CMD;
    }

    ACCELERO_SPI_CS_LOW();

    prvAcceleroSpiWriteRead(read_addr);

    while (num_byte_to_read > 0x01) {
        *buf_ptr = prvAcceleroSpiWriteRead(DUMMY_BYTE);
        num_byte_to_read--;
        buf_ptr++;
    }

    ACCELERO_SPI_CS_HIGH();
}
/******************************************************************************/




/**
  * @brief  Reads a block of data from the Accelero with interrupt.
  * @param  buf_ptr: pointer to the buffer that receives the data read from the Accelero.
  * @param  read_addr: Accelero's internal address to read from.
  * @param  num_byte_to_read: number of bytes to read from the Accelero.
  * @retval None
  */
void AcceleroIoReadIT(uint8_t *buf_ptr, uint8_t read_addr, uint16_t num_byte_to_read)
{
    uint8_t trash = 0x00;

    PrintfLogsCRLF("");
    PrintfLogsCRLF(CLR_MG"\tStarted reading..."CLR_DEF);
    PrintfLogsCRLF("");

    if (num_byte_to_read > 0x01) {
        read_addr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
    }
    else {
        read_addr |= (uint8_t)READWRITE_CMD;
    }

    if(!AccelerometerPutDataToTxBuffer(&read_addr, 1)) {
        PrintfLogsCRLF(CLR_RD"1.\tPUT DATA TO TX BUFFER FROM READ IT FUNCTION ERROR"CLR_DEF);
    }

    ACCELERO_SPI_CS_LOW();

    if (!AccelerometerReadDataFromTxBuffer(&accelero_spi.transmit, 1)) {
        PrintfLogsCRLF(CLR_RD"2.\tREAD DATA FROM TX BUFFER FROM READ IT FUNCTION ERROR"CLR_DEF);
    }

    if (!AcceleroSpiSetupWriteReadIT()) {
        HardFault_Handler();
    }

    if (!AccelerometerReadDataFromRxBuffer(&trash, 1)) {
        PrintfLogsCRLF(CLR_RD"5.\tREAD DATA FROM RX BUFFER FROM READ IT FUNCTION IN CYCLE ERROR"CLR_DEF);
    }

    while (num_byte_to_read > 0x01) {

        uint8_t data = 0x00;
        uint8_t dummy = 0x00;

        dummy = DUMMY_BYTE;

        if(!AccelerometerPutDataToTxBuffer(&dummy, 1)) {
            PrintfLogsCRLF(CLR_RD"3.\tPUT DUMMY_BYTE TO TX BUFFER FROM READ IT FUNCTION IN CYCLE ERROR"CLR_DEF);
        }

        if (!AccelerometerReadDataFromTxBuffer(&accelero_spi.transmit, 1)) {
            PrintfLogsCRLF(CLR_RD"4.\tREAD DATA FROM TX BUFFER FROM READ IT FUNCTION IN CYCLE ERROR"CLR_DEF);
        }

        if (!AcceleroSpiSetupWriteReadIT()) {
            HardFault_Handler();
        }

        if (!AccelerometerReadDataFromRxBuffer(&data, 1)) {
            PrintfLogsCRLF(CLR_RD"5.\tREAD DATA FROM RX BUFFER FROM READ IT FUNCTION IN CYCLE ERROR"CLR_DEF);
        }

        PrintfLogsCRLF("info from RX SPI buff: %d", data);

        *buf_ptr = data;
        num_byte_to_read--;
        buf_ptr++;
    }

    ACCELERO_SPI_CS_HIGH();
}
/******************************************************************************/




/**
 * @brief          SPI1 Transmit and Receive
 */
uint8_t prvAcceleroSpiWriteRead(uint8_t byte)
{
    uint8_t received_byte = 0;

    if (HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&byte, (uint8_t*)&received_byte, 1, ACCELERO_SPI_TIMEOUT) != HAL_OK) {
        PrintfLogsCRLF(CLR_RD"Error transition/receiving SPI"CLR_DEF);
    }

    return received_byte;
}
/******************************************************************************/




/**
 * @brief          SPI1 Transmit and Receive
 */
bool AcceleroSpiSetupWriteReadIT(void)
{
    if (HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t*)&accelero_spi.transmit, (uint8_t*)&accelero_spi.receive, 1) != HAL_OK) {
        PrintfLogsCRLF(CLR_RD"Error setup transition/receiving SPI IT"CLR_DEF);
        return false;
    }

    return true;
}
/******************************************************************************/




/**
 * @brief          SPI1 Transmit and Receive with buff
 */
bool AcceleroSpiSetupWriteReadIT_BUFF(uint8_t num_byte)
{
    if (HAL_SPI_TransmitReceive_IT(&hspi1, accelero_spi.transmit_buff, accelero_spi.receive_buff, num_byte) != HAL_OK) {
        PrintfLogsCRLF(CLR_RD"Error setup transition/receiving SPI IT"CLR_DEF);
        return false;
    }

    return true;
}
/******************************************************************************/




/**
 * @brief          SPI1 Transmit
 */
bool prvAcceleroSpiWrite(uint8_t byte)
{
    if (HAL_SPI_Transmit(&hspi1, (uint8_t*)&byte, 1, ACCELERO_SPI_TIMEOUT) != HAL_OK) {
        PrintfLogsCRLF(CLR_RD"Error transition SPI"CLR_DEF);
        return false;
    }

    return true;
}
/******************************************************************************/




///**
// * @brief          SPI1 transmit fn
// */
//bool AcceleroSpiSetupTransmitIT(void)
//{
//    if (HAL_SPI_Transmit_IT(&hspi1, &accelero_spi.transmit, sizeof(uint8_t)) != HAL_OK) {
//        return false;
//    }
//
//    return true;
//}
///******************************************************************************/
//
//
//
//
///**
// * @brief          SPI1 receive fn
// */
//bool AcceleroSpiSetupReceiveIT(void)
//{
//    if (HAL_SPI_Receive_IT(&hspi1, &accelero_spi.receive, sizeof(uint8_t)) != HAL_OK) {
//        return false;
//    }
//
//    return true;
//}
///******************************************************************************/




/**
 * @brief          TX RX interrupt callback
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1) {
        //IndicationLedBottom();

        //PrintfLogsCRLF("Received: %d", accelero_spi.receive);

//        if (!AccelerometerPutDataToRxBuffer(accelero_spi.receive_buff, 6)) {
//            PrintfLogsCRLF(CLR_RD"ERROR WRITING TO RX RING BUFFER IN CALLBACK"CLR_DEF);
//        }

        if (!AccelerometerPutDataToRxBuffer(&accelero_spi.receive, 1)) {
            //IndicationLedTop();
            PrintfLogsCRLF(CLR_RD"ERROR WRITING TO RX RING BUFFER IN CALLBACK"CLR_DEF);
        }
    }
}
/******************************************************************************/



///**
// * @brief          TX interrupt callback
// */
//void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
//{
//    if (hspi->Instance == SPI1) {
//        IndicationLedTop();
//
//        PrintfLogsCRLF("Transmit: %d", accelero_spi.transmit);
//
//        if (!AcceleroSpiSetupTransmitIT()) {
//            PrintfLogsCRLF("\tError HAL transmit IT in callback init");
//        }
//    }
//}
///******************************************************************************/
//
//
//
//
///**
// * @brief          RX interrupt callback
// */
//void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
//{
//    if (hspi->Instance == SPI1) {
//        IndicationLedBottom();
//
//        PrintfLogsCRLF("Receive: %d", accelero_spi.receive);
//
//        if(!AccelerometerPutDataToRxBuffer(&accelero_spi.receive, 1)) {
//            PrintfLogsCRLF("Put Data to RX SPI BUFF ERROR!!!");
//        }
//
//        if (!AcceleroSpiSetupReceiveIT()) {
//            PrintfLogsCRLF("\tError HAL receive IT in callback init");
//        }
//    }
//}
///******************************************************************************/




/**
 * @brief          Led ON/OFF depend on the position
 */
void AcceleroLedIndication(void)
{
    AccelerometerGetXyz(accelero_spi.xyz_buf);

    if (accelero_spi.xyz_buf[ACCELERO_SPI_X] < ACCELERO_SPI_BOUNDARY_NEG) {
        HAL_GPIO_WritePin(LED_LEFT_GPIO_Port, LED_LEFT_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_RESET);
    }
    else if (accelero_spi.xyz_buf[ACCELERO_SPI_X] > ACCELERO_SPI_BOUNDARY_POS) {
        HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_LEFT_GPIO_Port, LED_LEFT_Pin, GPIO_PIN_RESET);
    }
    else if (accelero_spi.xyz_buf[ACCELERO_SPI_Y] < ACCELERO_SPI_BOUNDARY_NEG) {
        HAL_GPIO_WritePin(LED_BOTTOM_GPIO_Port, LED_BOTTOM_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_TOP_GPIO_Port, LED_TOP_Pin, GPIO_PIN_RESET);
    }
    else if (accelero_spi.xyz_buf[ACCELERO_SPI_Y] > ACCELERO_SPI_BOUNDARY_POS) {
        HAL_GPIO_WritePin(LED_BOTTOM_GPIO_Port, LED_BOTTOM_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_TOP_GPIO_Port, LED_TOP_Pin, GPIO_PIN_SET);
    }
}
/******************************************************************************/




/**
 * @brief          Error callback fn
 */
void AcceleroSpiError(void)
{
    // TODO code with error statement
}
/******************************************************************************/
