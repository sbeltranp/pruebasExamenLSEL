#include "snake.h"
#include "snake_display.h"
#include "snake_time.h"

snake_game_t game;

int main(int argc, char* args[])
{
	snake_init(&game);

	snake_reset_game(&game);
	while (game.running) {
		snake_input_update_new_heading(&game);
		snake_change_dir(&game);
		snake_game_logic(&game);
		if (snake_check_collision(&game)) {
			snake_reset_game(&game);
		}
		snake_display_render(&game);
		snake_time_delay_ms (300);
	}

	snake_reset_game(&game);

	snake_close(&game);

	return 0;
}