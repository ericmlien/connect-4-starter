#include "Connect4.h"

static const int AIBoardWeights[CONNECT4_WIDTH][CONNECT4_HEIGHT] = { 
    {3, 4, 5, 5, 4, 3},
    {4, 6, 7, 7, 6, 4},
    {5, 8, 11, 11, 8, 5},
    {7, 10, 13, 13, 10, 7},
    {5, 8,  11, 11, 8, 5},
    {4, 6, 7, 7, 6, 4},
    {3, 4, 5, 5, 4, 3}
};

Connect4::Connect4() : Game() {
    _grid = new Grid(CONNECT4_WIDTH, CONNECT4_HEIGHT);
}

Connect4::~Connect4() {
    delete _grid;
}

void Connect4::setUpBoard() {
    setNumberOfPlayers(2);
    _grid->initializeSquares(80, "square.png");
    _gameOptions.rowX = 7;
    _gameOptions.rowY = 6;

    startGame();
}

Player* Connect4::ownerAt(int x, int y) {
    if (x < 0 || x >= CONNECT4_WIDTH || y < 0 || y >= CONNECT4_HEIGHT) {
        return nullptr;
    }
    auto square = _grid -> getSquare(x, y);
    if (!square || !square -> bit()) {
        return nullptr;
    }
    Player* player = square -> bit() -> getOwner();
    std::cout << "We found a player! Player index: " << player -> playerNumber() << std::endl;
    std::cout << "Found player position: (" << square -> getColumn() << ", " << square -> getRow() << ")" << std::endl;
    return player;
}

#pragma region checkGameEnd
Player* Connect4::checkForWinner() {
    int windowSize = 4;
    for (int i = 0; i < CONNECT4_WIDTH - (windowSize - 1); i++) {
        for (int j = 0; j < CONNECT4_HEIGHT - (windowSize - 1); j++) {

            //Check Right and Left column for vertical 4.
            Player* leftColumn = checkWinnerColumn(i, j);
            if (leftColumn != nullptr) return leftColumn;
            
            Player* rightColumn = checkWinnerColumn(i + 3, j);
            if (rightColumn != nullptr) return rightColumn;

            //Check Top and Bottom Rows for horizontal 4.
            Player* topRow = checkWinnerRow(i, j);
            if (topRow != nullptr) return topRow;
            
            Player* bottomRow = checkWinnerRow(i, j + 3);
            if (bottomRow != nullptr) return bottomRow;

            //Check Right Down Diagonal
            Player* downRightDiag = checkWinnerDownRightDiag(i, j);
            if (downRightDiag != nullptr) return downRightDiag;

            //Check Right Up Diagonal
            Player* upRightDiag = checkWinnerUpRightDiag(i, j + 3);
            if (upRightDiag != nullptr) return upRightDiag;
            
        }
    }

    return nullptr;
} 

bool Connect4::checkForDraw() {
    for (int i = 0; i < CONNECT4_WIDTH; i++) {
        if (ownerAt(i, 0) == nullptr) return false;
    }
    return true;
}

#pragma region checkWinnerHelpers

Player* Connect4::checkWinnerColumn(int x, int y) {
    Player* top = ownerAt(x, y);
    bool winnerFound = (
        top != nullptr 
        && ownerAt(x, y + 1) == top
        && ownerAt(x, y + 2) == top
        && ownerAt(x, y + 3) == top
    );
    if (winnerFound) return top;
    return nullptr;
}

Player* Connect4::checkWinnerRow(int x, int y) {
    Player* left = ownerAt(x, y);
    bool winnerFound = (
        left != nullptr 
        && ownerAt(x + 1, y) == left
        && ownerAt(x + 2, y) == left
        && ownerAt(x + 3, y) == left
    );
    if (winnerFound) return left;
    return nullptr;
}

Player* Connect4::checkWinnerDownRightDiag(int x, int y) {
    Player* topLeft = ownerAt(x, y);
    bool winnerFound = (
        topLeft != nullptr 
        && ownerAt(x + 1, y + 1) == topLeft
        && ownerAt(x + 2, y + 2) == topLeft
        && ownerAt(x + 3, y + 3) == topLeft
    );
    if (winnerFound) return topLeft;
    return nullptr;
}

