#pragma once
#include "Game.h"

const int CONNECT4_WIDTH = 7;
const int CONNECT4_HEIGHT = 6;

class Connect4 : public Game {
    public:
        Connect4();
        ~Connect4();

        void        setUpBoard() override;
        Player*     checkForWinner() override;
        bool        checkForDraw() override;
        std::string initialStateString() override;
        std::string stateString() override;
        void        setStateString(const std::string &s) override;
        bool        actionForEmptyHolder(BitHolder &holder) override;
        bool        canBitMoveFrom(Bit &bit, BitHolder &src);
        bool        canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst);
        void        stopGame() override;
        void        bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;

    // AI methods
    private:
        Grid*       _grid;
        Grid*       getGrid() override { return _grid; }
        Player*     ownerAt(int x, int y);
        Bit*        PieceForPlayer(const int playerNumber);

        int         lowestFreeSpot (int x);
};