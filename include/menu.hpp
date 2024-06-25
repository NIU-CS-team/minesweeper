#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Menu {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture difficulty_texture;
    sf::Sprite difficulty_sprite;

    int draw_difficulty();
    int draw_menu();
    int get_menu_input();

public:
    Menu();

    int run();
};

#endif