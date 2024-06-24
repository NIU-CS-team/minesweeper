#ifndef SFML_HPP
#define SFML_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "board.h"
#include "config.h"

enum game_action { REVEAL, FLAG, NONE };

class SFML : public Board {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture texture;
    sf::Sprite sprite;

    int init_block();
    std::pair<game_action, int> mouse_input();
    block get_block();
    int draw_board();
    int end_game();

public:
    SFML(int row, int col, int mines) : Board(row, col, mines) {};
    int init();
    int play_single();
};
#endif