#ifndef SFML_HPP
#define SFML_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class SFML {
public:
    sf::RenderWindow window;
    sf::Font font;

    int init();
};
#endif