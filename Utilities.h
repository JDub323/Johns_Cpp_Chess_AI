//
// Created by jwhal on 6/11/2024.
//

#include <stdint.h>
#include <string>
#include <vector>

#include "Movement.h"

#ifndef JOHNS_CPP_CHESS_AI_UTILITIES_H
#define JOHNS_CPP_CHESS_AI_UTILITIES_H

#endif //JOHNS_CPP_CHESS_AI_UTILITIES_H

using namespace std;
class Utilities {
public:
    static vector<string> split(string& string, char delimiter);

    static void printTripleBitboard(ull piece, ull blockers, ull attackingSquares);

    static unsigned short getPiece(char next);

    static char getPiece(unsigned short piece);

    static unsigned short getSquare(string &square);

    static string getSquare(short square);

    static unsigned inline long long toBitboard(int in) { return 1ULL << in; }

    static void printBitboard(ull bitboard);

    static unsigned inline short numberOfTrailingZeroes(unsigned long long bitboard) {
        if (bitboard == 0) return 64; //make sure input is nonzero
        return __builtin_ctzll(bitboard);
    };

    static unsigned long long makeZobristKey(const unsigned short *chessboard, bool whiteToMove, const bool *castlingRights,
                                          unsigned long long enPassantTargetSquare);
    static unsigned inline long long getWhiteToMoveZobrist();
    static unsigned inline long long getEnPassantSquareZobrist(int square);
    static unsigned inline long long getCastlingRightsZobrist(const bool *castlingRights);
    static unsigned inline long long getPieceSquareZobrist(int piece, int square);

    static int pop_lsb(uint64_t &bb);
};