#include "slot.hpp"
#include "utils.hpp"

void st_toggle(PlayerType* pSlotType) {
    switch (*pSlotType)
    {
    case PlayerType::PLR_YELLOW:
        *pSlotType = PlayerType::PLR_RED;
        break;
    case PlayerType::PLR_RED:
        *pSlotType = PlayerType::PLR_YELLOW;    
        break;
    case PlayerType::PLR_NONE:
        std::cout << "Warning: Attempting to toggle state of unfilled slot";
        break;
    default:
        break;
    }
}

Color st_toColour(PlayerType* pSlotType) {
    Color slotColour = DARKGRAY;
    switch (*pSlotType)
    {
    case PlayerType::PLR_RED:
        slotColour = RED;
        break;
    case PlayerType::PLR_YELLOW:
        slotColour = {184, 178, 24, 255};
        break;
    default:
        break;
    }
    return slotColour;
}

Color st_toAltColour(PlayerType* pSlotType) {
    Color slotColour = RAYWHITE;
    switch (*pSlotType)
    {
    case PlayerType::PLR_RED:
        slotColour = MAROON;
        break;
    case PlayerType::PLR_YELLOW:
        slotColour = ORANGE;
        break;
    default:
        break;
    }
    return slotColour;
}

Slot::Slot() {
    m_type = PlayerType::PLR_NONE;
}
Slot::Slot(Slot* pSrc) {
    m_type = pSrc->getType();
}

PlayerType Slot::getType() { return m_type;   }
float Slot::getRadius()  { return m_radius; }

void Slot::draw(Vector2* pPosition) {
    Color slotColour = st_toColour(&m_type);
    DrawCircleV(*pPosition, m_radius, slotColour);
}

void Slot::drawDebug(Vector2* pPosition, Vector2i* pSlotPosition) {
    draw(pPosition);
    std::string dbgStr = vecToStr(pSlotPosition);
    Color debugTextColour = st_toAltColour(&m_type);
    
    DrawText(dbgStr.c_str(), pPosition->x-m_radius, pPosition->y-m_radius,40, debugTextColour);
}

void Slot::setType(PlayerType pNewType) {m_type = pNewType;}