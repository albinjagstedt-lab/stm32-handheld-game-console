/*
 * input_handler.c
 *
 *  Created on: 7 nov. 2026
 *      Author: albin
 */

#include "main.h"

#define button_debounce_time 150

uint32_t last_tick_causing_exti = 0;
uint32_t last_tick_causing_debounce = 0;

int bl_press = 0;
int br_press = 0;
int ba_press = 0;
int bb_press = 0;
int bs_press = 0;

void debounced_button_press(uint16_t pin){
	if(last_tick_causing_exti >= (last_tick_causing_debounce + button_debounce_time)){
		last_tick_causing_debounce = last_tick_causing_exti;

		switch(pin){
			case button_left_Pin:
				bl_press++;
			break;
			case button_right_Pin:
				br_press++;
			break;
			case button_a_Pin:
				ba_press++;
			break;
			case button_b_Pin:
				bb_press++;
			break;
			case button_start_Pin:
				bs_press++;
			break;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t pin){
	if(pin == button_left_Pin || pin == button_right_Pin || pin == button_a_Pin || pin == button_b_Pin || pin == button_start_Pin){
		last_tick_causing_exti = HAL_GetTick();
		debounced_button_press(pin);
	}
}
