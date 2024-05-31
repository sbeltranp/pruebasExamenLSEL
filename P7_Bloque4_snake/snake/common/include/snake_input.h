#ifndef _SNAKE_INPUT_H
#define _SNAKE_INPUT_H

#include "snake.h"

int snake_input_init(snake_game_t* p_game);
void snake_input_update_new_heading (snake_game_t* p_game);
void snake_input_close(snake_game_t* p_game);

#endif