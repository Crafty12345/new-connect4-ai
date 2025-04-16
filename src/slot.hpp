#ifndef SLOT_H
#define SLOT_H

#include <iostream>

#include "raylib.h"

#include "utils.hpp"
#include "enums.hpp"


void st_toggle(PlayerType* pSlotType);
Color st_toColour(PlayerType* pSlotType);
Color st_toAltColour(PlayerType* pSlotType);

class Slot {
    public:
        Slot();
        Slot(Slot* pSrc);
        PlayerType getType();
        float getRadius();
        void setType(PlayerType pNewType);
        void draw(Vector2* pPosition);
        void drawDebug(Vector2* pPosition, Vector2i* pSlotPosition);
    private:
        PlayerType m_type;
        const float m_radius = 25;
};

#endif