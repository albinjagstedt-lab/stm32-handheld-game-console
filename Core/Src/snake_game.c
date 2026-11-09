/*
 * snake_game.c
 *
 *  Created on: 9 nov. 2026
 *      Author: albin
 */

#include "main.h"

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

void queue_push_front(struct position pos){

}

void queue_pop_back(){

}

struct position queue_get_front(){

}

struct snake{
	struct position queue[16];
	struct position move_dir;
};

void snake_init(){

}

void snake_move(enum turn_dir dir){

}

void snake_grow(enum turn_dir dir){

}
