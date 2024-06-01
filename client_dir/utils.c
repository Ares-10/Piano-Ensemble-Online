#include "client.h"

void puterr(char *msg)
{
    write(2, "Error: ", 7);
    write(2, msg, strlen(msg));
    exit(1);
}

void sound(char *note)
{
    int scale;
    int len;

    len = strlen(note);
    for (unsigned long i = 0; i < len; i++)
    {
        scale = note[i] - '0';

        switch (scale)
        {
        case 1:
            system("afplay sound/A.wav &");
            break;
        case 2:
            system("afplay sound/B.wav &");
            break;
        case 3:
            system("afplay sound/C.wav &");
            break;
        case 4:
            system("afplay sound/D.wav &");
            break;
        case 5:
            system("afplay sound/E.wav &");
            break;
        case 6:
            system("afplay sound/F.wav &");
            break;
        case 7:
            system("afplay sound/G.wav &");
            break;
        case 8:
            system("afplay sound/A_high.wav &");
            break;
        default:
            printf("잘못된 입력입니다. 1~8 사이의 숫자를 입력해주세요.\n");
        }
    }
}

void	init_mlx(data_t *data, int sock)
{
	data->win_width = 720;
	data->win_height = 480;
	data->sock = sock;
	data->note = (char *)malloc(sizeof(char) * 2);
	if (!data->note)
		puterr("malloc failed\n");

    data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, data->win_width, data->win_height, "client");
	data->img = mlx_new_image(data->mlx, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_length, &data->endian);
}

void keycode_to_note(int keycode, char *note)
{
    if (keycode >= 18 && keycode <= 28)
    {
        switch (keycode)
        {
            case 18:
                note[0] = '1';
                break;
            case 19:
                note[0] = '2';
                break;
            case 20:
                note[0] = '3';
                break;
            case 21:
                note[0] = '4';
                break;
            case 23:
                note[0] = '5';
                break;
            case 22:
                note[0] = '6';
                break;
            case 26:
                note[0] = '7';
                break;
            case 28:
                note[0] = '8';
                break;
            default:
                note[0] = '\0';  // Undefined keycode
                break;
        }
        note[1] = '\0';  // Null-terminate the string
    }
    else
    {
        note[0] = '\0';  // Undefined keycode
        note[1] = '\0';  // Null-terminate the string
    }
}
