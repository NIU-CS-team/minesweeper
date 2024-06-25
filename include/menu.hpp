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

    int draw_menu();
    int get_menu_input();
public:
    Menu() {
        window.create(sf::VideoMode(800, 600), "Minesweeper!");
        if (!font.loadFromFile("../font/Cubic_11_1.100_R.ttf")) {
            std::cerr << "Failed to load font" << std::endl;
        }
        if (!difficulty_texture.loadFromFile("../image/difficulty.png")) {
            std::cerr << "Failed to load difficulty.png" << std::endl;
        }
    };
    int run();

};

#endif