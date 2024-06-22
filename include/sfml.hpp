#ifndef SFML_HPP
#define SFML_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "board.h"

enum game_action { REVEAL, FLAG, REMOVE_FLAG, NONE };

class SFML : public Board {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture texture;
    sf::Sprite sprite;

    int mouse_input();
public:
    int init();
};
#endif