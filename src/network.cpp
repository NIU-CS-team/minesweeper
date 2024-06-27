#include "network.h"
#include "board.h"

#include <iostream>
#include <optional>
#include <vector>
#include <algorithm>

int Network::client() {
    std::optional<sf::IpAddress> ip;
    sf::Packet packet;

    if (socket.bind(this->port) != sf::Socket::Done) {
        std::cerr << "Failed to bind socket" << std::endl;
        return SOCKET_CREATE_FAILED;
    }

    do {
        std::cout << "Enter server ip: ";
        std::cin >> ip.value();
    } while (ip.has_value());

    packet << "Minesweeper";
    if (socket.send(packet, ip.value(), this->port) != sf::Socket::Done) {
        std::cerr << "Failed to send packet" << std::endl;
        return MESSENGE_SEND_ERROR;
    }
    
    return 0;
}

int Network::host() {
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    std::vector<sf::IpAddress> clients;
    sf::SocketSelector selector;
    unsigned int max_clients = 0;

    if (socket.bind(this->port) != sf::Socket::Done) {
        std::cerr << "Failed to bind socket" << std::endl;
        return SOCKET_CREATE_FAILED;
    }
    selector.add(socket);
    std::cout << "Server ip: " << ip << std::endl;

    do {
        std::cout << "Enter max clients(1 ~ 10):";
        std::cin >> max_clients;
    } while (max_clients < 1 || max_clients > 10);
    
    std::cout << "Waiting for clients..." << std::endl;
    while (selector.wait()) {
        if (selector.isReady(socket)) {
            sf::IpAddress client;
            sf::Packet packet;
            if (socket.receive(packet, client, this->port) != sf::Socket::Done) {
                std::cerr << "Failed to receive packet" << std::endl;
                return MESSENGE_RECV_ERROR;
            }
            if (std::find(clients.begin(), clients.end(), client) == clients.end()){
                clients.push_back(client);
            }
            if (clients.size() == max_clients) {
                break;
            }
        }
    }
    

    return 0;
}

unsigned Network::create_seed() {
    std::random_device rd;
    return rd();
}

int Network::generate_mines(unsigned seed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(0, this->row * this->col - 1);
    for (int i = 0; i < this->n_mines; i++) {
        int rand = dis(gen);
        if (this->blocks[rand].value >= block::MINE) {
            i--;
            continue;
        }

        this->blocks[rand].value = 9;
        for (int j = -1; j <= 1; j++) {
            if (rand < row && j == -1) continue;
            if (rand >= row * (col - 1) && j == 1) continue;

            for (int k = -1; k <= 1; k++) {
                if (rand % row == 0 && k == -1) continue;
                if (rand % row == row - 1 && k == 1) continue;
                if (this->blocks[rand + j * row + k].value < block::MINE) {
                    this->blocks[rand + j * row + k].value++;
                }
            }
        }
    }
    return 0;

}

int Network::send_data(sf::IpAddress& ip, sf::Packet& packet){
    if (socket.send(packet, ip, this->port) != sf::Socket::Done) {
        std::cerr << "Failed to send packet" << std::endl;
        return MESSENGE_SEND_ERROR;
    }
    return SUCCESS;
}

sf::Packet& operator>>(sf::Packet& packet, const SFML::game_action& action) {
    return packet;
}

int Network::recv_data(sf::Packet& packet) {
    sf::IpAddress ip;
    if (socket.receive(packet, ip, this->port) != sf::Socket::Done) {
        std::cerr << "Failed to receive packet" << std::endl;
        return MESSENGE_RECV_ERROR;
    }

    game_action action;
    int block;
    packet >> action >> block;
    mtx.lock();
    if (action == REVEAL) {
        reveal(blocks[block]);
    } else if (action == FLAG) {
        flip_flag(blocks[block]);
    }
    mtx.unlock();

    return SUCCESS;
}

int Network::play_multi(sf::IpAddress& ip, unsigned seed) {
    init_block();
    generate_mines(seed);
    while (window.isOpen() && status == PLAYING) {
        window.clear(sf::Color::Black);
        draw_board();
        draw_flag();
        draw_time();
        window.display();

        mtx.lock();
        auto input = mouse_input();
        mtx.unlock();

        if (input.second == -2) {
            return 0;
        }
        if (input.first != NONE) {
            sf::Packet packet;
            packet << input.first << input.second;
            if (send_data(ip, packet) != SUCCESS) {
                return GAME_INTERACTION_ERROR;
            }
        }
        check_win();
    }
    end_game();

    return 0;
}

int Network::close_socket() {
    socket.unbind();
    return SUCCESS;
}