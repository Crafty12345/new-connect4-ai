#include "utils.hpp"
#include "pointer.hpp"

Pointer::Pointer(uint16_t pNumColumns, Vector2 pBasePosition) {
    m_currentPlayer = PlayerType::PLR_YELLOW;
    m_currentColour = st_toColour(&m_currentPlayer);
    m_numColumns = pNumColumns;
    m_basePosition = pBasePosition;
};

uint16_t Pointer::getCurrentColumn() { return m_selectedColumn; }

PlayerType Pointer::getCurrentPlayer() { return m_currentPlayer; }

void Pointer::draw() {
    //std::cout << colourToStr(&m_currentColour) << std::endl;
    const float triangleSize = 100;
    Vector2 point1 = {(m_basePosition.x + (triangleSize * m_selectedColumn)),
                        m_basePosition.y};

    Vector2 point2 = {m_basePosition.x + (triangleSize * (m_selectedColumn + 0.5)),
                        m_basePosition.y + 100};

    Vector2 point3 = {m_basePosition.x + (triangleSize * (m_selectedColumn + 1)),
                     m_basePosition.y};
    DrawTriangle(point1, point2, point3, m_currentColour);
}

void Pointer::setCurrentColumn(uint16_t pNewColumn) {
    if(pNewColumn < m_numColumns) {
        m_selectedColumn = pNewColumn;
    }
}
void Pointer::incCurrentColumn() {
    if(m_selectedColumn + 1 < m_numColumns) {
        m_selectedColumn++;
    }
}
void Pointer::decCurrentColumn() {
    if(m_selectedColumn > 0) {
        m_selectedColumn--;
    }
}

void Pointer::switchTurn() {
    st_toggle(&m_currentPlayer);
    m_currentColour = st_toColour(&m_currentPlayer);
}