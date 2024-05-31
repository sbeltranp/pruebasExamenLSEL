#ifndef _SNAKE_H
#define _SNAKE_H

enum direction_t{
	UP,
	RIGHT,
	DOWN,
	LEFT,
	NONE,
};

struct segment_t {
	struct segment_t *next;
	int x;
	int y;
};
struct snake_t {
	struct segment_t head;
	struct segment_t *tail;
	enum direction_t heading;
};
struct apple_t {
	int x;
	int y;
};

typedef struct {
	struct apple_t apple;
	struct snake_t snake;
	struct apple_t limits;
	enum direction_t new_heading;
	int running;
	void*  display_data;
	void*  input_data;
} snake_game_t;

int snake_init(snake_game_t*);
void snake_reset_game(snake_game_t*);
void snake_input_update_new_heading(snake_game_t*);
void snake_change_dir(snake_game_t*);
void snake_game_logic(snake_game_t*);
int snake_check_collision(snake_game_t*);
void snake_close(snake_game_t*);

#endif