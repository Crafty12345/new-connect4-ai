#include <float.h>

#include "board.hpp"
#include "decision_tree.hpp"

DecisionTree::DecisionTree(Slot** pSlots, Board* pBoard, PlayerType pPlayerType) {
    m_board = pBoard;
    m_state = (Slot**) malloc(sizeof(Slot*) * m_board->getSlotCount());
    for (int i = 0; i < m_board->getSlotCount(); i++) {
        m_state[i] = pSlots[i];
    }
    m_player = pPlayerType;
    m_score = 0;
}

DecisionTree::~DecisionTree() {
    free(m_state);
    m_state = nullptr;
    // for (Slot* slot : m_modifiedSlots) {
    //     delete(slot);
    //     slot = nullptr;
    // }
}

double DecisionTree::getScore() {return m_score;}

void DecisionTree::manageTurnSim(uint16_t pColumn, PlayerType pPlayer) {
    std::vector<TypePtrPair> modifiedSlots;
    //printf("[DEBUG] test 1\n");
    simulateTurn(pColumn, pPlayer, &modifiedSlots, 0);
    //printf("[DEBUG] test5\n");
    for (const TypePtrPair& pair : modifiedSlots) {
        pair.slot->setType(pair.slotType);
    }
}

void DecisionTree::simulateTurn(uint16_t pColumn, PlayerType pPlayer, std::vector<TypePtrPair>* pModifiedSlots, int numRecursions) {
    int foundIndex = getAvailableSlotStatic(m_board, m_state, pColumn);
    PlayerType currentTurn = pPlayer;
    m_score -= DECAY_AMT;

    if(foundIndex >= 0) {
        Slot* selectedSlot = m_state[foundIndex];
        PlayerType oldPlayer = selectedSlot->getType();
        //std::cout << "[DEBUG] oldPlayer=" << plrToStr(oldPlayer) << std::endl;
        selectedSlot->setType(pPlayer);

        m_state[foundIndex] = selectedSlot;
        m_score = evaluateState();
        TypePtrPair temp;
        temp.slot = selectedSlot;
        temp.slotType = oldPlayer;
        pModifiedSlots->push_back(temp);

        Vector2i boardDims = m_board->getDimensions();
        DecisionTree* subtree = new DecisionTree(m_state, m_board, m_player);

        st_toggle(&currentTurn);
        //std::cout << "[DEBUG] num recursions: " << numRecursions << std::endl;
        if(numRecursions < MAX_RECURSION_DEPTH) {
            for (int i = 0; i < boardDims.x; i++) {
                subtree->simulateTurn(i, currentTurn, pModifiedSlots, numRecursions+1);
                m_score += subtree->getScore();
            }
        }
        delete(subtree);
    }
}

double DecisionTree::calculateVerticalScore() {
    double score = 0;
    uint16_t numSlots = m_board-> getSlotCount();
    uint16_t numCols = m_board -> getDimensions().x;
    int minIndex = numCols * 4;
    InRowCounter counter;
    if(m_state != nullptr) {
        for (int i = numSlots-1; i >= minIndex; i--) {
            counter.red = 0;
            counter.yellow = 0;
            for (int j = 0; j >= -4; j--) {
                int offset = j * numCols;
                PlayerType currentSlotType = m_state[i + offset]->getType();
                Vector2i position = m_board->indexToVec(i+offset);
                //std::cout << "[DEBUG] " << vecToStr(&position) << std::endl;
                if(currentSlotType==PlayerType::PLR_YELLOW) {
                    counter.yellow++;
                    //std::cout << vecToStr(&position) << std::endl;
                    //std::cout << "[DEBUG] counter.yellow=" << counter.yellow << std::endl;
                }
                else {
                    // Check if red blocks yellow from winning
                    if(counter.yellow == 3) {
                        if((currentSlotType == PlayerType::PLR_RED) && (m_player == PlayerType::PLR_RED)) {
                            score += BLOCK_AMT;
                        }
                        else if ((currentSlotType == PlayerType::PLR_RED) && (m_player == PlayerType::PLR_YELLOW)) {
                            score -= BLOCK_AMT;
                        }
                    }
                    counter.yellow = 0;
                }
                if(counter.yellow > 0) {
                    if(m_player == PlayerType::PLR_YELLOW) {
                        score += counter.yellow;
                    } else {
                        score -= counter.yellow * PER_PIECE_AMT;
                    }
                }

                if (counter.yellow >= 4) {
                    if(m_player == PlayerType::PLR_YELLOW) {
                        score += BLOCK_AMT;
                    }
                    else if (m_player == PlayerType::PLR_RED) {
                        score -= BLOCK_AMT * PER_PIECE_AMT;
                    }
                }
    
                if(currentSlotType==PlayerType::PLR_RED) {
                    counter.red++;
                }
                else {
                    // Check if red blocks yellow from winning
                    if(counter.red == 3) {
                        if((currentSlotType == PlayerType::PLR_YELLOW) && (m_player == PlayerType::PLR_YELLOW)) {
                            score += BLOCK_AMT;
                        }
                        else if ((currentSlotType == PlayerType::PLR_YELLOW) && (m_player == PlayerType::PLR_RED)) {
                            score -= BLOCK_AMT * PER_PIECE_AMT;
                        }
                    }
                    counter.red = 0;
                }
                if (counter.red > 0) {
                    if(m_player == PlayerType::PLR_RED) {
                        score += counter.red;
                    }
                    else {
                        score -= counter.red * PER_PIECE_AMT;
                    }
                }

                if (counter.red >= 4) {
                    if(m_player == PlayerType::PLR_RED) {
                        score += WIN_AMT;
                    }
                    else if (m_player == PlayerType::PLR_YELLOW) {
                        score -= WIN_AMT * LOSE_MULTIPLIER;
                    }
                }
                //std::cout << "[DEBUG] Yellow: " << counter.yellow << std::endl;
            }
        }
    } else {
        std::cout << "Error: board state is nullptr" << std::endl;
    }
    return score;
}

