//
// Created by jwhal on 6/13/2024.
//

#include "unittests.h"
#include "Utilities.h"
#include "Position.h"
#include "ChessConstants.h"
#include <iostream>

int main() {
    auto* pos = new Position(ChessConstants::startpos);
    unittests::printBoard(pos->getChessBoard());
    delete pos;
    return 0;
}

void unittests::printBitboard(unsigned long long bitboard) {
    for (int row = 7; row >=0; row--) {
        for (int col = 0; col<8; col++) {
            if (bitboard & 1ULL<<(row*8+col)) {
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

