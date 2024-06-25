#include <cassert>
#include <iostream>

#include "sfml.hpp"
#include "config.h"
#include "network.h"
#include "menu.hpp"

int main(int argc, char* argv[]) {
    Menu menu;
    menu.run();
    return 0;
}
