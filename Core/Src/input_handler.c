/*
 * input_handler.c
 *
 *  Created on: 7 nov. 2026
 *      Author: albin
 */

#include "main.h"

#define DEBOUNCE_TIME 30

uint32_t debounce_ticks[5];
int button_presses[5];

void button_press(int button_index){
	uint32_t now = HAL_GetTick();

		if(now < (debounce_ticks[button_index] + DEBOUNCE_TIME))
			return;

	debounce_ticks[button_index] = now;
	button_presses[button_index]++;
}

void HAL_GPIO_EXTI_Callback(uint16_t pin){
	switch(pin){
		case button_left_Pin:
			button_press(0);
		break;
		case button_right_Pin:
			button_press(1);
		break;
		case button_a_Pin:
			button_press(2);
		break;
		case button_b_Pin:
			button_press(3);
		break;
		case button_start_Pin:
			button_press(4);
		break;
	}
}
