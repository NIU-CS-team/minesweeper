#ifndef SFML_HPP
#define SFML_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

enum game_action { REVEAL, FLAG, REMOVE_FLAG };

class SFML {
public:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture texture;
    sf::Sprite sprite;

    int init();
};
#endif