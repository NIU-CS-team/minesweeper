#include "menu.hpp"

Menu::Menu() {
    window.create(sf::VideoMode(260, 260), "Minesweeper");
    if (!font.loadFromFile("../font/Cubic_11_1.100_R.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    if (!difficulty_texture.loadFromFile("../image/difficulty.png")) {
        std::cerr << "Failed to load difficulty.png" << std::endl;
    }
    difficulty_sprite.setTexture(difficulty_texture);
    difficulty_sprite.setScale(2, 2);
}

int Menu::draw_difficulty() {
    for (int i = 0; i < 3; i++) {
        difficulty_sprite.setTextureRect(sf::IntRect(0, i * 54, 130, 26));
        difficulty_sprite.setPosition(0, 80 + i * 60);
        window.draw(difficulty_sprite);
    }
    return 0;
}

int Menu::run() {
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        draw_difficulty();

        window.display();
    }
    return 0;
}