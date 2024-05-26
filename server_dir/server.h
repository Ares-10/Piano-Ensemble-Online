#ifndef SERVER_H
# define SERVER_H

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

int				client_sockets[MAX_CLIENTS] = {0}; // 클라이언트 소켓을 저장할 배열
pthread_mutex_t	clients_mutex = PTHREAD_MUTEX_INITIALIZER;

#endif