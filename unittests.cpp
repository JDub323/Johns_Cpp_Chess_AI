//
// Created by jwhal on 6/13/2024.
//

#include "unittests.h"
#include "Utilities.h"
#include "Position.h"
#include "ChessConstants.h"
#include "Movement.h"
#include <iostream>

int main() {
    Movement::initializePieceMovement();
    // unittests::printAllMovement();
    auto pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //pos.printPos();
    return 0;
}

void unittests::perft() {
    constexpr int MAX_DEPTH = 6;
    constexpr ull PERFT_VALS [] = {1, 20, 400, 8902, 197281, 4865609, 119060324, 3195901860l, 84998978956l};
    for (int i = 1; i < MAX_DEPTH; i++) {

    }
}

void unittests::perft(int depth) {

}

void unittests::printBitboard(ull bitboard) {
    printDoubleBitboard(bitboard,0);
}

void unittests::printTripleBitboard(ull piece, ull blockers, ull attackingSquares) {
    for (int row = 7; row >=0; row--) {
        for (int col = 0; col<8; col++) {
            ull squareBB = Utilities::toBitboard(row*8+col);
            if (piece & squareBB) {
                std::cout << 'X';
            }
            else if (blockers & squareBB) {
                std::cout << 'B';
            }
            else if (attackingSquares & squareBB) {
                std::cout << 'O';
            }
            else {
                std::cout << ' ';
            }
            if (col != 7) {
                std::cout << '|';
            }
        }
        std::cout << std::endl;
    }
}

void unittests::printDoubleBitboard(ull top, ull bottom) {
    for (int row = 7; row >=0; row--) {
        for (int col = 0; col<8; col++) {
            ull squareBB = Utilities::toBitboard(row*8+col);
            if (top & squareBB) {
                std::cout << 'O';
            }
            else if (bottom & squareBB) {
                std::cout << 'X';
            }
            else {
                std::cout << ' ';
            }
            if (col != 7) {
                std::cout << '|';
            }
        }
        std::cout << std::endl;
    }
}

void unittests::printBoard(unsigned short *chessBoard) {
    for (int row = 7; row >=0; row--) {
        for (int col = 0; col<8; col++) {
            if (chessBoard[row*8+col]) {
                std::cout << Utilities::getPiece(chessBoard[row*8+col]);
            }
            else {
                std::cout << ' ';
            }
            if (col != 7) {
                std::cout << '|';
            }
        }
        std::cout << std::endl;
    }
}

void unittests::printAllMovement() {
    for (int square = 0; square < 64; square++) {
        constexpr ull blockers = 0x0000FF00000l;
        // do not print out invalid configs (blockers can't be on top of the square you are on)
        if ((blockers & Utilities::toBitboard(square)) != 0) {
            continue;
        }
        const ull attackingSquares = Movement::getKnightAttacks(square);
        printTripleBitboard(Utilities::toBitboard(square), blockers, attackingSquares);
        std::cout << "--------Square "+std::to_string(square)+"--------" << std::endl;
    }
}

