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

struct snake{
	struct position array[16];
	int head_ix;
	int tail_ix;
};
