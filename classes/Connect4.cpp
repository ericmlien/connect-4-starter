#include "Connect4.h"
using namespace std;

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
    cout << "We found a player! Player index: " << player -> playerNumber() << endl;
    cout << "Found player position: (" << square -> getColumn() << ", " << square -> getRow() << ")" << endl;
    return player;
}

Player* Connect4::checkForWinner() {
    // int windowSize = 4;
    // for (int i = 0; i < CONNECT4_WIDTH - (windowSize - 1); i++) {
    //     for (int j = 0; j < CONNECT4_HEIGHT - (windowSize - 1); j++) {
    //         //Check Right and Left column for vertical 4.

    //         //Check Top and Bottom Rows for horizontal 4.

    //         //Check Right Up Diagonal

    //         //Check Right Down Diagonal
    //     }
    // }


    return nullptr;
}

bool Connect4::checkForDraw() {
    for (int i = 0; i < CONNECT4_WIDTH; i++) {
        if (ownerAt(i, 0) != nullptr) return false;
    }
    return true;
}

string Connect4::initialStateString() {
    return "000000000000000000000000000000000000000000";
}

string Connect4::stateString() {
    string state = initialStateString();
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
            square->setBit( PieceForPlayer(playerNumber-1) );
        } else {
            square->setBit( nullptr );
        }
    });
}

// returns the row value of the given x column value.
int Connect4::lowestFreeSpot(int x) {
    int lowestRow = -1;

    for (int i = 0; i < CONNECT4_HEIGHT; i++) {
        if (ownerAt(x, i) != nullptr) {
            cout << "We've hit a placed piece at: (" << x << ", " << i << ")" << endl;
            break;
        }
        else lowestRow = i;
    }

    cout << "Our lowest row is: (" << x << ", " << lowestRow << ")" << endl;
    return lowestRow;
    
}

Bit* Connect4::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    // should possibly be cached from player class
    bit->LoadTextureFromFile(playerNumber == AI_PLAYER ? "yellow.png" : "red.png");
    bit->setOwner(getPlayerAt(playerNumber == AI_PLAYER ? 1 : 0));
    return bit;
}

void Connect4::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {

}

bool Connect4::canBitMoveFrom(Bit &bit, BitHolder &src) {
    return false;
}

bool Connect4::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {
    return false;
}

bool Connect4::actionForEmptyHolder(BitHolder &holder) {
    ChessSquare* clickedSquare = static_cast<ChessSquare*>(&holder);
    if (!clickedSquare) return false;
    
    int placedX = clickedSquare -> getColumn();
    cout << "Checking column " << placedX << endl;

    int lowestY = lowestFreeSpot(placedX);
    cout << "We found the lowest spot" << endl;

    if (lowestY < 0) {
        cout << "Column is Full!" << endl;
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

void Connect4::stopGame() {
    
}









