//
// Created by jwhal on 6/13/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_MOVEMENT_H
#define JOHNS_CPP_CHESS_AI_MOVEMENT_H

using ull = unsigned long long;
class Movement {
public:
    static inline ull pawnPushes(int square);
    static inline ull pawnAttacks(int square);
    static inline ull knightAttacks(int square);
    static inline ull bishopAttacks(int square, ull allPieces);
    static inline ull rookAttacks(int square, ull allPieces);
    static inline ull queenAttacks(int square, ull allPieces);
    static inline ull kingAttacks(int square);
    static inline ull bishopTargets(int square);
    static inline ull rookTargets(int square);
    static inline ull queenTargets(int square);

private:
    static inline ull pawnPushesTable[64];
    static inline ull pawnAttacksTable[64];
    static inline ull knightAttacksTable[64];
    static inline ull bishopTargetsTable[64];
    static inline ull rookTargetsTable[64];
    static inline ull queenTargetsTable[64];
    static inline ull kingAttacksTable[64];
};


#endif //JOHNS_CPP_CHESS_AI_MOVEMENT_H
