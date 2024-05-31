#ifndef _SNAKE_DISPLAY_H
#define _SNAKE_DISPLAY_H

#include "snake.h"

void snake_display_render(snake_game_t* p_game);
int snake_display_init(snake_game_t* p_game);
void snake_display_close(snake_game_t* p_game);

#endif