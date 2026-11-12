/*
 * st7735.h
 *
 *  Created on: 12 nov. 2026
 *      Author: albin
 */

#ifndef INC_ST7735_H_
#define INC_ST7735_H_

#include "main.h"
#include <stdint.h>
#include <stddef.h>

// Screen dimensions (change if using 128x128 or 80x160)
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160

// Common RGB565 Color Definitions
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

// Function Prototypes
void ST7735_init(void);
void ST7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ST7735_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7735_fill_screen(uint16_t color);

#endif /* INC_ST7735_H_ */
