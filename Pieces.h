//
// Created by jwhal on 6/11/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_PIECES_H
#define JOHNS_CPP_CHESS_AI_PIECES_H
#include <array>
#include <string_view>

namespace Pieces {
    static constexpr short WHITE = 0;
    static constexpr short BLACK = 8;

    static constexpr short EMPTY = 0;
    static constexpr short PAWN = 1;
    static constexpr short KNIGHT = 2;
    static constexpr short BISHOP = 3;
    static constexpr short ROOK = 4;
    static constexpr short QUEEN = 5;
    static constexpr short KING = 6;
};

static constexpr std::array<std::string_view, 15> pieceNames = {
    "Empty",         // 0
    "White Pawn",    // 1
    "White Knight",  // 2
    "White Bishop",  // 3
    "White Rook",    // 4
    "White Queen",   // 5
    "White King",    // 6
    "",              // 7
    "",              // 8
    "Black Pawn",    // 8 + 1 = 9
    "Black Knight",  // 10
    "Black Bishop",  // 11
    "Black Rook",    // 12
    "Black Queen",   // 13
    "Black King",    // 14
};

static constexpr std::array<char, 15> pieceChars = {
    ' ', // 0
    'P', // 1
    'N', // 2
    'B', // 3
    'R', // 4
    'Q', // 5
    'K', // 6
    'E', // 7
    'E', // 8
    'p', // 9
    'n', // 10
    'b', // 11
    'r', // 12
    'q', // 13
    'k', // 14
};



#endif //JOHNS_CPP_CHESS_AI_PIECES_H