Player* Connect4::checkWinnerUpRightDiag(int x, int y) {
    Player* bottomLeft = ownerAt(x, y);
    bool winnerFound = (
        bottomLeft != nullptr 
        && ownerAt(x + 1, y - 1) == bottomLeft
        && ownerAt(x + 2, y - 2) == bottomLeft
        && ownerAt(x + 3, y - 3) == bottomLeft
    );
    if (winnerFound) return bottomLeft;
    return nullptr;
}

#pragma endregion checkWinnerHelpers

#pragma endregion checkGameEnd
#pragma region stateString
std::string Connect4::initialStateString() {
    return std::string(42, '0');
}

std::string Connect4::stateString() {
    std::string state = initialStateString();
    _grid -> forEachSquare([&](ChessSquare* square, int x, int y) {
        Bit *bit = square->bit();                                                                                                                                                                                                           
        if (bit) {
            state[y * 7 + x] = std::to_string(bit -> getOwner() -> playerNumber() + 1)[0];
        }
    });
    return state;
}

void Connect4::setStateString(const std::string &s) {
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y * 7 + x;
        int playerNumber = s[index] - '0';
        if (playerNumber) {
            square->setBit(PieceForPlayer(playerNumber - 1));
        } else {
            square->setBit(nullptr);
        }
    });
}
#pragma endregion stateString

Bit* Connect4::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    // should possibly be cached from player class
    bit->LoadTextureFromFile(playerNumber == AI_PLAYER ? "yellow.png" : "red.png");
    bit->setOwner(getPlayerAt(playerNumber == AI_PLAYER ? 1 : 0));
    return bit;
}

bool Connect4::actionForEmptyHolder(BitHolder &holder) {
    ChessSquare* clickedSquare = static_cast<ChessSquare*>(&holder);
    if (!clickedSquare) return false;
    
    int placedX = clickedSquare -> getColumn();
    std::cout << "Checking column " << placedX << std::endl;

    int lowestY = lowestFreeSpot(placedX);
    std::cout << "We found the lowest spot" << std::endl;

    if (lowestY < 0) {
        std::cout << "Column is Full!" << std::endl;
        return false;
    }

    Bit *toPlace = PieceForPlayer(getCurrentPlayer() -> playerNumber() == 0 ? HUMAN_PLAYER : AI_PLAYER);
    if (!toPlace) return false;

    ImVec2 initialPos = _grid -> getSquare(placedX, 0) -> getPosition();

    ChessSquare *finalSpot = _grid -> getSquare(placedX, lowestY);
    finalSpot -> setBit(toPlace);

    toPlace -> setPosition(initialPos);
    toPlace -> moveTo(finalSpot -> getPosition());

    endTurn();
    return true;
}

// returns the row value of the given x column value.
int Connect4::lowestFreeSpot(int x) {
    int lowestRow = -1;

    for (int i = 0; i < CONNECT4_HEIGHT; i++) {
        if (ownerAt(x, i) != nullptr) {
            std::cout << "We've hit a placed piece at: (" << x << ", " << i << ")" << std::endl;
            break;
        }
        else lowestRow = i;
    }

    std::cout << "Our lowest row is: (" << x << ", " << lowestRow << ")" << std::endl;
    return lowestRow;
    
}

void Connect4::updateAI() {
    return;
}

int Connect4::evaluateAIState(std::string state, int playerColor) {

} 

int Connect4::negamax(std::string state, int depth, int alpha, int beta, int playerColor){
    int bestVal = -1000;

    // int boardWinner = aiWinner(state);
    // if (boardWinner) {
    //     return -boardWinner;
    // }

    // bool boardFull = aiBoardFull(state);
    // if (boardFull) {
    //     return 0;
    // }

    // for (int i = 0; i < 9; i++) {
    //     if (state[i] == '0') {
    //         state[i] = playerColor == HUMAN_PLAYER ? '1' : '2';
    //         int result = -negamax(state, depth++, -beta, -alpha, 1-playerColor);
    //         if (result > bestVal) {
    //             bestVal = result;
    //         }

    //         if (result > alpha) {
    //             alpha = result;
    //         }
            
    //         state[i] = '0';

    //         if (alpha >= beta) {
    //             break;
    //         }
    //     }
    // }
    
    return bestVal;
}


void Connect4::stopGame() {
    
}

#pragma region bitMoving
void Connect4::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {

}

bool Connect4::canBitMoveFrom(Bit &bit, BitHolder &src) {
    return false;
}

bool Connect4::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {
    return false;
}
#pragma endregion bitMoving









