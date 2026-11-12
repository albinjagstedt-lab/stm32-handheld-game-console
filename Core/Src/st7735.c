/*
 * st7735.c
 *
 *  Created on: 12 nov. 2026
 *      Author: albin
 */

#include "st7735.h"

extern SPI_HandleTypeDef hspi1;

#define X_OFFSET 2
#define Y_OFFSET 1

static void ST7735_reset(void) {
    HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET);
    HAL_Delay(120);
}

static void ST7735_write_command(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET); // Command mode
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // Select display
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);   // Deselect display
}

static void ST7735_write_data(uint8_t *buff, size_t buff_size) {
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);   // Data mode
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); // Select display
    HAL_SPI_Transmit(&hspi1, buff, buff_size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);   // Deselect display
}

static void ST7735_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    x0 += X_OFFSET;
    x1 += X_OFFSET;
    y0 += Y_OFFSET;
    y1 += Y_OFFSET;

    // Column Address Set (CASET)
    ST7735_write_command(0x2A);
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (x1 >> 8) & 0xFF;
    data[3] = x1 & 0xFF;
    ST7735_write_data(data, 4);

    // Row Address Set (RASET)
    ST7735_write_command(0x2B);
    data[0] = (y0 >> 8) & 0xFF;
    data[1] = y0 & 0xFF;
    data[2] = (y1 >> 8) & 0xFF;
    data[3] = y1 & 0xFF;
    ST7735_write_data(data, 4);

    // Memory Write (RAMWR)
    ST7735_write_command(0x2C);
}

void ST7735_init(void) {
    ST7735_reset();

    ST7735_write_command(0x01); // SWRESET: Software reset
    HAL_Delay(150);

    ST7735_write_command(0x11); // SLPOUT: Exit sleep mode
    HAL_Delay(120);

    // COLMOD: Set color mode to 16-bit RGB565 (0x05)
    ST7735_write_command(0x3A);
    uint8_t colmod = 0x05;
    ST7735_write_data(&colmod, 1);

    // MADCTL: Memory data access control (0x00 = Portrait RGB, 0x08 = Portrait BGR)
    ST7735_write_command(0x36);
    uint8_t madctl = 0x08;
    ST7735_write_data(&madctl, 1);

    // INVOFF: Normal display inversion (change to 0x21 if colors look inverted)
    ST7735_write_command(0x20);

    // NORON: Normal display mode on
    ST7735_write_command(0x13);
    HAL_Delay(10);

    // DISPON: Turn display output on
    ST7735_write_command(0x29);
    HAL_Delay(100);
}

void ST7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT) return;

    ST7735_set_address_window(x, y, x, y);

    uint8_t data[2];
    data[0] = (color >> 8) & 0xFF;
    data[1] = color & 0xFF;
    ST7735_write_data(data, 2);
}

void ST7735_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT) return;
    if ((x + w - 1) >= ST7735_WIDTH)  w = ST7735_WIDTH - x;
    if ((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;

    ST7735_set_address_window(x, y, x + w - 1, y + h - 1);

    uint8_t hi = (color >> 8) & 0xFF;
    uint8_t lo = color & 0xFF;

    // Small 128-byte chunk buffer (64 pixels) to keep RAM usage tiny while speeding up SPI
    uint8_t chunk[128];
    for (int i = 0; i < 128; i += 2) {
        chunk[i] = hi;
        chunk[i + 1] = lo;
    }

    uint32_t total_bytes = (uint32_t)w * h * 2;
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

    while (total_bytes > 0) {
        uint16_t send_len = (total_bytes > sizeof(chunk)) ? sizeof(chunk) : total_bytes;
        HAL_SPI_Transmit(&hspi1, chunk, send_len, HAL_MAX_DELAY);
        total_bytes -= send_len;
    }

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

void ST7735_fill_screen(uint16_t color) {
    ST7735_fill_rect(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}
