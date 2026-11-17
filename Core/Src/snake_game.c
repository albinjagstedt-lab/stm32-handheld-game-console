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

void game_tick(){

}

void visual_tick(){

}

uint8_t counter = 0;
void snake_game_timer_callback(){
	if(counter > 8){
		counter = 0;
		game_tick();
	}
	counter++;
	visual_tick();
}

void snake_game_init(){
	HAL_TIM_Base_Start_IT(&htim7);
}

