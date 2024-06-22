#include "sfml.hpp"

int SFML::init() {
    window.create(sf::VideoMode(800, 800), "Minesweeper");
    window.setFramerateLimit(60);

    if (!window.isOpen()) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }
    if (!font.loadFromFile("../font/Cubic_11_1.100_R.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }
    if (!texture.loadFromFile("../image/spritesheet.png")) {
        std::cerr << "Failed to load texture" << std::endl;
        return -1;
    }
    sprite.setTexture(texture);

    return 0;
}

std::pair<game_action, int> SFML::mouse_input() {
    sf::Event event;
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
            }
        }
    }

    return {NONE, -1};
}

block SFML::get_block(double x, double y) {
    return blocks[0];
}