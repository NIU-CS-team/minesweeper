#include "menu.hpp"
#include "network.h"
#include "sfml.hpp"

#include <functional>
#include <thread>

Menu::Menu() {
    window.create(sf::VideoMode(260, 260), "Minesweeper");
    icon.loadFromFile("../image/icon/menu.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    if (!font.loadFromFile("../font/Cubic_11_1.100_R.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    if (!difficulty_texture.loadFromFile("../image/difficulty.png")) {
        std::cerr << "Failed to load difficulty.png" << std::endl;
    }

    title.setFont(font);
    title.setString("Minesweeper!");
    title.setCharacterSize(24);
    title.setPosition(60, 20);
    title.setFillColor(sf::Color::White);

    difficulty_sprite.setTexture(difficulty_texture);
    difficulty_sprite.setScale(2, 2);

    difficulty_text.setFont(font);
    difficulty_text.setCharacterSize(16);
    difficulty_text.setFillColor(sf::Color::White);
}

int Menu::draw_difficulty() {
    for (int i = 0; i < 3; i++) {
        difficulty_sprite.setTextureRect(sf::IntRect(0, i * 54, 130, 26));
        difficulty_sprite.setPosition(0, 80 + i * 60);
        window.draw(difficulty_sprite);
        difficulty_text.setString(difficulty_name[i] + "\n" +
                                  std::to_string(difficulty[i][0]) + "x" +
                                  std::to_string(difficulty[i][1]) + " " +
                                  std::to_string(difficulty[i][2]) + " mines");
        difficulty_text.setPosition(65, 82 + i * 60);
        window.draw(difficulty_text);
    }

    int input = get_input();
    if (input != -1) {
        difficulty_sprite.setTextureRect(
            sf::IntRect(0, 27 + input * 54, 130, 26));
        difficulty_sprite.setPosition(0, 80 + input * 60);
        window.draw(difficulty_sprite);
        difficulty_text.setString(difficulty_name[input] + "\n" +
                                  std::to_string(difficulty[input][0]) + "x" +
                                  std::to_string(difficulty[input][1]) + " " +
                                  std::to_string(difficulty[input][2]) +
                                  " mines");
        difficulty_text.setPosition(68, 85 + input * 60);
        window.draw(difficulty_text);
        window.display();

        while (window.waitEvent(event)) {
            if (event.type == sf::Event::MouseButtonReleased) {
                difficulty_sprite.setTextureRect(
                    sf::IntRect(0, input * 54, 130, 26));
                difficulty_sprite.setPosition(0, 80 + input * 60);
                window.draw(difficulty_sprite);
                difficulty_text.setString(
                    difficulty_name[input] + "\n" +
                    std::to_string(difficulty[input][0]) + "x" +
                    std::to_string(difficulty[input][1]) + " " +
                    std::to_string(difficulty[input][2]) + " mines");
                difficulty_text.setPosition(65, 82 + input * 60);
                window.draw(difficulty_text);
                window.display();
                SFML game(difficulty[input][0], difficulty[input][1],
                          difficulty[input][2]);
                game.play_single();
                return 0;
            }
        }
    }

    return 0;
}

int Menu::get_input() {
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (mouse.y >= 80 && mouse.y <= 132) {
            return 0;
        } else if (mouse.y >= 140 && mouse.y <= 192) {
            return 1;
        } else if (mouse.y >= 200 && mouse.y <= 252) {
            return 2;
        }
    }
    return -1;
}

int Menu::run() {
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        window.draw(title);
        draw_difficulty();

        window.display();
    }
    return 0;
}

int Menu::Server::client() {
    sf::Packet packet;
    unsigned seed;

    if (socket.bind(this->port) != sf::Socket::Done) {
        std::cerr << "Failed to bind socket" << std::endl;
        return SOCKET_CREATE_FAILED;
    }

    do {
        std::cout << "Enter server ip: ";
        std::cin >> server_ip.value();
    } while (server_ip.has_value());

    packet << "Minesweeper";
    if (socket.send(packet, server_ip.value(), this->port) !=
        sf::Socket::Done) {
        std::cerr << "Failed to send packet" << std::endl;
        return MESSENGE_SEND_ERROR;
    }

    if (socket.receive(packet, server_ip.value(), this->port) !=
        sf::Socket::Done) {
        std::cerr << "Failed to receive packet" << std::endl;
        return MESSENGE_RECV_ERROR;
    }
    packet >> seed;
    Network game(30, 16, 99);
    std::thread(static_cast<int (Network::*)()>(&Network::recv_data), &game)
        .detach();
    game.play_multi(server_ip.value(), seed);

    return 0;
}

int Menu::Server::host() {
    server_ip.value() = sf::IpAddress::getLocalAddress();
    std::vector<sf::IpAddress> clients;
    sf::SocketSelector selector;
    unsigned int max_clients = 0;

    if (socket.bind(this->port) != sf::Socket::Done) {
        std::cerr << "Failed to bind socket" << std::endl;
        return SOCKET_CREATE_FAILED;
    }
    selector.add(socket);
    std::cout << "Server ip: " << server_ip.value() << std::endl;

    do {
        std::cout << "Enter max clients(1 ~ 10):";
        std::cin >> max_clients;
    } while (max_clients < 1 || max_clients > 10);

    sf::Packet seed;
    seed << create_seed();

    std::cout << "Waiting for clients..." << std::endl;
    while (selector.wait()) {
        if (selector.isReady(socket)) {
            sf::IpAddress client;
            sf::Packet packet;
            if (socket.receive(packet, client, this->port) !=
                sf::Socket::Done) {
                std::cerr << "Failed to receive packet" << std::endl;
                return MESSENGE_RECV_ERROR;
            }
            if (std::find(clients.begin(), clients.end(), client) ==
                clients.end()) {
                clients.push_back(client);
            }
            if (clients.size() == max_clients) {
                break;
            }
        }
    }

    for (auto& client : clients) {
        if (socket.send(seed, client, this->port) != sf::Socket::Done) {
            std::cerr << "Failed to send packet" << std::endl;
            return MESSENGE_SEND_ERROR;
        }
    }

    unsigned seed_value;
    seed >> seed_value;
    Network game(30, 16, 99);
    std::thread(static_cast<int (Network::*)(std::vector<sf::IpAddress>&)>(
                    &Network::recv_data),
                &game, std::ref(clients))
        .detach();
    game.play_multi(clients, seed_value);

    return 0;
}

unsigned Menu::Server::create_seed() {
    std::random_device rd;
    return rd();
}