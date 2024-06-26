#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Network.hpp>

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
    int client();

private:
    int port = 6969;

    int recv_data(int &socket_fd, char *buffer, int size);
    int send_data(int &socket_fd, const char *buffer, int size);
    int close_socket(int &socket_fd);
};

#endif