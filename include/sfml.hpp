#ifndef SFML_HPP
#define SFML_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "board.h"

enum game_action { REVEAL, FLAG, REMOVE_FLAG };

class SFML : public Board {
public:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture texture;
    sf::Sprite sprite;

    int init();
};
#endif