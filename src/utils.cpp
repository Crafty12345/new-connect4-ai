#include "utils.hpp"

std::string winToStr(WinType* pWinType) {
    std::string str;
    str.append("Winner: ");
    str.append(plrToStr(pWinType->type));
    str.append(", ");
    str.append(std::to_string(pWinType->numInRow));
    return str;
};

std::string plrToStr(PlayerType pPlayer) {
    std::string str;
    switch (pPlayer) {
        case PlayerType::PLR_YELLOW:
            str.append("Yellow");
            break;
        case PlayerType::PLR_RED:
            str.append("Red");
            break;
        case PlayerType::PLR_NONE:
            str.append("None");
            break;
    }
    return str;
}

std::string colourToStr(Color* pColour) {
    std::string str = "RGBA(";
    str.append(std::to_string(pColour->r));
    str.append(", ");
    str.append(std::to_string(pColour->g));
    str.append(", ");
    str.append(std::to_string(pColour->b));
    str.append(", ");
    str.append(std::to_string(pColour->a));
    str.append(")");
    return str;
}

std::string vecToStr(Vector2i* pVec) {
    std::string str = "(";
    str.append(std::to_string(pVec->x));
    str.append(",");
    str.append(std::to_string(pVec->y));
    str.append(")");
    return str;
}