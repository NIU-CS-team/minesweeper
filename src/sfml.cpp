#include "sfml.hpp"

int SFML::init() {
    window.create(sf::VideoMode(800, 800), "Minesweeper");
    window.setFramerateLimit(60);

    if(!window.isOpen()) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }
    if (!font.loadFromFile("../font/Cubic_11_1.100_R.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }

    return 0;
}