double DecisionTree::calculateHorizontalScore() {
    InRowCounter counter;
    double score = 0;
    Vector2i currentPosition;
    Vector2i lastPosition{0,0};
    uint16_t numSlots = m_board->getSlotCount();
    PlayerType currentSlotType;
    if(m_state != nullptr) {
        for (int i = 0; i < numSlots; i++) {
            currentPosition = m_board->indexToVec(i);
            currentSlotType = m_state[i]->getType();
            if(currentPosition.x == 0) {
                counter.red = 0;
                counter.yellow = 0;
            }
            if((currentPosition.y == lastPosition.y) && (currentPosition.x == lastPosition.x+1)) {
                //std::cout << std::to_string(i) << "," << vecToStr(&currentPosition) << std::endl;
                if(currentSlotType == PlayerType::PLR_YELLOW) {
                    counter.yellow++;
                    if(counter.red == 3) {
                        //Grant points for "blocking off" other player
                        if(m_player == PlayerType::PLR_YELLOW) {
                            score += BLOCK_AMT;
                        }
                    }
                    counter.red = 0;
                }
                if(currentSlotType == PlayerType::PLR_NONE) {
                    counter.yellow = 0;
                    counter.red = 0;
                }
                if(currentSlotType == PlayerType::PLR_RED) {
                    counter.red++;
                    if(counter.yellow == 3) {
                        //Grant points for "blocking off" other player
                        if(m_player == PlayerType::PLR_RED) {
                            score += BLOCK_AMT;
                        }
                    }
                    counter.yellow = 0;
                }
            } else if (currentPosition.x == 0) {
                counter.yellow = 0;
                counter.red = 0;
                if(currentSlotType == PlayerType::PLR_YELLOW) {
                    counter.yellow++;
                }
                if(currentSlotType == PlayerType::PLR_RED) {
                    counter.red++;
                }
            }

            if(m_player == PlayerType::PLR_YELLOW) {
                score += counter.yellow * PER_PIECE_AMT;
                score -= counter.red * PER_PIECE_AMT;
            }
            if(m_player == PlayerType::PLR_RED) {
                score += counter.red * PER_PIECE_AMT;
                score -= counter.yellow * PER_PIECE_AMT;
            }

            if((counter.yellow >= 4) || (counter.red >= 4)) {
                if(counter.yellow >= 4) {
                    if (m_player == PlayerType::PLR_YELLOW) {
                        score += WIN_AMT;
                    }
                    else {
                        score -= WIN_AMT * LOSE_MULTIPLIER;
                    }
                }
                else if (counter.red >= 4) {
                    if(m_player == PlayerType::PLR_RED) {
                        score += WIN_AMT;
                    } else {
                        score -= WIN_AMT * LOSE_MULTIPLIER;
                    }
                }
            }
            lastPosition = currentPosition;
        }
    }
    return score;
}

