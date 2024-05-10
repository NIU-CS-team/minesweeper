#ifndef NETWORK_H
#define NETWORK_H
#if defined(_WIN32)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Ws2tcpip.h>
#include <winsock2.h>

#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#endif

int socket_fd;

sockaddr_in address;

int host_game(int port, int member); // 開放起遊戲房間

int join_game(int host_address); // 加入遊戲

int close_host(); // 關閉房間

#endif