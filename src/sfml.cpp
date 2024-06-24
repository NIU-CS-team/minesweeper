#include "sfml.hpp"

int SFML::init() {
    int window_scale = 16 * blockScale;
    window.create(sf::VideoMode(row * window_scale, col * window_scale + 60), "Minesweeper");
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
    face.setTexture(texture);
    face.setScale(blockScale, blockScale);
    face.setPosition(window.getSize().x / 2 - 26, 4);
    face.setTextureRect(sf::IntRect(0, 24, 26, 26));

    return 0;
}

int SFML::init_block() {
    int pos = std::min(window.getSize().x / row, window.getSize().y / col);
    sprite.setScale(blockScale, blockScale);
    for (block& block : blocks) {
        block.gl_x = block.index % row * pos;
        block.gl_y = block.index / row * pos + 60;
    }

    return 0;
}

int SFML::draw_board() {
    for (int i = 0; i < row * col; i++) {
        if (blocks[i].state == block::REVEALED) {
            if (blocks[i].value >= block::MINE) {
                if (get_block() == i && status == LOST) {
                    sprite.setTextureRect(sf::IntRect(102, 51, 16, 16));
                } else {
                    sprite.setTextureRect(sf::IntRect(85, 51, 16, 16));
                }
            } else if (blocks[i].value != block::EMPTY) {
                int sprite_pos = (blocks[i].value - 1) * 17;
                sprite.setTextureRect(sf::IntRect(sprite_pos, 68, 16, 16));
            } else {
                sprite.setTextureRect(sf::IntRect(17, 51, 16, 16));
            }
        } else if (blocks[i].state == block::FLAGGED) {
            if (status == LOST && blocks[i].value != block::MINE) {
                sprite.setTextureRect(sf::IntRect(119, 51, 16, 16));
            } else {
                sprite.setTextureRect(sf::IntRect(34, 51, 16, 16));
            }
        } else {
            sprite.setTextureRect(sf::IntRect(0, 51, 16, 16));
        }
        sprite.setPosition(blocks[i].gl_x, blocks[i].gl_y);
        window.draw(sprite);
        window.draw(face);
    }

    return 0;
}

std::pair<SFML::game_action, int> SFML::mouse_input() {
    sf::Event event;
    auto action = std::make_pair(SFML::NONE, -1);

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            int index = get_block();
            if (index < 0 || index >= row * col) {
                break;
            }
            if (event.mouseButton.button == sf::Mouse::Left) {
                face.setTextureRect(sf::IntRect(54, 24, 26, 26));
                window.draw(face);
                reveal(blocks[index]);
                action = {SFML::REVEAL, index};
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                if (blocks[index].state == block::HIDDEN ||
                    blocks[index].state == block::FLAGGED) {
                    flip_flag(blocks[index]);
                    action = {SFML::FLAG, index};
                }
            }
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            face.setTextureRect(sf::IntRect(0, 24, 26, 26));
            window.draw(face);
        }
    }

    return action;
}

int SFML::get_block() {
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    if (pos.y < 60) {
        if (pos.x > window.getSize().x / 2 - 26 &&
            pos.x < window.getSize().x / 2 + 26) {
            face.setTextureRect(sf::IntRect(27, 24, 26, 26));
            window.draw(face);

            clear();
            play_single();
        }
        return -1;
    }

    int x = pos.x / (window.getSize().x / row);
    int y = (pos.y - 60) / (window.getSize().x / row);

    return x + y * row;
}

int SFML::play_single() {
    init_block();
    generate_mines();
    while (window.isOpen() && status == PLAYING) {
        window.clear(sf::Color::White);
        draw_board();

        mouse_input();
        window.display();
        check_win();
    }
    end_game();

    return 0;
}

int SFML::end_game() {
    end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    std::cout << "End time: (" << duration.count() / 1000000 << "."
              << duration.count() % 1000000 << "s)\n";

    if (status == WON) {
        face.setTextureRect(sf::IntRect(81, 24, 26, 26));
    } else {
        face.setTextureRect(sf::IntRect(108, 24, 26, 26));
        for (int i = 0; i < row * col; i++) {
            if (blocks[i].value >= block::MINE) {
                blocks[i].state = block::REVEALED;
            }
        }
    }

    draw_board();
    window.display();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            get_block();
            window.close();
        }
    }
    return 0;
}