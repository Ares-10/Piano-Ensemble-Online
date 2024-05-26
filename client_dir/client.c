#include "client.h"

static void	*message_handler(void *);
static void	start_application(int sock);
static int	close_callback(void);
static int	key_press(int keycode, t_data *data);

int main(void)
{
    int                 sock;
    struct sockaddr_in  server;
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
void    start_application(int sock)
{
    t_data	data;

    init_mlx(&data, sock);
	printf("Enter message: \n");
	mlx_hook(data.win, KEY_PRESS, 0, key_press, &data);
	mlx_hook(data.win, DESTROY_NOTIFY, 0, close_callback, &data);
	mlx_loop(data.mlx);
	exit(0);
}

int key_press(int keycode, t_data *data)
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

int	close_callback(void)
{
	exit(0);
	return (0);
}
