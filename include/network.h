#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Network.hpp>
#include <random>
#include <optional>
#include <vector>

#include "sfml.hpp"

enum connect_status {
    // Universal error
    SUCCESS,
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
    Network(int width, int height, int mines) : SFML(width, height, mines) {};
    ~Network();

    int play_multi(sf::IpAddress& ip, unsigned seed);
    int recv_data();
    int recv_data(std::vector<sf::IpAddress>& clients);

protected:
    unsigned short port = 6969;
    unsigned seed;
    sf::Mutex mtx;
    sf::UdpSocket socket;
    std::optional<sf::IpAddress> server_ip;

    int generate_mines(unsigned seed);
    int send_data(sf::IpAddress& ip, sf::Packet& packet);
};

#endif