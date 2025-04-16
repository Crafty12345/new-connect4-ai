#include <iostream>

#include "board.hpp"
#define DEBUG

Board::Board(Vector2 pSize, Vector2 pPosition, uint16_t pRows, uint16_t pCols) {
    m_position = pPosition;
    m_size = pSize;
    m_numRows = pRows;
    m_numCols = pCols;
    m_numSlots = m_numCols * m_numRows;
    m_slots = (Slot**) malloc(sizeof(Slot*) * m_numSlots);
    for (int i = 0; i < m_numSlots; i++) {
        m_slots[i] = new Slot();
    }
}

Board::~Board() {
    for (int i = 0; i < m_numSlots; i++) {
        delete(m_slots[i]);
        m_slots[i] = NULL;
    }
    free(m_slots);
    m_slots = NULL;
}

Vector2 Board::getPosition()    { return m_position; }
uint16_t Board::getSlotCount()  { return m_numSlots; }
Vector2i Board::getDimensions() { return {m_numCols, m_numSlots};}
Slot** Board::getSlots()        { return m_slots; }

Vector2i Board::indexToVec(uint16_t pIndex) {
    int row = pIndex / m_numCols;
    int column = pIndex % m_numCols;
    return Vector2i{column, row};
}

int Board::getAvailableSlot(uint16_t pColumn) {
    for (int i = m_numSlots-1; i >= 0; i--) {
        Vector2i currentCoord = indexToVec(i);
        if(currentCoord.x == pColumn) {
            if(m_slots[i]->getType() == PlayerType::PLR_NONE) {
                return i;
            }
        }
    }
    return -1;
}

int getAvailableSlotStatic(Board* pBoard, Slot** pSlots, uint16_t pColumn) {
    uint16_t slotCount = pBoard -> getSlotCount();
    for (int i = slotCount-1; i >= 0; i--) {
        Vector2i currentCoord = pBoard->indexToVec(i);
        if(currentCoord.x == pColumn) {
            if(pSlots[i]->getType() == PlayerType::PLR_NONE) {
                return i;
            }
        }
    }
    return -1;
};

bool Board::isFull() {
    uint16_t numFilled = 0;
    for (int i = 0; i < m_numSlots; i++) {
        if(m_slots[i]->getType() != PlayerType::PLR_NONE) {
            numFilled++;
        }
    }
    return numFilled >= m_numSlots;
}

WinType Board::checkVerticalWin() {
    int maxIters = m_numSlots - m_numCols * 3;
    InRowCounter counter;
    WinType winner;
    for (int i = 0; i < maxIters; i++) {
        counter.red = 0;
        counter.yellow = 0;
        for (int j = 0; j < 4; j++) {
            int offset = j * m_numCols;
            if(m_slots[i + offset]->getType()==PlayerType::PLR_YELLOW) {
                counter.yellow++;
            }
            else {
                counter.yellow = 0;
            }
            if (counter.yellow >= 4) {
                winner.numInRow = counter.yellow;
                winner.type = PlayerType::PLR_YELLOW;
            }

            if(m_slots[i + offset]->getType()==PlayerType::PLR_RED) {
                counter.red++;
            }
            else {
                counter.red = 0;
            }
            if (counter.red >= 4) {
                winner.numInRow = counter.red;
                winner.type = PlayerType::PLR_RED;
            }
        }
    }
    return winner;
}

WinType Board::checkHorizontalWin() {
    InRowCounter counter;
    WinType winner;
    Vector2i currentPosition;
    Vector2i lastPosition{0,0};
    for (int i = 0; i < m_numSlots; i++) {
        currentPosition = indexToVec(i);
        if(currentPosition.x == 0) {
            counter.red = 0;
            counter.yellow = 0;
        }
        if((currentPosition.y == lastPosition.y) && (currentPosition.x == lastPosition.x+1)) {
            //std::cout << std::to_string(i) << "," << vecToStr(&currentPosition) << std::endl;
            if(m_slots[i]->getType() == PlayerType::PLR_YELLOW) {
                counter.yellow++;
                counter.red = 0;
            }
            if(m_slots[i]->getType() == PlayerType::PLR_NONE) {
                counter.yellow = 0;
                counter.red = 0;
            }
            if(m_slots[i] -> getType() == PlayerType::PLR_RED) {
                counter.red++;
                counter.yellow = 0;
            }
        } else if (currentPosition.x == 0) {
            counter.yellow = 0;
            counter.red = 0;
            if(m_slots[i]->getType() == PlayerType::PLR_YELLOW) {
                counter.yellow++;
            }
            if(m_slots[i] -> getType() == PlayerType::PLR_RED) {
                counter.red++;
            }
        }
        if((counter.yellow >= 4) || (counter.red >= 4)) {
            if(counter.yellow >= 4) {
                std::cout << "Yellow wins" << std::endl;
                winner.numInRow = counter.yellow;
                winner.type = PlayerType::PLR_YELLOW;
            }
            else if (counter.red >= 4) {
                //std::cout << "Red wins" << std::endl;
                winner.numInRow = counter.red;
                winner.type = PlayerType::PLR_RED;
            }
            return winner;
        }
        lastPosition = currentPosition;
    }
    return winner;
}

