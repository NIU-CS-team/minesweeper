#include "sfml.hpp"

int SFML::init() {
    window.create(sf::VideoMode(800, 800), "Minesweeper");
    window.setFramerateLimit(60);
    if (!font.loadFromFile("Cubic_11_1.100_R.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }
    
    return 0;
}