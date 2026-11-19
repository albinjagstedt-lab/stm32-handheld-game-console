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
	right
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
struct position move_dir;

struct position snake_head;
struct position snake_queue[MAP_SIZE_X * MAP_SIZE_Y];
int queue_front_ix = 0;
int queue_back_ix = 0;

void queue_push_front(struct position pos){
	queue_front_ix = (queue_back_ix + 1) % (MAP_SIZE_X * MAP_SIZE_Y);
	snake_queue[queue_front_ix] = pos;
}

struct position queue_pop_back(){
	struct position pos = snake_queue[queue_back_ix];
	queue_back_ix = (queue_back_ix + 1) % (MAP_SIZE_X * MAP_SIZE_Y);
	return pos;
}

uint8_t check_collisions(){
	//TODO
}

void snake_move_head(){
	//TODO
}

void snake_eat_apple(){
	queue_push_front(snake_head);
	snake_move_head();
}

void snake_move(){
	queue_push_front(snake_head);
	queue_pop_back();
	snake_move_head();

}

void snake_init(){
	//TODO
}

// TIMED TICKS ----------------------------------------------------------------------------------------------

// Every 16th tick - 2hz
void game_tick(){
	//TODO
}

// Every 2nd tick - 16hz
void visual_tick(){
	//TODO
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
	snake_dir = left;
}

void snake_game_right_button_pressed(){
	snake_dir = right;
}

void snake_game_a_button_pressed(){
	snake_dir = left;
}

void snake_game_b_button_pressed(){
	snake_dir = right;
}

void snake_game_start_button_pressed(){
	//TODO
}

