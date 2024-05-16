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

struct game_data {
    int game_statu;
    int action;
    int x;
    int y;
};

enum connect_status {
    // Universal error
    SUCESS,
    SOCKET_CREATE_FAILED,
    SOCKET_CLOSE_ERROR,
    MESSENGE_SEND_ERROR,

    // host connect error: 1XX
    HOST_ERROR_CODE_SERIES = 100,
    BIND_FAILED,
    LISTEN_ERROR,

    // client connect error: 2XX
    CLIENT_ERROR_CODE_SERIES = 200,
    CONNECT_FAILED
};

int host_game(u_int16_t port, int max_member); // 開放起遊戲房間

int join_game(uint32_t host_address, uint16_t host_port); // 加入遊戲

int close_host(); // 關閉房間

#endif