WinType Board::checkDownRightWin() {
    // current slot + numColumns + 1
    InRowCounter counter;
    WinType winner;
    int maxIters = m_numSlots - 3 * m_numCols;
    Vector2i currentPosition;
    for (int i = 0; i < maxIters; i++) {
        currentPosition = indexToVec(i);
        counter.red = 0;
        counter.yellow = 0;
        if(currentPosition.x < m_numCols - 3) {
            for (int j = 0; j < 4; j++) {
                int offset = j * (m_numCols + 1);
                if(m_slots[i + offset]->getType() == PlayerType::PLR_RED) {
                    counter.red++;
                    counter.yellow = 0;
                }
                if(m_slots[i + offset]->getType() == PlayerType::PLR_YELLOW) {
                    counter.yellow++;
                    counter.red = 0;
                }
                if(m_slots[i + offset]->getType() == PlayerType::PLR_NONE) {
                    counter.red = 0;
                    counter.yellow = 0;
                }
            }
        }
        if((counter.red >= 4) || (counter.yellow >= 4)) {
            if(counter.red >= 4) {
                winner.type = PlayerType::PLR_RED;
                winner.numInRow = counter.red;
                //std::cout << "Red wins!" << std::endl;
            }
            else if(counter.yellow >= 4) {
                winner.type = PlayerType::PLR_YELLOW;
                winner.numInRow = counter.yellow;
                //std::cout << "Yellow wins!" << std::endl;
            }
            return winner;
        }
    }
    return winner;
}

WinType Board::checkDownLeftWin() {
    // current slot + numColumns + 1
    InRowCounter counter;
    WinType winner;
    int maxIters = m_numSlots - 3 * m_numCols;
    Vector2i currentPosition;
    for (int i = 0; i < maxIters; i++) {
        currentPosition = indexToVec(i);
        counter.red = 0;
        counter.yellow = 0;
        if(currentPosition.x > 3) {
            for (int j = 0; j < 4; j++) {
                int offset = j * (m_numCols - 1);
                if(m_slots[i + offset]->getType() == PlayerType::PLR_RED) {
                    counter.red++;
                    counter.yellow = 0;
                }
                if(m_slots[i + offset]->getType() == PlayerType::PLR_YELLOW) {
                    counter.yellow++;
                    counter.red = 0;
                }
                if(m_slots[i + offset]->getType() == PlayerType::PLR_NONE) {
                    counter.red = 0;
                    counter.yellow = 0;
                }
            }
        }
        if((counter.red >= 4) || (counter.yellow >= 4)) {
            if(counter.red >= 4) {
                winner.type = PlayerType::PLR_RED;
                winner.numInRow = counter.red;
                std::cout << "Red wins!" << std::endl;
            }
            else if(counter.yellow >= 4) {
                winner.type = PlayerType::PLR_YELLOW;
                winner.numInRow = counter.yellow;
                std::cout << "Yellow wins!" << std::endl;
            }
            return winner;
        }
    }
    return winner;
}

void Board::copyWin(WinType* pSrc, WinType* pDest) {
    // Add to numInRow if winner is the same
    //std::cout << winToStr(pSrc) << std::endl;
    if((pSrc->type == pDest->type) && (pDest->type != PlayerType::PLR_NONE)) {
        pDest->numInRow += pSrc->numInRow;
    }
    // Copy if a winner exists and a winner has not been declared yet
    if((pDest->type == PlayerType::PLR_NONE) && (pSrc->type != PlayerType::PLR_NONE)) {
        *pDest = *pSrc;
    }
}

WinType Board::checkWin() {
    WinType winner;
    WinType vertWin = checkVerticalWin();
    WinType horiWin = checkHorizontalWin();
    WinType downRightWin = checkDownRightWin();
    WinType downLeftWin = checkDownLeftWin();

    copyWin(&vertWin, &winner);
    copyWin(&horiWin, &winner);
    copyWin(&downRightWin, &winner);
    copyWin(&downLeftWin, &winner);
    //std::cout << winToStr(&winner) << std::endl;
    return winner;
}

void Board::draw() {
    DrawRectangleV(m_position, m_size, m_colour);

    float slotRadius = m_slots[0]->getRadius();

    const float paddingX = 2.0f;
    const float paddingY = 2.0f;

    for (int i = 0; i < m_numSlots; i++) {

        Vector2i tempVec = indexToVec(i);
        int currentCol = tempVec.x;
        int currentRow = tempVec.y;

        int offsetX = (slotRadius * 2 * currentCol * paddingX) + (slotRadius*2);
        int offsetY = (slotRadius * 2 * currentRow * paddingY) + (slotRadius*2);
        Vector2 position = Vector2{m_position.x + offsetX, m_position.y + offsetY};
        #ifdef DEBUG
            m_slots[i] -> drawDebug(&position, &tempVec);
        #else
            m_slots[i] -> draw(&position);
        #endif
    }
}

void Board::setSlot(int pSlotIndex, PlayerType pNewType) {
    m_slots[pSlotIndex]->setType(pNewType);
}

void Board::setPosition(int pX, int pY) {
    m_position.x = pX;
    m_position.y = pY;
}

void Board::centerX(int pScreenWidth) {
    int border = pScreenWidth - m_size.x;
    m_position.x = border / 2;
}

void Board::centerY(int pScreenHeight) {
    int border = pScreenHeight - m_size.y;
    m_position.y = border / 2;
}

void Board::reset() {
    for (int i = 0; i < m_numSlots; i++) {
        m_slots[i]->setType(PlayerType::PLR_NONE);
    }
}