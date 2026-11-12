/*
 * st7735.c
 *
 *  Created on: 12 nov. 2026
 *      Author: albin
 */

#include "main.h"
extern SPI_HandleTypeDef hspi2;

void ST7735_write_data(uint8_t *buff, size_t buff_size) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);   // DC = 1 (Data mode)
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // CS = 0 (Select display)

    HAL_SPI_Transmit(&hspi2, buff, buff_size, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);   // CS = 1 (Deselect display)
}
