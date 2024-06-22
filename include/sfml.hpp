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

    std::pair<game_action, int> mouse_input();
    block get_block(double x, double y);

public:
    int init();
};
#endif