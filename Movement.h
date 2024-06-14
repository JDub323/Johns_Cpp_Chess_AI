//
// Created by jwhal on 6/13/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_MOVEMENT_H
#define JOHNS_CPP_CHESS_AI_MOVEMENT_H

#include "Magics.h"

using ull = unsigned long long;
class Movement {
public:
    static void initializePieceMovement();
    static inline ull getLineThrough(int square1, int square2){
        if (square1 % 8 == square2 % 8 || square1 / 8 == square2 / 8) {//if they are on the same file or same rank
            return (rookTargetsTable[square1] & rookTargetsTable[square2]);
        }
        return (bishopTargetsTable[square1] & bishopTargetsTable[square2]);
    };
    static inline ull getPawnPushes(int square, bool isWhite){
        if (isWhite)return whitePawnPushesTable[square];
        return blackPawnPushesTable[square];
    };
    static inline ull getPawnAttacks(int square, bool isWhite){
        if (isWhite)return whitePawnAttacksTable[square];
        return blackPawnAttacksTable[square];
    };
    static inline ull getKnightAttacks(int square) {return knightAttacksTable[square];};
    static inline ull getBishopAttacks(int square, ull allPieces){
        ull blockers = allPieces & bishopMaskTable[square];
        ull index = (blockers * Magics::bishopMagics[square]) >> Magics::bishopBitshiftBy[square];
        return bishopAttacksTable[square][index];
    };
    static inline ull getRookAttacks(int square, ull allPieces){
        ull blockers = allPieces & rookMaskTable[square];
        ull index = (blockers * Magics::rookMagics[square]) >> Magics::rookBitshiftBy[square];
        return rookAttacksTable[square][index];
    };
    static inline ull getQueenAttacks(int square, ull allPieces){
        return getBishopAttacks(square, allPieces) | getRookAttacks(square, allPieces);
    };
    static inline ull getKingAttacks(int square) {return kingAttacksTable[square];};
    static inline ull getBishopTargets(int square) {return bishopTargetsTable[square];};
    static inline ull getRookTargets(int square) {return rookTargetsTable[square];};
    static inline ull getQueenTargets(int square) {return queenTargetsTable[square];};



private:
    static inline ull whitePawnPushesTable[64];
    static inline ull whitePawnAttacksTable[64];
    static inline ull blackPawnPushesTable[64];
    static inline ull blackPawnAttacksTable[64];
    static inline ull knightAttacksTable[64];
    static inline ull bishopTargetsTable[64];
    static inline ull rookTargetsTable[64];
    static inline ull queenTargetsTable[64];
    static inline ull kingAttacksTable[64];
    static inline ull** bishopAttacksTable;
    static inline ull** rookAttacksTable;
    static inline ull bishopMaskTable[64];
    static inline ull rookMaskTable[64];


    static void initializePawnPushesTables();
    static void initializePawnAttacksTables();
    static void initializeKnightAttacksTable();
    static void initializeBishopTargetsTable();
    static void initializeRookTargetsTable();
    static void initializeQueenTargetsTable();
    static void initializeKingAttacksTable();
    static void initializeMagicDatabases();
    static void initializeMagicMaskTables();

    static ull getBlockerCombo(int blockerNum, ull mask);
    static ull generateBishopAttacks(int square, ull allPieces);
    static ull generateRookAttacks(int square, ull allPieces);

};


#endif //JOHNS_CPP_CHESS_AI_MOVEMENT_H
