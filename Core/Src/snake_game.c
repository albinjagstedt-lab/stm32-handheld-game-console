/*
 * snake_game.c
 *
 *  Created on: 17 nov. 2026
 *      Author: albin
 */

/*
 * snake_game.c
 *
 *  Created on: 9 nov. 2026
 *      Author: albin
 */

#include "main.h"
#include "Textures/snake_textures.h"

extern TIM_HandleTypeDef htim7;

#define MAP_SIZE_X 16
#define MAP_SIZE_Y 16

struct position{
	int x;
	int y;
};

enum turn_dir{
	forward,
	left,
	right,
	back
};

enum snake_part{
	head,
	tail,
	body_straight,
	body_right,
	body_left,
	head_eat
};

enum turn_dir snake_dir = forward;

enum snake_part snake_queue[128];
uint8_t queue_front_ix = 0;
uint8_t queue_back_ix = 0;

void queue_push_back(){

}

enum snake_part queue_pop_back(){

}

void snake_eat_apple(){

}

void snake_move(){

}

void game_tick(){

}

void visual_tick(){

}

uint8_t counter = 0;
void snake_game_timer_callback(){
	if(counter > 16){
		counter = 0;
		game_tick();
	}
	if(counter % 2 == 0)
		visual_tick();
	counter++;
}

void snake_game_init(){
	HAL_TIM_Base_Start_IT(&htim7);
}

// CONTROLS ----------------------------------------------------------------------------------------------

void snake_game_left_button_pressed(){

}

void snake_game_right_button_pressed(){

}

void snake_game_a_button_pressed(){

}

void snake_game_b_button_pressed(){

}

void snake_game_start_button_pressed(){

}

