#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "raylib.h"
#include "enums.hpp"

struct Vector2i {
    int x;
    int y;
};

struct WinType {
    uint16_t numInRow = 0;
    enum PlayerType type = PlayerType::PLR_NONE;
};

std::string winToStr(WinType* pWinType);
std::string plrToStr(PlayerType pPlayer);

std::string colourToStr(Color* pColour);
std::string vecToStr(Vector2i* pVec);

#endif