#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "minilibx/mlx.h"

#define PORT 8080

# define KEY_PRESS		2
# define DESTROY_NOTIFY	17
# define ESC			53

typedef struct s_data
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
}	t_data;

static void	*message_handler(void *);
static void	puterr(char *msg);
static void	sound(char *note);
static void	start_application(int sock);
static int	close_callback(void);
static int	key_press(int keycode, t_data *data);
static void	keycode_to_note(int keycode, char *note);

int main(int argc, char *argv[])
{
    int                 sock;
    struct sockaddr_in  server;
    char                server_reply[2000];
    pthread_t           recv_thread;

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        puterr("Could not create socket\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
        puterr("connect failed.\n");
    
    printf("Connected to server\n");

    // 메시지 수신을 위한 스레드 생성
    if (pthread_create(&recv_thread, NULL, message_handler, (void *)&sock) < 0)
        puterr("could not create thread\n");

    start_application(sock);

    close(sock);
    return 0;
}

// 서버에서 수신받은 음계
void *message_handler(void *socket_desc)
{
    // 소켓 번호를 가져온다.
    int sock = *(int *)socket_desc;
    int read_size;
    char server_reply[2000];

    // 서버로부터 메시지를 계속해서 받는다.
    while ((read_size = recv(sock, server_reply, 2000, 0)) > 0)
    {
        sound(server_reply);
        printf("Server input: %s\n", server_reply);
        memset(server_reply, 0, sizeof(server_reply)); // 메모리 초기화
    }

    if (read_size == 0)
    {
        puts("Server disconnected\n");
        fflush(stdout);
    }
    else if (read_size == -1)
        perror("recv failed\n");

    return 0;
}

// 본인이 입력한 음계
void start_application(int sock)
{
    t_data	data;

	data.win_width = 720;
	data.win_height = 480;
	data.sock = sock;
	data.note = (char *)malloc(sizeof(char) * 2);
	if (!data.note)
		puterr("malloc failed\n");

    data.mlx = mlx_init();
	data.win = mlx_new_window(data.mlx, data.win_width, data.win_height, "client");
	data.img = mlx_new_image(data.mlx, data.win_width, data.win_height);
	data.addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_length, &data.endian);

	printf("Enter message: \n");
	mlx_hook(data.win, KEY_PRESS, 0, key_press, &data);
	mlx_hook(data.win, DESTROY_NOTIFY, 0, close_callback, &data);
	mlx_loop(data.mlx);
	exit(0);
}

int	key_press(int keycode, t_data *data)
{
	if (keycode >= 18 && keycode <= 28)
	{
		keycode_to_note(keycode, data->note);
        sound(data->note);
		if (send(data->sock, data->note, 1, 0) < 0)
            puterr("Send failed\n");
		printf("Client input: %s\n", data->note);
	}
	if (keycode == ESC)
	{
		mlx_destroy_window(data->mlx, data->win);
		exit(0);
	}
	return (0);
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

void sound(char *note)
{
    int scale;
    for (int i = 0; i < strlen(note); i++)
    {
        scale = note[i] - '0';

        switch (scale)
        {
        case 1:
            system("afplay sound/do.wav &");
            break;
        case 2:
            system("afplay sound/re.wav &");
            break;
        case 3:
            system("afplay sound/mi.wav &");
            break;
        case 4:
            system("afplay sound/fa.wav &");
            break;
        case 5:
            system("afplay sound/sol.wav &");
            break;
        case 6:
            system("afplay sound/la.wav &");
            break;
        case 7:
            system("afplay sound/si.wav &");
            break;
        case 8:
            system("afplay sound/do_high.wav &");
            break;
        default:
            printf("잘못된 입력입니다. 1~8 사이의 숫자를 입력해주세요.\n");
        }
    }
}

void puterr(char *msg)
{
    write(2, "Error: ", 7);
    write(2, msg, strlen(msg));
    exit(1);
}

int	close_callback(void)
{
	exit(0);
	return (0);
}
