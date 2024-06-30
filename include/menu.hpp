#ifndef MENU_H
#define MENU_H

#include "network.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <optional>
#include <vector>

class Menu {
private:
    std::vector<std::vector<int>> difficulty = {
        {9, 9, 10}, {16, 16, 40}, {30, 16, 99}};
    std::vector<std::string> difficulty_name = {"Easy", "Medium", "Hard"};
    std::vector<std::vector<std::string>> mode = {{"Single", "multiplayer"},
                                                  {"Host", "Join"},
                                                  {"Easy", "Medium", "Hard"}};

    sf::Event event;
    sf::RenderWindow window;
    sf::Image icon;
    sf::Font font;
    sf::Text title;
    sf::Text difficulty_text;
    sf::Text menu_text;
    sf::Texture texture;
    sf::Sprite sprite;

    int draw_difficulty();
    int draw_menu(int mode_index);
    int draw_quit(bool is_pressed = false);
    int draw_button(int mode_index, int button_index, bool is_pressed = false);
    int get_input();
    int host();
    int client();
    unsigned create_seed();

public:
    Menu();
    class Server;

    int run();
};

#endif