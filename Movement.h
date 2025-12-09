//
// Created by jwhal on 6/13/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_MOVEMENT_H
#define JOHNS_CPP_CHESS_AI_MOVEMENT_H

#include <iterator>
#include <ranges>

#include "ChessConstants.h"
#include "Magics.h"
#include "Pieces.h"

using ull = unsigned long long;
using namespace ChessConstants;
using namespace Pieces;

class Movement {
public:
    static void initializePieceMovement();
    static ull getLineThrough(const int square1, const int square2){
        if (square1 % 8 == square2 % 8 || square1 / 8 == square2 / 8) {//if they are on the same file or same rank
            return (rookTargetsTable[square1] & rookTargetsTable[square2]);
        }
        return (bishopTargetsTable[square1] & bishopTargetsTable[square2]);
    }
    static ull getPawnPushes(const int square, const bool isWhite){
        if (isWhite)return whitePawnPushesTable[square];
        return blackPawnPushesTable[square];
    }
    static ull getPawnAttacks(const int square, const bool isWhite){
        if (isWhite)return whitePawnAttacksTable[square];
        return blackPawnAttacksTable[square];
    }
    static ull getKnightAttacks(const int square) {return knightAttacksTable[square];};
    static ull getBishopAttacks(const int square, const ull allPieces){
        ull blockers = allPieces & bishopMaskTable[square];
        ull index = (blockers * Magics::bishopMagics[square]) >> Magics::bishopBitshiftBy[square];
        return bishopAttacksTable[square][index];
    }
    static ull getRookAttacks(const int square, const ull allPieces){
        ull blockers = allPieces & rookMaskTable[square];
        ull index = (blockers * Magics::rookMagics[square]) >> Magics::rookBitshiftBy[square];
        return rookAttacksTable[square][index];
    }
    static ull getQueenAttacks(const int square, const ull allPieces){
        return getBishopAttacks(square, allPieces) | getRookAttacks(square, allPieces);
    }
    static ull getKingAttacks(const int square) {return kingAttacksTable[square];}
    static ull getBishopTargets(const int square) {return bishopTargetsTable[square];}
    static ull getRookTargets(const int square) {return rookTargetsTable[square];}
    static ull getQueenTargets(const int square) {return queenTargetsTable[square];}

    //this function ignores checks
    static ull getPieceMovement(const unsigned short piece, const int square, const ull friendlyPieces, const ull enemyPieces) {
        const bool isWhite = piece / 8 > 0;
        switch (piece) {
            case PAWN:
                return getPawnMovement(square, isWhite, friendlyPieces, enemyPieces);
            case BISHOP:
                return getBishopMovement(square, friendlyPieces, enemyPieces);
            case KNIGHT:
                return getKnightMovement(square, friendlyPieces, enemyPieces);
            case ROOK:
                return getRookMovement(square, friendlyPieces, enemyPieces);
            case QUEEN:
                return getQueenMovement(square, friendlyPieces, enemyPieces);
            case KING:
                return getKingMovement(square, friendlyPieces, enemyPieces);
            default:
                throw std::invalid_argument("incorrect piece value");
        }
    }
    static ull getPawnMovement(const int square, const bool isWhite, const ull friendlyPieces, const ull enemyPieces) {
        const ull squareBB = 1l << square;
        const ull allPieces = friendlyPieces | enemyPieces;

        ull ret = getPawnPushes(square, isWhite);
        if (ret & RANK_4 && squareBB & RANK_2 && squareBB << 8 & allPieces) {
            return 0l; // if blocked from going two squares, pawn can't hop over, basically
        }
        if (ret & RANK_5 && squareBB & RANK_7 && squareBB >> 8 & allPieces) {
            return 0l; // same but for black
        }

        ret &= ~allPieces;

        ret |= getPawnAttacks(square, isWhite) & enemyPieces;

        return ret;
    }
    static ull getKnightMovement(const int square, const ull friendlyPieces, const ull enemyPieces) {
        return getKnightAttacks(square) & ~friendlyPieces;
    }
    static ull getBishopMovement(const int square, const ull friendlyPieces, const ull enemyPieces) {
        const ull ret = getBishopAttacks(square, friendlyPieces | enemyPieces);
        return ret & ~friendlyPieces;
    }
    static ull getRookMovement(const int square, const ull friendlyPieces, const ull enemyPieces) {
        const ull ret = getRookAttacks(square, friendlyPieces | enemyPieces);
        return ret & ~friendlyPieces;
    }
    static ull getQueenMovement(const int square, const ull friendlyPieces, const ull enemyPieces) {
        const ull ret = getQueenAttacks(square, friendlyPieces | enemyPieces);
        return ret & ~friendlyPieces;
    }
    static ull getKingMovement(const int square, const ull friendlyPieces, const ull enemyPieces) {
        return getKingAttacks(square) & ~friendlyPieces;
    }

    static ull generateBishopAttacks(int square, ull allPieces);
    static ull generateRookAttacks(int square, ull allPieces);
    static ull createWhitePawnMovesBB(ull occupancies, ull friendlyPieces, ull enemyPieces);
    static ull createKnightAttacksBB(ull squareBB);
    static ull createKingAttacksBB(ull squareBB);




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

    ull getWhitePawnPushSquares(ull occupancyBB);

    static void initializePawnAttacksTables();


    static void initializeKnightAttacksTable();


    static void initializeBishopTargetsTable();
    static void initializeRookTargetsTable();
    static void initializeQueenTargetsTable();
    static void initializeKingAttacksTable();


    static void initializeMagicDatabases();
    static void initializeMagicMaskTables();

    static ull getBlockerCombo(int blockerNum, ull mask);

};


#endif //JOHNS_CPP_CHESS_AI_MOVEMENT_H
