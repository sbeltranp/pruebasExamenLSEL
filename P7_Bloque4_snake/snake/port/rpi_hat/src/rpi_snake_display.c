#include "snake_display.h"

#define _GNU_SOURCE

#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"

struct fb_t {
	uint16_t pixel[8][8];
};

static int is_framebuffer_device(const struct dirent *dir)
{
	return strncmp(FB_DEV_NAME, dir->d_name,
		       strlen(FB_DEV_NAME)-1) == 0;
}

static int open_fbdev(const char *dev_name)
{
	struct dirent **namelist;
	int i, ndev;
	int fd = -1;
	struct fb_fix_screeninfo fix_info;

	ndev = scandir(DEV_FB, &namelist, is_framebuffer_device, versionsort);
	if (ndev <= 0)
		return ndev;

	for (i = 0; i < ndev; i++)
	{
		char fname[64];

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_FB, namelist[i]->d_name);
		fd = open(fname, O_RDWR);
		if (fd < 0)
			continue;
		ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
		if (strcmp(dev_name, fix_info.id) == 0)
			break;
		close(fd);
		fd = -1;
	}
	for (i = 0; i < ndev; i++)
		free(namelist[i]);

	return fd;
}

static int fbfd = 0;
static struct fb_t* fb = NULL;

void 
snake_display_render(snake_game_t* p_game)
{
  /* Change original code: color and X,Y arrangement */ 
  /* Correct arrangement is pixel[Y][X] */
  /* Color: RGB565 code: 5-bit RED, 6-bit GREEN, 5-bit BLUE */
  /* Draw apple as a GREEN pixel */
  /* Draw snake head as BLUE */
  /* Draw snake body as RED */

}


int
snake_display_init(snake_game_t* p_game)
{
  p_game->limits.x = 8;
  p_game->limits.y = 8;

  /* Get from original code. Return 0 if wrong and 1 if correct */
}

void
snake_display_close(snake_game_t* p_game)
{
  /* Get from original code, if something to close */
}
