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

int SFML::init_block() { 
    int pos = std::max(window.getSize().x / row, window.getSize().y / col);
    for(block& block : blocks){
        block.gl_x = block.index % col * pos;
        block.gl_y = block.index / col * pos;
    }

    return 0; 
}

int SFML::draw_board() {
    for (int i = 0; i < row * col; i++) {
        if (blocks[i].state == REVEALED) {
            if (blocks[i].value != MINE || blocks[i].value != EMPTY) {
                int sprite_pos = (blocks[i].value - 1) * 17;
                sprite.setTextureRect(sf::IntRect(sprite_pos, 68, 16, 16));
            } else if(blocks[i].value == MINE) {
                sprite.setTextureRect(sf::IntRect(85, 51, 16, 16));
            } else {
                sprite.setTextureRect(sf::IntRect(17, 51, 16, 16));
            }
        } else if (blocks[i].state == FLAGGED) {
            sprite.setTextureRect(sf::IntRect(34, 51, 16, 16));
        } else {
            sprite.setTextureRect(sf::IntRect(0, 51, 16, 16));
        }
        sprite.setPosition(blocks[i].gl_x, blocks[i].gl_y);
        window.draw(sprite);
    }

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
            block target_block = get_block(mouse_pos.x, mouse_pos.y);
            if (event.mouseButton.button == sf::Mouse::Left) {
                reveal(blocks[target_block.index]);
                return {REVEAL, target_block.index};
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                if(blocks[target_block.index].state == HIDDEN) {
                    flagged(blocks[target_block.index]);
                    return {FLAG, target_block.index};
                } else if(blocks[target_block.index].state == FLAGGED) {
                    remove_flagged(blocks[target_block.index]);
                    return {REMOVE_FLAG, target_block.index};
                }
            }
        }
    }

    return {NONE, -1};
}

block SFML::get_block(double x, double y) {
    return blocks[0];
}

int SFML::play_single() {
    while (window.isOpen() && status == PLAYING) {
        window.clear(sf::Color::White);
        draw_board();

        mouse_input();
        window.display();
        check_win();
    }

    return 0;
}