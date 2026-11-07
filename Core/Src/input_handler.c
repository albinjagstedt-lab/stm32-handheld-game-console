/*
 * input_handler.c
 *
 *  Created on: 7 nov. 2026
 *      Author: albin
 */

#include "main.h"

#define button_press_debounce_time 20

uint32_t last_tick_causing_exti = 0;
uint32_t last_tick_causing_debounce = 0;

void debounced_button_press(uint16_t pin){
	if(last_tick_causing_exti >= (last_tick_causing_debounce + button_press_debounce_time)){

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t pin){
	if(pin == button_left_Pin || pin == button_right_Pin || pin == button_A_Pin || pin == button_B_Pin || pin == button_Start_Pin){
		last_tick_causing_exti = HAL_GetTick();
		debounced_button_press(pin);
	}
}
