/**
 ******************************************************************************
 * @file           : Accelero_spi.c
 * @author         : aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : SPI1 usage file
 ******************************************************************************
 * ----------------- Copyright (c) 2023 Aleksandr Shabalin------------------- *
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
bool prvAcceleroCheckX(int16_t coordinate);
bool prvAcceleroCheckY(int16_t coordinate);
void prvAcceleroDrawPoint(int16_t x, int16_t y);




/**
 * @brief           SPI1 init fn
 */
uint8_t AcceleroSpiInit(void)
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
    hspi1.Init.NSS                = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode             = SPI_TIMODE_DISABLED;
    hspi1.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLED;
    hspi1.Init.CRCPolynomial      = 7;

    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        return ACCELERO_INIT_ERROR;
    }

    return ACCELERO_OK;
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
  * @brief  Writes one byte to the Accelero interrupt
  * @param  buf_ptr: pointer to the buffer containing the data to be written to the Accelero.
  * @param  write_addr: Accelero's internal address to write to.
  * @param  num_byte_to_write: Number of bytes to write.
  * @retval None
  */
uint8_t AcceleroIoWriteIT(uint8_t *buf_ptr, uint8_t write_addr, uint16_t num_byte_to_write)
{
    uint8_t data = 0x0000;
    uint8_t trash = 0x00;

    if (num_byte_to_write > 0x01) {
        write_addr |= (uint8_t)MULTIPLEBYTE_CMD;
    }

    if(!AccelerometerPutDataToTxBuffer(&write_addr, 1)) {
        return ACCELERO_TX_BUFFER_WRITE_ERROR;
    }

    ACCELERO_SPI_CS_LOW();

    if (!AccelerometerReadDataFromTxBuffer(&accelero_spi.transmit, 1)) {
        return ACCELERO_TX_BUFFER_READ_ERROR;
    }

    if (!AcceleroSpiSetupWriteReadIT()) {
        return ACCELERO_SPI_SETUP_ERROR;
    }

    if (!AccelerometerReadDataFromRxBuffer(&trash, 1)) {
        return ACCELERO_RX_BUFFER_READ_ERROR;
    }

    while (num_byte_to_write >= 0x01) {

        data = *buf_ptr;

        if(!AccelerometerPutDataToTxBuffer(&data, 1)) {
            return ACCELERO_TX_BUFFER_WRITE_ERROR;
        }

        if (!AccelerometerReadDataFromTxBuffer(&accelero_spi.transmit, 1)) {
            return ACCELERO_TX_BUFFER_READ_ERROR;
        }

        if (!AcceleroSpiSetupWriteReadIT()) {
            return ACCELERO_SPI_SETUP_ERROR;
        }

        if (!AccelerometerReadDataFromRxBuffer(&trash, 1)) {
            return ACCELERO_RX_BUFFER_READ_ERROR;
        }

        num_byte_to_write--;
        buf_ptr++;
    }

    ACCELERO_SPI_CS_HIGH();

    return ACCELERO_OK;
}
/******************************************************************************/




/**
  * @brief  Reads a block of data from the Accelero with interrupt.
  * @param  buf_ptr: pointer to the buffer that receives the data read from the Accelero.
  * @param  read_addr: Accelero's internal address to read from.
  * @param  num_byte_to_read: number of bytes to read from the Accelero.
  * @retval None
  */
uint8_t AcceleroIoReadIT(uint8_t *buf_ptr, uint8_t read_addr, uint16_t num_byte_to_read)
{
    uint8_t trash = 0x00;

    if (num_byte_to_read > 0x01) {
        read_addr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
    }
    else {
        read_addr |= (uint8_t)READWRITE_CMD;
    }

    if(!AccelerometerPutDataToTxBuffer(&read_addr, 1)) {
        return ACCELERO_TX_BUFFER_WRITE_ERROR;
    }

    ACCELERO_SPI_CS_LOW();

    if (!AccelerometerReadDataFromTxBuffer(&accelero_spi.transmit, 1)) {
        return ACCELERO_TX_BUFFER_READ_ERROR;
    }

    if (!AcceleroSpiSetupWriteReadIT()) {
        return ACCELERO_SPI_SETUP_ERROR;
    }

    if (!AccelerometerReadDataFromRxBuffer(&trash, 1)) {
        return ACCELERO_DELETE_TRASH_ERROR;
    }

    while (num_byte_to_read > 0x01) {

        uint8_t data = 0x00;
        uint8_t dummy = 0x00;

        dummy = DUMMY_BYTE;

        if(!AccelerometerPutDataToTxBuffer(&dummy, 1)) {
            return ACCELERO_TX_BUFFER_WRITE_ERROR;
        }

        if (!AccelerometerReadDataFromTxBuffer(&accelero_spi.transmit, 1)) {
            return ACCELERO_TX_BUFFER_READ_ERROR;
        }

        if (!AcceleroSpiSetupWriteReadIT()) {
            return ACCELERO_RX_BUFFER_WRITE_ERROR;
        }

        if (!AccelerometerReadDataFromRxBuffer(&data, 1)) {
            return ACCELERO_RX_BUFFER_READ_ERROR;
        }

        *buf_ptr = data;
        num_byte_to_read--;
        buf_ptr++;
    }

    ACCELERO_SPI_CS_HIGH();

    return ACCELERO_OK;
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
 * @brief          SPI1 Transmit and Receive IT
 */
bool AcceleroSpiSetupWriteReadIT(void)
{
    if (HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t*)&accelero_spi.transmit, (uint8_t*)&accelero_spi.receive, 1) != HAL_OK) {
        return false;
    }

    return true;
}
/******************************************************************************/




