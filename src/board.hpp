#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "raylib.h"
#include "slot.hpp"
#include "utils.hpp"

struct InRowCounter {
    uint16_t yellow = 0;
    uint16_t red = 0;
};

class Board {
    private:
        const Color m_colour = SKYBLUE;
        Vector2 m_size;
        Vector2 m_position;
        uint16_t m_numCols;
        uint16_t m_numRows;
        uint16_t m_numSlots;
        WinType checkHorizontalWin();
        WinType checkVerticalWin();
        WinType checkDownRightWin();
        WinType checkDownLeftWin();
        void copyWin(WinType* pOutWinner, WinType* pWinCheck);
        
    public:
        Slot** m_slots;
        Board(Vector2 pSize, Vector2 pPosition, uint16_t pRows, uint16_t pCols);
        ~Board();
        Vector2 getPosition();
        uint16_t getSlotCount();
        Vector2i getDimensions();
        Slot** getSlots();
        int getAvailableSlot(uint16_t pColumn);
        bool isFull();
        Vector2i indexToVec(uint16_t pIndex);
        WinType checkWin();
        void draw();
        void setSlot(int pSlotIndex, PlayerType pNewType);
        void setPosition(int pX, int pY);
        void centerX(int pScreenWidth);
        void centerY(int pScreenHeight);
        void reset();
};

int getAvailableSlotStatic(Board* pBoard, Slot** pSlots, uint16_t pColumn);

#endif