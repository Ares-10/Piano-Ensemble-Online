#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *message_handler(void *);
void sound(char *note);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];
    pthread_t recv_thread;

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    // 서버에 연결
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    printf("Connected to server\n");

    // 메시지 수신을 위한 스레드 생성
    if (pthread_create(&recv_thread, NULL, message_handler, (void *)&sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }

    printf("음계 번호를 입력하세요 (종료하려면 0):\n");
    while (1)
    {
        printf("Enter message: ");
        scanf("%s", message);

        if (message[0] == '0')
        {
            printf("프로그램을 종료합니다.\n");
            break;
        }
        // 음계 출력
        sound(message);
        // 서버로 메시지 보내기
        if (send(sock, message, strlen(message), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
    }

    close(sock);
    return 0;
}

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
        printf("\nServer reply: %s\n", server_reply);
        puts("Enter message: ");
        memset(server_reply, 0, sizeof(server_reply)); // 메모리 초기화
    }

    if (read_size == 0)
    {
        puts("Server disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
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