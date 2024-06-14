//
// Created by jwhal on 6/13/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_UNITTESTS_H
#define JOHNS_CPP_CHESS_AI_UNITTESTS_H


class unittests {
public:
    static void printBitboard(unsigned long long bitboard);
    static void printDoubleBitboard(unsigned long long top, unsigned long long bottom);
    static void printBoard(unsigned short *chessBoard);

    static void printAllMovement();
};


#endif //JOHNS_CPP_CHESS_AI_UNITTESTS_H
