#ifndef SFML_HPP
#define SFML_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>

#include "board.h"
#include "config.h"

class SFML : public Board {
public:
    enum game_action { REVEAL, FLAG, NONE };

    SFML(int row, int col, int mines);
    SFML();
    int play_single();
    
protected:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Sprite face;
    sf::Sprite number;

    int init_block();
    std::pair<SFML::game_action, int> mouse_input();
    int get_block();
    int draw_board();
    int draw_flag();
    int draw_time();
    int end_game();
};
#endif