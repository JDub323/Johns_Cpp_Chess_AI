//
// Created by jwhal on 1/19/2024.
//

#include "Position.h"

#include <iostream>
#include <string>
#include <vector>
#include "Utilities.h"
#include "Pieces.h"
#include "ChessConstants.h"

using namespace std;
using namespace Pieces;
using namespace ChessConstants;
//remember to initialize legal moves here too
Position::Position(string fen) : pieceBitboards{0}, chessBoard{0}, castlingRights{false} {
    int firstSpaceIndex = (int) fen.find(' ');
    int stringTicker = 0;
    char next;
    for (int i=7; i>=0; i--) {//for each row on the chessboard
        int square = i*8;
        next = fen[stringTicker];
        while (stringTicker < firstSpaceIndex && next != '/') {
            if (isdigit(next)) {
                square += next-48;//uses ASCII value for char, to get number from a char, subtract 48
            }
            else {
                unsigned short pieceFound = Utilities::getPiece(next);
                chessBoard[square] = pieceFound;
                pieceBitboards[pieceFound] |= Utilities::toBitboard(square);
                square++;
            }
            next = fen[++stringTicker];
        }
        stringTicker++;//skips over the '/'
    }

    vector<string> fenArray = Utilities::split(fen,' ');

    whiteToMove = fenArray[1] == "w";

    string castlingRightsStr = fenArray[2];
    castlingRights[WHITE_CASTLE_SHORT] = castlingRightsStr.find('K') != string::npos;
    castlingRights[WHITE_CASTLE_LONG] = castlingRightsStr.find('Q') != string::npos;
    castlingRights[BLACK_CASTLE_SHORT] = castlingRightsStr.find('k') != string::npos;
    castlingRights[BLACK_CASTLE_LONG] = castlingRightsStr.find('q') != string::npos;

    if (fenArray[3] != "-") {//if there is an en passant target square
        enPassantTargetSquare = Utilities::toBitboard(Utilities::getSquare(fenArray[3]));
    }
    else enPassantTargetSquare = 0;

    halfmoveClock = stoi(fenArray[4]);

    plyNumber = stoi(fenArray[5])*2;//this is correct math that I won't explain
    if (whiteToMove) plyNumber--;//Source: trust me bro

    zobristKey = Utilities::makeZobristKey(chessBoard, whiteToMove, castlingRights, enPassantTargetSquare);
    calculatePrecalculatedData();//initializes gameState, not legal moves
    gameState = 0;
}

void Position::printPieceArray() {
    for (int row = 7; row >=0; row--) {
        for (int col = 0; col<8; col++) {
            const int square = row * 8 + col;
            std::cout << pieceChars[chessBoard[square]];
            if (col != 7) {
                std::cout << '|';
            }
        }
        std::cout << std::endl;
    }
}

void Position::printAllBitboards() {
    for (int color = 0; color <= 8; color+=8) {
        for (int piece = 1; piece <= 6; piece++) {
            ull pieceBB = pieceBitboards[color | piece];
            std::cout << "\nBitboard for: " << pieceNames[color | piece] << std::endl;
            Utilities::printBitboard(pieceBB);
            std::cout << std::endl;
        }
    }
}

void Position::printPos() {
    std::cout << "\n\n\n----------Position Information-----------\n" <<
        "Zobrist key: " << zobristKey << "\n" <<
        "Game State: " << gameState << "\n" <<
        "Ply Number: " << plyNumber << "\n" <<
        "Halfmove clock: " << halfmoveClock << "\n" <<
        "Player to move: " << (whiteToMove ? 'w' : 'b') << "\n" <<
        "Castling rights: " << castlingRights << "\n" <<
        "En passant target square: " << enPassantTargetSquare << "\n" <<
        "\nPIECE ARRAY'S POSITION:\n";

    printPieceArray();

    std::cout << "\nBITBOARDS:\n";

    printAllBitboards();
}

void Position::makeMove() {

}

void Position::unmakeMove() {

}

void Position::generateMoves() {

}

void Position::generateCapturingMoves() {

}

void Position::calculatePrecalculatedData() {
//initialize gameState, find checks, pins,
}



