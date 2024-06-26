#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Network.hpp>
#include <mutex>

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
    unsigned short port = 6969;
    std::mutex mtx;

    int play_multi(sf::UdpSocket& socket, sf::IpAddress& ip);
    int recv_data(sf::UdpSocket& socket, sf::Packet& packet);
    int send_data(sf::UdpSocket& socket, sf::IpAddress& ip, sf::Packet& packet);
    int close_socket(sf::UdpSocket& socket);
};

#endif