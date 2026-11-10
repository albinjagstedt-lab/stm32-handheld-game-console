/*
 * input_handler.c
 *
 *  Created on: 7 nov. 2026
 *      Author: albin
 */

#include "main.h"

#define DEBOUNCE_TIME 20
#define NUM_BUTTONS 5

//Button 0 - Button Left
//Button 1 - Button Right
//Button 2 - Button A
//Button 3 - Button B
//Button 4 - Button Start

uint32_t debounce_ticks[NUM_BUTTONS];
uint8_t is_holding[NUM_BUTTONS];

int button_presses[NUM_BUTTONS];

void button_press(int button_index, uint16_t pin){
	uint32_t now = HAL_GetTick();

		if(now < (debounce_ticks[button_index] + DEBOUNCE_TIME))
			return;

	is_holding[button_index] = (HAL_GPIO_ReadPin(GPIOA, pin) == GPIO_PIN_RESET);

	debounce_ticks[button_index] = now;
	button_presses[button_index]++;
}

void HAL_GPIO_EXTI_Callback(uint16_t pin){
	switch(pin){
		case button_left_Pin:
			button_press(0, pin);
		break;
		case button_right_Pin:
			button_press(1, pin);
		break;
		case button_a_Pin:
			button_press(2, pin);
		break;
		case button_b_Pin:
			button_press(3, pin);
		break;
		case button_start_Pin:
			button_press(4, pin);
		break;
	}
}
