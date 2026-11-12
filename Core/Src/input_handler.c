/*
 * input_handler.c
 *
 *  Created on: 13 nov. 2026
 *      Author: albin
 */

/* INPUT HANDLING -------------------------------------------------*/

#include "main.h"

extern TIM_HandleTypeDef htim6;

#define DEBOUNCE_TIME 20
#define NUM_BUTTONS 5

//Button 0 - Button Left
//Button 1 - Button Right
//Button 2 - Button A
//Button 3 - Button B
//Button 4 - Button Start

uint32_t button_debounce_ticks[NUM_BUTTONS];
uint8_t button_state_changed[NUM_BUTTONS];
uint8_t is_holding_button[NUM_BUTTONS];
uint16_t button_pins[NUM_BUTTONS] = {button_left_Pin, button_right_Pin, button_a_Pin, button_b_Pin, button_start_Pin};

int button_presses[NUM_BUTTONS];

void button_update(int button_index, uint16_t pin){
	uint32_t now = HAL_GetTick();

	uint8_t was_button_held = is_holding_button[button_index];
	is_holding_button[button_index] = (HAL_GPIO_ReadPin(GPIOA, pin) == GPIO_PIN_RESET);

	if(!was_button_held && is_holding_button[button_index] &&
		button_state_changed[button_index] && now > (button_debounce_ticks[button_index] + DEBOUNCE_TIME))
	{
		button_state_changed[button_index] = 0;
		button_debounce_ticks[button_index] = now;
		button_presses[button_index]++;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim != &htim6)
		return;
	for(int i = 0; i < NUM_BUTTONS; ++i)
		button_update(i, button_pins[i]);
}

void HAL_GPIO_EXTI_Callback(uint16_t pin){
	switch(pin){
		case button_left_Pin:
			button_state_changed[0] = 1;
		break;
		case button_right_Pin:
			button_state_changed[1] = 1;
		break;
		case button_a_Pin:
			button_state_changed[2] = 1;
		break;
		case button_b_Pin:
			button_state_changed[3] = 1;
		break;
		case button_start_Pin:
			button_state_changed[4] = 1;
		break;
	}
}
