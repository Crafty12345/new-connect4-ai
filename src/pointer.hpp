#ifndef POINTER_H
#define POINTER_H

#include <stdint.h>
#include "raylib.h"
#include "slot.hpp"

class Pointer {
    public:
        Pointer(uint16_t pNumColumns, Vector2 pBasePosition);
        uint16_t getCurrentColumn();
        PlayerType getCurrentPlayer();
        void draw();
        void setCurrentColumn(uint16_t pNewColumn);
        void incCurrentColumn();
        void decCurrentColumn();
        void switchTurn();
    private:
        uint16_t m_numColumns;
        Vector2 m_basePosition;
        uint16_t m_selectedColumn = 0;
        enum PlayerType m_currentPlayer;
        Color m_currentColour;
};

#endif