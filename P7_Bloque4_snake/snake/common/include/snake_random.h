#ifndef _SNAKE_RANDOM_H
#define _SNAKE_RANDOM_H

#include "snake.h"

int snake_random_init(snake_game_t* p_game);
int snake_random_get(int limit);
void snake_random_close(snake_game_t* p_game);

#endif