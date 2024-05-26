#include <pthread.h>
#include <stdio.h>
#include <string.h> // strlen 함수 사용을 위해 추가
#include <stdlib.h> // malloc, free 함수 사용을 위해 추가
#include <unistd.h> // write 함수 사용을 위해 추가
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_addr 함수 사용을 위해 추가

#define PORT 8080
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS] = {0}; // 클라이언트 소켓을 저장할 배열
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_client_socket(int client_sock)
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (client_sockets[i] == 0)
        {
            client_sockets[i] = client_sock;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client_socket(int client_sock)
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (client_sockets[i] == client_sock)
        {
            client_sockets[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void broadcast_message(char *message, int origin_sock)
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (client_sockets[i] != 0 && client_sockets[i] != origin_sock)
        {
            write(client_sockets[i], message, strlen(message));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// 클라이언트 처리를 위한 스레드 함수
void *clientHandler(void *socket_desc)
{
    int sock = *(int *)socket_desc;
    int read_size;
    char client_message[2000];
    char message[2000];

    // 클라이언트에서 메시지 받기
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {
        // 받은 메시지를 모든 클라이언트에게 브로드캐스트
        client_message[read_size] = '\0'; // 메시지 종료 지점에 NULL 문자 추가
        broadcast_message(client_message, sock);
    }

    if (read_size == 0)
    {
        printf("Client disconnected: %d\n", sock);
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    // 클라이언트 소켓 배열에서 제거
    remove_client_socket(sock);
    free(socket_desc);
    return 0;
}

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    // 소켓 생성
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    // 소켓 구성
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // 소켓 바인드
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }

    // 리스닝
    listen(socket_desc, 3);

    // 클라이언트 연결 수락
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        add_client_socket(client_sock); // 클라이언트 소켓 추가

        if (pthread_create(&sniffer_thread, NULL, clientHandler, (void *)new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        // 클라이언트를 위한 스레드 생성
        printf("Handler assigned: %d\n", client_sock);
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}
