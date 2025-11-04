#pragma once
#include "Game.h"

const int CONNECT4_WIDTH = 7;
const int CONNECT4_HEIGHT = 6;
const int SCORES[5] = {0, 1, 10, 100, 1000};
const int MAX_DEPTH = 3;
const int MOVE_ORDER[7] = { 3,2,4,1,5,0,6 };


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
        void        updateAI() override;
        bool        gameHasAI() override { return true; }
        int         evaluateAIState(std::string& state, int playerColor);

    // AI methods
    private:
        Grid*       _grid;
        Grid*       getGrid() override { return _grid; }
        Player*     ownerAt(int x, int y);
        Bit*        PieceForPlayer(const int playerNumber);

        int         lowestFreeSpot (int x);
        Player*     checkWinnerColumn(int x, int y);
        Player*     checkWinnerRow(int x, int y);
        Player*     checkWinnerDownRightDiag(int x, int y);
        Player*     checkWinnerUpRightDiag(int x, int y);

        int         checkAIWinner(std::string& state);
        bool        aiBoardFull(const std::string& state);
        int         lowestFreeSpotAI (std::string& state, int x);

        int         checkWinStateColumn(std::string& state, int index);
        int         checkWinStateRow(std::string& state, int index);
        int         checkWinStateDownRightDiag(std::string& state, int index);
        int         checkWinStateUpRightDiag(std::string& state, int index);

        int         evaluateBoard(std::string& state);
        
        int         negamax(std::string& state, int depth, int alpha, int beta, int playerColor);
};