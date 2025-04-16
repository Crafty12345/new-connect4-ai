#include <iostream>
#include "game.hpp"

int main(void) {
    Game* game;
    game = new Game();
    game->run();
    delete(game);
    return EXIT_SUCCESS;
}