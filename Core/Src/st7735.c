/*
 * st7735.c
 *
 *  Created on: 12 nov. 2026
 *      Author: albin
 */

#include "st7735.h"
#include "colors.h"

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
    if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT)
    	return;

    ST7735_set_address_window(x, y, x, y);

    uint8_t data[2];
    data[0] = (color >> 8) & 0xFF;
    data[1] = color & 0xFF;
    ST7735_write_data(data, 2);
}

void ST7735_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT)
    	return;
    if ((x + w - 1) >= ST7735_WIDTH)
    	w = ST7735_WIDTH - x;
    if ((y + h - 1) >= ST7735_HEIGHT)
    	h = ST7735_HEIGHT - y;

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

void sprite_overwrite(uint16_t *org, uint16_t *alt, uint32_t size){
	for(int i = 0; i < size; i++)
		org[i] = alt[i];
}

void rotate_sprite(uint16_t *width, uint16_t *height, uint16_t *sprite, uint8_t rotation){
	rotation--;
	uint16_t new_sprite[*width * *height];

	for(uint16_t y = 0; y < *height; y++)
		for(uint16_t x = 0; x < *width; x++)
			new_sprite[x * *height + (*height - 1 - y)] = sprite[y * *width + x];

	uint16_t temp = *width;
	*width = *height;
	*height = temp;

	sprite_overwrite(sprite, new_sprite, *width * *height);
	if(rotation > 0)
		rotate_sprite(width, height, sprite, rotation);
}

void mirror_sprite(uint16_t width, uint16_t height, uint16_t *sprite){
	uint16_t new_sprite[width * height];

	for(uint16_t y = 0; y < height; y++)
		for(uint16_t x = 0; x < width; x++)
			new_sprite[y * width + (width - 1 - x)] = sprite[y * width + x];

	sprite_overwrite(sprite, new_sprite, width * height);
}

void ST7735_draw_sprite(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *sprite, uint8_t rotation, uint8_t mirrored){
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    uint16_t temp_sprite[width * height];
	for(int i = 0; i < width * height; i++)
		temp_sprite[i] = sprite[i];

    if(mirrored % 2 > 0)
        mirror_sprite(width, height, temp_sprite);
    if(rotation > 0)
    	rotate_sprite(&width, &height, temp_sprite, rotation);

    if ((x + width) > ST7735_WIDTH)
        width = ST7735_WIDTH - x;
    if ((y + height) > ST7735_HEIGHT)
        height = ST7735_HEIGHT - y;

    ST7735_set_address_window(x, y, x + width - 1, y + height - 1);

    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

    for (uint32_t i = 0; i < (uint32_t)width * height; i++){
        uint8_t pixel[2];

        pixel[0] = temp_sprite[i] >> 8;
        pixel[1] = temp_sprite[i] & 0xFF;

        HAL_SPI_Transmit(&hspi1, pixel, 2, HAL_MAX_DELAY);
    }

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

void ST7735_draw_sprite_transparent(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *sprite, uint8_t rotation, uint8_t mirrored){
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    uint16_t temp_sprite[width * height];

    for(uint32_t i = 0; i < (uint32_t)width * height; i++)
        temp_sprite[i] = sprite[i];

    if(mirrored % 2)
        mirror_sprite(width, height, temp_sprite);

    if(rotation > 0)
        rotate_sprite(&width, &height, temp_sprite, rotation);

    for(uint16_t py = 0; py < height; py++){
        for(uint16_t px = 0; px < width; px++){
            uint16_t color = temp_sprite[py * width + px];

            if(color == TRANSPARENT)
                continue;

            if(x + px >= ST7735_WIDTH || y + py >= ST7735_HEIGHT)
                continue;

            ST7735_draw_pixel(x + px, y + py, color);
        }
    }
}
