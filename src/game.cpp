#include <iostream>

#include "raylib.h"
#include "game.hpp"
#include "pointer.hpp"
#include "slot.hpp"

Game::Game() {
    m_board = new Board(Vector2{700,600}, Vector2{0,0}, 6, 7);
    m_board -> centerX(m_screenWidth);
    m_board -> centerY(m_screenHeight);
    Vector2 pointerPosition = m_board->getPosition();
    pointerPosition.y -= 100;
    m_pointer = new Pointer(7, pointerPosition);
    //std::cout << "sizeof(Slot) = " << sizeof(Slot) << std::endl;
    std::cout << "sizeof(Board) = " << sizeof(Board) << std::endl;
}

Game::~Game() {
    delete(m_board);
    m_board = NULL;
    delete(m_pointer);
    m_pointer = NULL;
}

int Game::run() {

    InitWindow(m_screenWidth, m_screenHeight, "Connect Four AI");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        pollEvents();
        drawObjects();
    }
    CloseWindow();
    return EXIT_SUCCESS;
}

void Game::reset(){
    m_currentWinner = PlayerType::PLR_NONE;
    if(m_pointer->getCurrentPlayer() == PlayerType::PLR_RED) {
        m_pointer -> switchTurn();
    }
    m_board->reset();
    m_currentState = GameState::GAME_RUNNING;
}

void Game::drawScores() {
    Vector2 scorePosition{100, 0};
    DrawText(std::to_string(m_yellowScore).c_str(), scorePosition.x, scorePosition.y, 40, YELLOW);
    DrawText(std::to_string(m_redScore).c_str(), scorePosition.x, scorePosition.y+40, 40, RED);
}

void Game::drawFinishText() {
    if(m_currentState == GameState::GAME_FINISHED) {
        Color textColour = st_toAltColour(&m_currentWinner);
        Vector2 textOrigin = {m_screenWidth / 2, m_screenHeight / 2};
        //std::cout << textOrigin.x << ", " << textOrigin.y << std::endl;
        std::string winnerStr;
        if(m_currentWinner != PlayerType::PLR_NONE) {
            if(m_currentWinner == PlayerType::PLR_YELLOW) {
                winnerStr = "Yellow";
            }
            if (m_currentWinner == PlayerType::PLR_RED) {
                winnerStr = "Red";
            }
            winnerStr.append(" wins!");
        } else if (m_currentWinner == PlayerType::PLR_NONE) {
            textColour = DARKGRAY;
            winnerStr = "It's a tie!";
        }
        //std::cout << winnerStr.c_str() << std::endl;
        textOrigin.x -= 150;
        textOrigin.y -= 50;
        DrawText(winnerStr.c_str(), textOrigin.x, textOrigin.y, 50, textColour);
        //std::cout << "[DEBUG] drawing" << std::endl;
    }
}

void Game::drawObjects() {
    BeginDrawing();
        ClearBackground(DARKGRAY);
        drawScores();
        m_board -> draw();
        m_pointer -> draw();
        drawFinishText();
    EndDrawing();
}

void Game::playTurn() {
    PlayerType currentTurn = m_pointer -> getCurrentPlayer();
    uint16_t selectedColumn = m_pointer -> getCurrentColumn();

    int foundSlot = m_board->getAvailableSlot(selectedColumn);
    if(foundSlot >= 0) {
        m_board->setSlot(foundSlot, m_pointer -> getCurrentPlayer());
        m_pointer->switchTurn();
    }
    if(m_pointer->getCurrentPlayer() == AI_PLAYER) {
        DecisionTree* tree = new DecisionTree(m_board->getSlots(), m_board, AI_PLAYER);
        int action = tree->getBestAction();
        if(action > -1) {
            int aiSlot = m_board->getAvailableSlot(action);
            if(aiSlot > -1) {
                m_board->setSlot(aiSlot, AI_PLAYER);
                m_pointer->switchTurn();
            }
        }
        delete(tree);
    }

    WinType winResult = m_board->checkWin();
    //std::cout << "[DEBUG] " << winToStr(&winResult) << std::endl;
    if(winResult.type == PlayerType::PLR_YELLOW) {
        m_yellowScore++;
        m_currentState = GameState::GAME_FINISHED;
        m_currentWinner = PlayerType::PLR_YELLOW;
    }
    if (winResult.type == PlayerType::PLR_RED) {
        m_redScore++;
        m_currentState = GameState::GAME_FINISHED;
        m_currentWinner = PlayerType::PLR_RED;
    }
    if(m_board->isFull()) {
        m_currentState = GameState::GAME_FINISHED;
        m_currentWinner = PlayerType::PLR_NONE;
    }
}

void Game::pollEvents() {
    if(m_currentState == GameState::GAME_RUNNING) {
        if(IsKeyPressed(KEY_RIGHT)){
            m_pointer->incCurrentColumn();
        }
        else if (IsKeyPressed(KEY_LEFT)) {
            m_pointer -> decCurrentColumn();
        }
        if (IsKeyPressed(KEY_ENTER)) {
            playTurn();
        }
    }
    else if (m_currentState == GameState::GAME_FINISHED) {
        if (IsKeyPressed(KEY_ENTER)) {
            reset();
        }
    }
}