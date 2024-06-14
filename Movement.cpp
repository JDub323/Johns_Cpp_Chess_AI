//
// Created by jwhal on 6/13/2024.
//

#include "Movement.h"
#include "Utilities.h"
#include "ChessConstants.h"
#include <bit>

using namespace ChessConstants;
using ull = unsigned long long;

void Movement::initializePieceMovement() {
    initializePawnPushesTables();
    initializePawnAttacksTables();
    initializeKnightAttacksTable();
    initializeBishopTargetsTable();
    initializeRookTargetsTable();
    initializeQueenTargetsTable();
    initializeKingAttacksTable();
    initializeMagicMaskTables();
    initializeMagicDatabases();
}

void Movement::initializePawnPushesTables() {
    for (int square = 0; square < 64; square++) {
        ull squareBB = Utilities::toBitboard(square);
        whitePawnPushesTable[square] = squareBB<<8;
        blackPawnPushesTable[square] = squareBB>>8;
        if (squareBB & RANK_2){//if white is on the second rank, can move two squares
            whitePawnPushesTable[square] |= squareBB<<16;
        }
        if (squareBB & RANK_7) {//if black is on the seventh rank, can move two squares
            blackPawnPushesTable[square] |= squareBB>>16;
        }
    }
}

void Movement::initializePawnAttacksTables() {
    for (int square = 0; square < 64; square++) {
        ull squareBB = Utilities::toBitboard(square);
        whitePawnAttacksTable[square] = squareBB<<7 | squareBB<<9;
        blackPawnAttacksTable[square] = squareBB>>7 | squareBB>>9;
        if (squareBB & A_FILE) {//don't want pawns wrapping around the board
            whitePawnAttacksTable[square] &= ~H_FILE;
            blackPawnAttacksTable[square] &= ~H_FILE;
        }
        if (squareBB & H_FILE) {//don't want pawns wrapping around the board
            whitePawnAttacksTable[square] &= ~A_FILE;
            blackPawnAttacksTable[square] &= ~A_FILE;
        }
    }
}

void Movement::initializeKnightAttacksTable() {
    ull temp;
    for (int square = 0; square < 64; square++) {//I'm sorry about this
        ull squareBB = Utilities::toBitboard(square);
        temp = ((squareBB<<6|squareBB>>10)&NOT_GH_FILE)|((squareBB<<10|squareBB>>6)&NOT_AB_FILE);
        temp |= ((squareBB<<17|squareBB>>15)&NOT_A_FILE)|((squareBB<<15|squareBB>>17)&NOT_H_FILE);
        knightAttacksTable[square] = temp;
    }
}

void Movement::initializeBishopTargetsTable() {
    for (int square = 0; square < 64; square++) {
        bishopTargetsTable[square] = generateBishopAttacks(square, 1ULL);
    }
}

void Movement::initializeRookTargetsTable() {
    for (int square = 0; square < 64; square++) {
        rookTargetsTable[square] = generateRookAttacks(square, 0ULL);
    }
}

void Movement::initializeQueenTargetsTable() {
    for (int square = 0; square < 64; square++) {
        queenTargetsTable[square] = bishopTargetsTable[square] | rookTargetsTable[square];
    }
}

void Movement::initializeKingAttacksTable() {
    for (int square = 0; square < 64; square ++) {//I'm sorry about this too
        ull squareBB = Utilities::toBitboard(square);
        kingAttacksTable[square] = squareBB<<1|squareBB<<9|squareBB<<8|squareBB<<7
                |squareBB>>1|squareBB>>9|squareBB>>8|squareBB>>7;
        if (squareBB & H_FILE) {
            kingAttacksTable[square] &= ~A_FILE;
        }
        else if (squareBB & A_FILE) {
            kingAttacksTable[square] &= ~H_FILE;
        }
    }
}

void Movement::initializeMagicDatabases() {
    bishopAttacksTable = new unsigned long long*[64];
    rookAttacksTable = new unsigned long long*[64];
    for (int square = 0; square < 64; square++) {
        bishopAttacksTable[square] = new ull[1<<(64-Magics::bishopBitshiftBy[square])];
        ull bishopMask = bishopMaskTable[square];
        int maxBlockerCombos = 1<<std::popcount(bishopMask);//counts bits
        for (int i = 0; i < maxBlockerCombos; i++) {
            ull bishopBlockers = getBlockerCombo(i, bishopMask);
            ull index = (bishopBlockers * Magics::bishopMagics[square]) >> Magics::bishopBitshiftBy[square];
            bishopAttacksTable[square][index] = generateBishopAttacks(square, bishopBlockers);
        }

        rookAttacksTable[square] = new ull[1<<(64-Magics::rookBitshiftBy[square])];
        ull rookMask = rookMaskTable[square];
        maxBlockerCombos = 1<<std::popcount(rookMask);//counts bits
        for (int i = 0; i < maxBlockerCombos; i++) {
            ull rookBlockers = getBlockerCombo(i, rookMask);
            ull index = (rookBlockers * Magics::rookMagics[square]) >> Magics::rookBitshiftBy[square];
            rookAttacksTable[square][index] = generateRookAttacks(square, rookBlockers);
        }
    }
}