/**
 * @brief          TX RX interrupt callback
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1) {
        if (!AccelerometerPutDataToRxBuffer(&accelero_spi.receive, 1)) {
            PrintfLogsCRLF(CLR_RD"ERROR WRITING TO RX RING BUFFER IN CALLBACK"CLR_DEF);
        }
    }
}
/******************************************************************************/




/**
 * @brief          Led ON/OFF depend on the position
 */
void AcceleroLedIndication(void)
{
    int16_t x_axis = 0x00;
    int16_t y_axis = 0x00;

    AccelerometerGetXyz(accelero_spi.xyz_buf);

    x_axis = accelero_spi.xyz_buf[ACCELERO_SPI_X];
    y_axis = accelero_spi.xyz_buf[ACCELERO_SPI_Y];

    if (x_axis < 300 && x_axis > 0 && y_axis < 300 && y_axis > 0) {
        prvAcceleroDrawPoint(0, 0);
    }
    else {
        prvAcceleroDrawPoint(x_axis, y_axis);
    }

    if (!prvAcceleroCheckX(accelero_spi.xyz_buf[ACCELERO_SPI_X])) {
        return;
    }

    if (!prvAcceleroCheckY(accelero_spi.xyz_buf[ACCELERO_SPI_Y])) {
        return;
    }

    if (accelero_spi.xyz_buf[ACCELERO_SPI_X] < ACCELERO_SPI_BOUNDARY_NEG) {
        IndicationLedLeft();
    }
    else if (accelero_spi.xyz_buf[ACCELERO_SPI_X] > ACCELERO_SPI_BOUNDARY_POS) {
        IndicationLedRight();
    }

    if (accelero_spi.xyz_buf[ACCELERO_SPI_Y] < ACCELERO_SPI_BOUNDARY_NEG) {
        IndicationLedBottom();
    }
    else if (accelero_spi.xyz_buf[ACCELERO_SPI_Y] > ACCELERO_SPI_BOUNDARY_POS) {
        IndicationLedTop();
    }
}
/******************************************************************************/




/**
 * @brief          Draw point in the console
 */
void prvAcceleroDrawPoint(int16_t x, int16_t y)
{
    const int16_t console_width = 80;  // Ширина консоли
    const int16_t console_height = 24; // Высота консоли
    const int16_t console_range = 2000; // Диапазон значений координат (от -1000 до 1000)

    // Вычисляем положение точки на экране
    int16_t screenX = (int16_t)(((int32_t)x * console_width) / (2 * console_range) + console_width / 2);
    int16_t screenY = (int16_t)(((int32_t)y * console_height) / (2 * console_range) + console_height / 2);

    // Установка курсора в нужное место на экране
    PrintfLogsCont("\033[2J\033[%d;%dH", screenY, screenX);

    // Выводим символ точки
    PrintfLogsCont("*");
}
/******************************************************************************/




/**
 * @brief          Check for zero level of X axis
 */
bool prvAcceleroCheckX(int16_t coordinate)
{
    if (coordinate == ACCELERO_SPI_ZERO_LEVEL_X_ONE || coordinate == ACCELERO_SPI_ZERO_LEVEL_X_TWO) {
        HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_LEFT_GPIO_Port, LED_LEFT_Pin, GPIO_PIN_RESET);
        return false;
    }

    return true;
}
/******************************************************************************/




/**
 * @brief          Check for zero level of Y axis
 */
bool prvAcceleroCheckY(int16_t coordinate)
{
    if (coordinate == ACCELERO_SPI_ZERO_LEVEL_Y_ONE || coordinate == ACCELERO_SPI_ZERO_LEVEL_Y_TWO) {
        HAL_GPIO_WritePin(LED_TOP_GPIO_Port, LED_TOP_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_BOTTOM_GPIO_Port, LED_BOTTOM_Pin, GPIO_PIN_RESET);
        return false;
    }

    return true;
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
