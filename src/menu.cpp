#include "menu.hpp"
#include "sfml.hpp"

Menu::Menu() {
    window.create(sf::VideoMode(260, 260), "Minesweeper");
    if (!font.loadFromFile("../font/Cubic_11_1.100_R.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    if (!difficulty_texture.loadFromFile("../image/difficulty.png")) {
        std::cerr << "Failed to load difficulty.png" << std::endl;
    }

    title.setFont(font);
    title.setString("Minesweeper!");
    title.setCharacterSize(24);
    title.setPosition(60, 20);
    title.setFillColor(sf::Color::White);

    difficulty_sprite.setTexture(difficulty_texture);
    difficulty_sprite.setScale(2, 2);
}

int Menu::draw_difficulty() {
    for (int i = 0; i < 3; i++) {
        difficulty_sprite.setTextureRect(sf::IntRect(0, i * 54, 130, 26));
        difficulty_sprite.setPosition(0, 80 + i * 60);
        window.draw(difficulty_sprite);
    }

    int input = get_input();
    if (input != -1) {
        difficulty_sprite.setTextureRect(
            sf::IntRect(0, 27 + input * 54, 130, 26));
        difficulty_sprite.setPosition(0, 80 + input * 60);
        window.draw(difficulty_sprite);
        window.display();

        while (window.waitEvent(event)) {
            if (event.type == sf::Event::MouseButtonReleased) {
                difficulty_sprite.setTextureRect(
                    sf::IntRect(0, input * 54, 130, 26));
                difficulty_sprite.setPosition(0, 80 + input * 60);
                window.draw(difficulty_sprite);
                window.display();
                SFML game(difficulty[input][0], difficulty[input][1],
                          difficulty[input][2]);
                game.play_single();
                return 0;
            }
        }
    }

    return 0;
}

int Menu::get_input() {
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (mouse.y >= 80 && mouse.y <= 132) {
            return 0;
        } else if (mouse.y >= 140 && mouse.y <= 192) {
            return 1;
        } else if (mouse.y >= 200 && mouse.y <= 252) {
            return 2;
        }
    }
    return -1;
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