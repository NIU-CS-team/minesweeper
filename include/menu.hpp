#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>

class Menu {
private:
    std::vector<std::vector<int>> difficulty = {
        {9, 9, 10},
        {16, 16, 40},
        {30, 16, 99}
    };
    std::vector<std::string> difficulty_name = {
        "Easy",
        "Medium",
        "Hard"
    };

    sf::Event event;
    sf::RenderWindow window;
    sf::Image icon;
    sf::Font font;
    sf::Text title;
    sf::Text difficulty_text;
    sf::Texture difficulty_texture;
    sf::Sprite difficulty_sprite;

    int draw_difficulty();
    int draw_menu();
    int get_input();

public:
    Menu();
    class Server;

    int run();
};

class Menu::Server {
public:
    Server();
    int host();
    int client();
private:
    sf::UdpSocket socket;
    std::optional<sf::IpAddress> server_ip;
    unsigned short port = 6969;
    sf::Packet packet;
    sf::Event event;
    sf::Text text;
    sf::Font font;
    sf::RenderWindow window;
    std::string message;

    unsigned create_seed();
};

#endif