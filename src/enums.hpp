#ifndef ENUMS_H
#define ENUMS_H

enum class PlayerType: uint8_t {
    PLR_NONE,
    PLR_YELLOW,
    PLR_RED
};

enum class GameState: uint8_t {
    GAME_FINISHED,
    GAME_RUNNING
};

#endif