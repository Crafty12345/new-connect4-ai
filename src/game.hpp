#ifndef GAME_H
#define GAME_H

#define AI_PLAYER PlayerType::PLR_RED

#include "board.hpp"
#include "pointer.hpp"
#include "enums.hpp"
#include "decision_tree.hpp"

class Game {
    private:
        Board* m_board;
        Pointer* m_pointer;
        const int m_screenWidth = 1600;
        const int m_screenHeight = 900;
        size_t m_yellowScore = 0;
        size_t m_redScore = 0;
        enum GameState m_currentState = GameState::GAME_RUNNING; 
        PlayerType m_currentWinner;
        uint16_t m_nextAIAction;
        const PlayerType m_aiPlayer = PlayerType::PLR_RED;
        void playTurn();
        void reset();
        void pollEvents();
        void drawObjects();
        void drawScores();
        void drawFinishText();
    public:
        Game();
        ~Game();
        int run();
};

#endif