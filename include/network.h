#ifndef NETWORK_H
#define NETWORK_H
#if defined(_WIN32)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Ws2tcpip.h>
#include <winsock2.h>

#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#endif

#include <unistd.h>

#include "sfml.hpp"

enum connect_status {
    // Universal error
    SUCESS,
    SOCKET_CREATE_FAILED,
    SOCKET_CLOSE_ERROR,
    MESSENGE_SEND_ERROR,
    MESSENGE_RECV_ERROR,
    GAME_DATA_UPDATE_ERROR,
    GAME_INTERACTION_ERROR,

    // host connect error: 1XX
    HOST_ERROR_CODE_SERIES = 100,
    BIND_FAILED,
    LISTEN_ERROR,

    // client connect error: 2XX
    CLIENT_ERROR_CODE_SERIES = 200,
    CONNECT_FAILED
};

class Network : public SFML {
public:
    Network(int row, int col, int mines) : SFML(row, col, mines) {};
    int host();
    int client(const char *ip);
    int send_data(int &socket_fd, std::pair<game_action, int> data);
    int close_socket(int &socket_fd);

private:
    int recv_data(int &socket_fd, char *buffer,
                  std::pair<game_action, int> *data);
};

#endif