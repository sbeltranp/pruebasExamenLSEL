#include "snake_input.h"

#define _GNU_SOURCE

#include <linux/input.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

static int is_event_device(const struct dirent *dir)
{
	return strncmp(EVENT_DEV_NAME, dir->d_name,
		       strlen(EVENT_DEV_NAME)-1) == 0;
}

static int open_evdev(const char *dev_name)
{
	struct dirent **namelist;
	int i, ndev;
	int fd = -1;

	ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
	if (ndev <= 0)
		return ndev;

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		char name[256];

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
		fd = open(fname, O_RDONLY);
		if (fd < 0)
			continue;
		ioctl(fd, EVIOCGNAME(sizeof(name)), name);
		if (strcmp(dev_name, name) == 0)
			break;
		close(fd);
	}

	for (i = 0; i < ndev; i++)
		free(namelist[i]);

	return fd;
}

static void
handle_events(int evfd, snake_game_t* p_game)
{
	struct input_event ev[64];
	int i, rd;

	rd = read(evfd, ev, sizeof(struct input_event) * 64);
	if (rd < (int) sizeof(struct input_event)) {
		fprintf(stderr, "expected %d bytes, got %d\n",
		        (int) sizeof(struct input_event), rd);
		return;
	}
	
	for (i = 0; i < (int)(rd / sizeof(struct input_event)); i++) {
		if (ev->type != EV_KEY)
			continue;
		if (ev->value != 1)
			continue;
		switch (ev->code) {
			case KEY_UP:
				p_game->new_heading = UP;
				break;
			case KEY_DOWN:
				p_game->new_heading = DOWN;
				break;
			case KEY_LEFT:
				p_game->new_heading = LEFT;
				break;
			case KEY_RIGHT:
				p_game->new_heading = RIGHT;
				break;
			default:
				break;
		}
	}
}


static struct pollfd evpoll = {
	.events = POLLIN,
};

int
snake_input_init(snake_game_t* p_game)
{
  /* Get from original code. Return 0 if wrong and 1 if correct */
}

void
snake_input_update_new_heading (snake_game_t* p_game)
{
  //Update new_heading value
  p_game->new_heading = p_game->snake.heading;

  //Process input events if any
  while (poll(&evpoll, 1, 0) > 0) {
    handle_events(evpoll.fd, p_game);
  }
}

void
snake_input_close(snake_game_t* p_game)
{
  /* Get from original code, if something to close */
}




