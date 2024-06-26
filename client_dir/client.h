#ifndef SERVER_H
# define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "../lib/include/mlx.h"

#define PORT 8080

# define KEY_PRESS		2
# define DESTROY_NOTIFY	17
# define ESC			53

typedef struct data_s
{
	void	*mlx;
	void	*win;
	int		win_width;
	int		win_height;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
	int		sock;
	char	*note;
}	data_t;

void	puterr(char *msg);
void	keycode_to_note(int keycode, char *note);
void	sound(char *note);
void	init_mlx(data_t *data, int sock);

#endif