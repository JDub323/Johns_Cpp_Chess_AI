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
    unittests::printAllMovement();
    return 0;
}

void unittests::printBitboard(ull bitboard) {
    printDoubleBitboard(bitboard,0);
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
        ull bitboard = Movement::getKnightAttacks(square);
        printDoubleBitboard(bitboard,Utilities::toBitboard(square));
        std::cout << "--------Square "+std::to_string(square)+"--------" << std::endl;
    }
}

