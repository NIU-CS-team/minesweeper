#include "network.h"
#include "board.h"

#include <iostream>
#include <optional>

int Network::client() {
    sf::UdpSocket socket;
    std::optional<sf::IpAddress> ip;
    sf::Packet packet;

    if (socket.bind(this->port) != sf::Socket::Done) {
        std::cerr << "Failed to bind socket" << std::endl;
        return SOCKET_CREATE_FAILED;
    }

    do {
        std::cout << "Enter server ip: ";
        std::cin >> *ip;
    } while (ip.has_value());
    
    return 0;
}
