#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "snake.h"
#include "snake_input.h"
#include "snake_display.h"
#include "snake_random.h"
#include "snake_time.h"

int 
check_apple(snake_game_t* p_game)
{
  struct segment_t *seg_i;

  for (seg_i = p_game->snake.tail; seg_i; seg_i=seg_i->next) {
    if (seg_i->x == p_game->apple.x && seg_i->y == p_game->apple.y) {
      return 1;
    }
  }
  return 0;
}

int 
snake_check_collision(snake_game_t* p_game)
{
  struct segment_t *seg_i;

  for(seg_i = p_game->snake.tail; seg_i->next; seg_i=seg_i->next) {
    if (p_game->snake.head.x == seg_i->x && p_game->snake.head.y == seg_i->y) {
      return 1;
    }
  }

  if (p_game->snake.head.x < 0 || p_game->snake.head.x >= p_game->limits.x ||
      p_game->snake.head.y < 0 || p_game->snake.head.y >= p_game->limits.y) 
  {
    return 1;
  }
  return 0;
}

void
create_new_apple(snake_game_t* p_game)
{
  do {
    p_game->apple.x = snake_random_get(p_game->limits.x);
    p_game->apple.y = snake_random_get(p_game->limits.y);
  } while (check_apple(p_game));
}

void 
snake_game_logic(snake_game_t* p_game)
{
  struct segment_t *seg_i;
  struct segment_t *new_tail;
	
  for(seg_i = p_game->snake.tail; seg_i->next; seg_i=seg_i->next) {
    seg_i->x = seg_i->next->x;
    seg_i->y = seg_i->next->y;
  }
  if (check_apple(p_game)) {
    new_tail = malloc(sizeof(struct segment_t));
    if (!new_tail) {
      printf("Ran out of memory.\n");
      p_game->running = 0;
      return;
    }
    new_tail->x=p_game->snake.tail->x;
    new_tail->y=p_game->snake.tail->y;
    new_tail->next=p_game->snake.tail;
    p_game->snake.tail = new_tail;

    create_new_apple(p_game);
  }
  switch (p_game->snake.heading) {
    case LEFT:
      seg_i->x--;
      break;
    case DOWN:
      seg_i->y++;
      break;
    case RIGHT:
      seg_i->x++;
      break;
    case UP:
      seg_i->y--;
      break;
    default:
      break;
  }
}

void 
snake_reset_game(snake_game_t* p_game)
{
  struct segment_t *seg_i;
  struct segment_t *next_tail;
  p_game->snake.tail = &(p_game->snake.head);

  seg_i=p_game->snake.tail;
  while (seg_i->next) {
    next_tail=seg_i->next;
    free(seg_i);
    seg_i=next_tail;
  }
  p_game->snake.tail=seg_i;
  p_game->snake.tail->next=NULL;
  p_game->snake.tail->x=p_game->limits.x/2;
  p_game->snake.tail->y=p_game->limits.y/2;
  create_new_apple(p_game);

  p_game->snake.heading = NONE;
  p_game->running = 1;
}

void 
snake_change_dir(snake_game_t* p_game)
{
  switch (p_game->new_heading) {
    case UP:
      if (p_game->snake.heading != DOWN)
        p_game->snake.heading = UP;
      break;
    case RIGHT:
      if (p_game->snake.heading != LEFT)
        p_game->snake.heading = RIGHT;
      break;
    case DOWN:
      if (p_game->snake.heading != UP)
        p_game->snake.heading = DOWN;
      break;
    case LEFT:
      if (p_game->snake.heading != RIGHT)
        p_game->snake.heading = LEFT;
      break;
    default:
      break;
  }
}

int
snake_init(snake_game_t* p_game)
{
  p_game->apple.x = 0;
  p_game->apple.y = 0;

  p_game->snake.head.next = NULL;
  p_game->snake.head.x = 0;
  p_game->snake.head.y = 0;
  p_game->snake.tail = NULL;
  p_game->snake.heading = NONE;

  p_game->running = 1;
  p_game->limits.x = 1;
  p_game->limits.y = 1;
  p_game->new_heading = NONE;
  p_game->display_data = NULL;
  p_game->input_data = NULL;

  return snake_time_init(p_game) &&
         snake_input_init(p_game) &&
         snake_display_init(p_game) &&
         snake_random_init(p_game);
}

void
snake_close(snake_game_t* p_game)
{
  snake_random_close(p_game);
  snake_display_close(p_game);
  snake_input_close(p_game);
  snake_time_close(p_game);
}