void Movement::initializeMagicMaskTables() {
    for (int square = 0; square < 64; square++) {
        ull squareBB = Utilities::toBitboard(square);
        ull squareAdder = squareBB;
        ull attacksBB = 0;
        while (!(squareAdder & H_FILE) && !(squareAdder & RANK_8)) {//up and to the right
            squareAdder <<= 9;
            attacksBB |= squareAdder;
        }
        squareAdder = squareBB;
        attacksBB &= ~(H_FILE | RANK_8);

        while (!(squareAdder & A_FILE) && !(squareAdder & RANK_8)) {//up and to the left
            squareAdder <<= 7;
            attacksBB |= squareAdder;
        }
        squareAdder = squareBB;
        attacksBB &= ~(A_FILE | RANK_8);

        while (!(squareAdder & A_FILE) && !(squareAdder & RANK_1)) {//down and to the left
            squareAdder >>= 9;
            attacksBB |= squareAdder;
        }
        squareAdder = squareBB;
        attacksBB &= ~(A_FILE | RANK_1);

        while (!(squareAdder & H_FILE) && !(squareAdder & RANK_1)) {//down and to the right
            squareAdder >>= 7;
            attacksBB |= squareAdder;
        }
        attacksBB &= ~(H_FILE | RANK_1);

        bishopMaskTable[square] = attacksBB;



        squareAdder = squareBB;
        attacksBB = 0;

        while (!(squareAdder & (H_FILE | G_FILE))) {//right
            squareAdder <<= 1;
            attacksBB |= squareAdder;
        }
        squareAdder = squareBB;

        while (!(squareAdder & (RANK_8 | RANK_7))) {//up
            squareAdder <<= 8;
            attacksBB |= squareAdder;
        }
        squareAdder = squareBB;

        while (!(squareAdder & (A_FILE | B_FILE))) {//left
            squareAdder >>= 1;
            attacksBB |= squareAdder;
        }
        squareAdder = squareBB;

        while (!(squareAdder & (RANK_1 | RANK_2))) {//down
            squareAdder >>= 8;
            attacksBB |= squareAdder;
        }

        rookMaskTable[square] = attacksBB;
    }
}

ull Movement::getBlockerCombo(int blockerNum, ull mask) {
    //for each bit on blockerNum, cast onto each on bit of mask
    ull ret = 0;
    int bitOn = 1;
    while(mask) {
        int nextSquareOnBlockers = Utilities::numberOfTrailingZeroes(mask);
        ull nextSquareOnBlockersBB = Utilities::toBitboard(nextSquareOnBlockers);
        mask ^= nextSquareOnBlockersBB;

        if (bitOn & blockerNum) {//the bit that is being checked is on for blockerNum
            ret |= nextSquareOnBlockersBB;
        }
        bitOn <<= 1;
    }
    return ret;
}

ull Movement::generateBishopAttacks(int square, ull allPieces) {
    ull squareBB = Utilities::toBitboard(square);
    ull squareAdder = squareBB;
    ull attacksBB = 0;
    while (!(squareAdder & H_FILE) && !(squareAdder & RANK_8) && !(squareAdder & allPieces)) {//up and to the right
        squareAdder <<= 9;
        attacksBB |= squareAdder;
    }
    squareAdder = squareBB;

    while (!(squareAdder & A_FILE) && !(squareAdder & RANK_8) && !(squareAdder & allPieces)) {//up and to the left
        squareAdder <<= 7;
        attacksBB |= squareAdder;
    }
    squareAdder = squareBB;

    while (!(squareAdder & A_FILE) && !(squareAdder & RANK_1) && !(squareAdder & allPieces)) {//down and to the left
        squareAdder >>= 9;
        attacksBB |= squareAdder;
    }
    squareAdder = squareBB;

    while (!(squareAdder & H_FILE) && !(squareAdder & RANK_1) && !(squareAdder & allPieces)) {//down and to the right
        squareAdder >>= 7;
        attacksBB |= squareAdder;
    }

    return attacksBB;
}

ull Movement::generateRookAttacks(int square, ull allPieces) {
    ull squareBB = Utilities::toBitboard(square);
    ull emptySquares = ~allPieces;
    ull squareAdder = squareBB;
    ull attacksBB = 0;

    while (!(squareAdder & H_FILE & emptySquares)) {//right
        squareAdder <<= 1;
        attacksBB |= squareAdder;
    }
    squareAdder = squareBB;
    while (!(squareAdder & RANK_8 & emptySquares)) {//up
        squareAdder <<= 8;
        attacksBB |= squareAdder;
    }
    squareAdder = squareBB;
    while (!(squareAdder & A_FILE & emptySquares)) {//left
        squareAdder >>= 1;
        attacksBB |= squareAdder;
    }
    squareAdder = squareBB;
    while (!(squareAdder & RANK_1 & emptySquares)) {//down
        squareAdder >>= 8;
        attacksBB |= squareAdder;
    }

    return attacksBB;
}