double DecisionTree::calculateDownLeftScore() {
    double score = 0;
    uint16_t numSlots = m_board-> getSlotCount();
    Vector2i boardDims = m_board -> getDimensions();
    uint16_t numCols = boardDims.x;
    uint16_t numRows = boardDims.y;
    int minIndex = numSlots - (numCols*3);
    InRowCounter counter;
    PlayerType currentSlotType;
    Vector2i position;
    Vector2i startPosition;
    // subtract 6 * j
    // i.e. subtract (numCols - 1) * j

    //std::cout << "[DEBUG] minIndex=" << minIndex << std::endl;
    if(m_state != nullptr) {
        for (int i = numSlots-1; i >= minIndex; i--) {
            //std::cout << "[DEBUG] " << std::endl;
            counter.red = 0;
            counter.yellow = 0;
            startPosition = m_board->indexToVec(i);
            //std::cout << "[DEBUG] " << vecToStr(&startPosition) << std::endl;
            if(startPosition.x < numCols-3) {
                for (int j = 0; j < 4; j++) {
                    int offset = -(j *(numCols-1));
                    position = m_board->indexToVec(i+offset);
                    //std::cout << "[DEBUG] position=" << vecToStr(&position) << std::endl;
                    //std::cout << "[DEBUG] i+offset=" << i+offset << std::endl;

                    currentSlotType = m_state[i+offset]->getType();
                    if(currentSlotType == PlayerType::PLR_YELLOW) {
                        counter.yellow++;
                        if((counter.red == 3) && (m_player == PlayerType::PLR_YELLOW)) {
                            score += BLOCK_AMT;
                        }
                        counter.red = 0;
                    }
                    else if(currentSlotType == PlayerType::PLR_RED) {
                        counter.red++;
                        if((counter.yellow == 3) && (m_player == PlayerType::PLR_RED)) {
                            score += BLOCK_AMT;
                        }
                        counter.yellow = 0;
                    }
                    else if (currentSlotType == PlayerType::PLR_NONE) {
                        counter.red = 0;
                        counter.yellow = 0;
                    }
                }
                if (counter.yellow >= 4) {
                    if(m_player == PlayerType::PLR_YELLOW) {
                        score += WIN_AMT;
                    }
                    else if (m_player == PlayerType::PLR_RED) {
                        score -= WIN_AMT * LOSE_MULTIPLIER;
                    }
                }
                else if (counter.red >= 4) {
                    if(m_player == PlayerType::PLR_RED) {
                        score += WIN_AMT;
                    }
                    else if (m_player == PlayerType::PLR_YELLOW) {
                        score -= WIN_AMT * LOSE_MULTIPLIER;
                    }
                }
            }
        }
    }
    return score;
}

double DecisionTree::calculateDownRightScore() {
    double score = 0;
    uint16_t numSlots = m_board-> getSlotCount();
    Vector2i boardDims = m_board -> getDimensions();
    uint16_t numCols = boardDims.x;
    uint16_t numRows = boardDims.y;
    int minIndex = numSlots - (numCols*3);
    InRowCounter counter;
    PlayerType currentSlotType;
    Vector2i position;
    Vector2i startPosition;
    // subtract 8 * j
    // i.e. subtract (numCols - 1) * j

    if(m_state != nullptr) {
        for (int i = numSlots-1; i >= minIndex; i--) {
            //std::cout << "[DEBUG] " << std::endl;
            counter.red = 0;
            counter.yellow = 0;
            startPosition = m_board->indexToVec(i);
            //std::cout << "[DEBUG] " << vecToStr(&startPosition) << std::endl;
            if(startPosition.x > 3) {
                for (int j = 0; j < 4; j++) {
                    int offset = -(j *(numCols+1));
                    position = m_board->indexToVec(i+offset);

                    currentSlotType = m_state[i+offset]->getType();
                    if(currentSlotType == PlayerType::PLR_YELLOW) {
                        counter.yellow++;
                        if((counter.red == 3) && (m_player == PlayerType::PLR_YELLOW)) {
                            score += BLOCK_AMT;
                        }
                        counter.red = 0;
                    }
                    else if(currentSlotType == PlayerType::PLR_RED) {
                        counter.red++;
                        if((counter.yellow == 3) && (m_player == PlayerType::PLR_RED)) {
                            score += BLOCK_AMT;
                        }
                        counter.yellow = 0;
                    }
                    else if (currentSlotType == PlayerType::PLR_NONE) {
                        counter.red = 0;
                        counter.yellow = 0;
                    }
                }
                if (counter.yellow >= 4) {
                    if(m_player == PlayerType::PLR_YELLOW) {
                        score += WIN_AMT;
                    }
                    else if (m_player == PlayerType::PLR_RED) {
                        score -= WIN_AMT * LOSE_MULTIPLIER;
                    }
                }
                else if (counter.red >= 4) {
                    if(m_player == PlayerType::PLR_RED) {
                        score += WIN_AMT;
                    }
                    else if (m_player == PlayerType::PLR_YELLOW) {
                        score -= WIN_AMT * LOSE_MULTIPLIER;
                    }
                }
            }
        }
    }
    return score;
}

// TODO: Add score calculating for diagonals
double DecisionTree::evaluateState() {
    double result = 0;
    result += calculateVerticalScore();
    result += calculateHorizontalScore();
    result += calculateDownLeftScore();
    result += calculateDownRightScore();
    return result;
    //std::cout << "[DEBUG] Score: " << m_score << std::endl;
}

int DecisionTree::getBestAction() {
    DecisionTree* subtree = nullptr;
    Vector2i boardDims = m_board->getDimensions();
    double currentMaxScore = -DBL_MAX;
    double currentScore = 0;
    int bestAction = -1;
    for (int i = 0; i < boardDims.x; i++) {
        subtree = new DecisionTree(m_state, m_board, m_player);
        subtree->manageTurnSim(i, m_player);
        if((subtree->getScore() > currentMaxScore) && (m_board->getAvailableSlot(i) != -1)) {
            currentMaxScore = subtree->getScore();
            bestAction = i;
        }
        delete(subtree);
        subtree = nullptr;
    }
    std::cout << "[DEBUG] Best score: " << currentMaxScore << std::endl;
    return bestAction;
}