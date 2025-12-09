//
// Created by jwhal on 6/13/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_UNITTESTS_H
#define JOHNS_CPP_CHESS_AI_UNITTESTS_H
#include "Movement.h"


class unittests {
public:
    static void perft();

    static void perft(int depth);

    static void printBitboard(unsigned long long bitboard);

    static void printTripleBitboard(ull piece, ull blockers, ull attackingSquares);

    static void printDoubleBitboard(unsigned long long top, unsigned long long bottom);
    static void printBoard(unsigned short *chessBoard);

    static void printAllMovement();
};


#endif //JOHNS_CPP_CHESS_AI_UNITTESTS_H
