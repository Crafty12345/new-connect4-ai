#ifndef TREE_H
#define TREE_H

#define MAX_RECURSION_DEPTH 3
#define DECAY_AMT 5
#define BLOCK_AMT 800
#define WIN_AMT 1000
#define LOSE_MULTIPLIER 2
#define PER_PIECE_AMT 1

#include <stdint.h>
#include <vector>

#include "enums.hpp"
#include "slot.hpp"

struct TypePtrPair {
    Slot* slot;
    PlayerType slotType;
};

class DecisionTree {
    public:
        DecisionTree(Slot** pSlots, Board* pBoard, PlayerType pPlayerType);
        ~DecisionTree();
        double getScore();
        void simulateTurn(uint16_t pColumn, PlayerType pPlayer, std::vector<TypePtrPair>* pModifiedSlots, int numRecursions);
        int getBestAction();
    private:
        PlayerType m_player;
        double m_score;
        uint16_t m_action;
        Slot** m_state = nullptr;
        std::vector<DecisionTree*> m_branches;
        Board* m_board;
        void manageTurnSim(uint16_t pColumn, PlayerType pPlayer);
        double evaluateState();
        double calculateVerticalScore();
        double calculateHorizontalScore();
        double calculateDownLeftScore();
        double calculateDownRightScore();
};

#endif