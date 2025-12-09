//
// Created by jwhal on 3/19/2024.
//
#include <vector>
#include <sstream>
#include "Utilities.h"

#include <cstdint>

#include "Pieces.h"
// #include <intrin.h>
#include <iostream>

#include "Zobrists.h"
#include "ChessConstants.h"
using namespace std;
using namespace Pieces;

vector <string> Utilities::split(string& str, char delimiter) {
    vector<string> ret;
    string token;
    stringstream ss(str);
    while (getline(ss,token,delimiter)) {
        ret.push_back(token);
    }
    return ret;
}

unsigned short Utilities::getPiece(char next) {
    unsigned short ret;
    switch (next) {
        case 'P' : ret = WHITE | PAWN;
            break;
        case 'N' : ret = WHITE | KNIGHT;
            break;
        case 'B' : ret = WHITE | BISHOP;
            break;
        case 'R' : ret = WHITE | ROOK;
            break;
        case 'Q' : ret = WHITE | QUEEN;
            break;
        case 'K' : ret = WHITE | KING;
            break;
        case 'p' : ret = BLACK | PAWN;
            break;
        case 'n' : ret = BLACK | KNIGHT;
            break;
        case 'b' : ret = BLACK | BISHOP;
            break;
        case 'r' : ret = BLACK | ROOK;
            break;
        case 'q' : ret = BLACK | QUEEN;
            break;
        case 'k' : ret = BLACK | KING;
            break;
        default:
            ret = -1; // incorrect value given
    }

    return ret;
}

char Utilities::getPiece(unsigned short piece) {
    char ret;
    switch (piece) {
        case WHITE | PAWN: ret = 'P';
            break;
        case WHITE | KNIGHT: ret = 'N';
            break;
        case WHITE | BISHOP: ret = 'B';
            break;
        case WHITE | ROOK: ret = 'R';
            break;
        case WHITE | QUEEN: ret = 'Q';
            break;
        case WHITE | KING: ret = 'K';
            break;
        case BLACK | PAWN: ret = 'p';
            break;
        case BLACK | KNIGHT: ret = 'n';
            break;
        case BLACK | BISHOP: ret = 'b';
            break;
        case BLACK | ROOK: ret = 'r';
            break;
        case BLACK | QUEEN: ret = 'q';
            break;
        case BLACK | KING: ret = 'k';
            break;
        default : ret = '?';
    }

    return ret;
}

unsigned short Utilities::getSquare(string& square) {//a is 65, b is 66, ... h is 72
    char fileChar = square[0];
    char rankChar = square[1];
    unsigned short file = fileChar-65;//brings char to number with ASCII, where a is 0, b is 1, and so on
    unsigned short rank = rankChar-48;//brings char number to number with ASCII, where 0 is 48, 1 is 49, and so on
    return file+rank*8;
}

string Utilities::getSquare(short square) {
    int file = square%8;
    int rank = square/8;
    unsigned char fileChar = file+65;//same idea as above
    unsigned char rankChar = rank+48;
    return {static_cast<char>(fileChar), static_cast<char>(rankChar), '\0'};
}

void Utilities::printBitboard(const ull bitboard) {
    for (int row = 7; row >=0; row--) {
        for (int col = 0; col<8; col++) {
            const int square = row * 8 + col;
            const ull squareBB = toBitboard(square);

            if (squareBB & bitboard) {
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

unsigned long long Utilities::makeZobristKey
(const unsigned short chessboard [], const bool whiteToMove, const bool castlingRights [], const unsigned long long enPassantTargetSquare) {
    unsigned long long ret = 0;
    if (whiteToMove) ret = whiteToMoveRandom;

    for (int i=0; i<4; i++) {
        if (castlingRights[i])ret ^= castlingRightsRandom[i];
    }

    if (enPassantTargetSquare != 0)ret ^= enPassantFileRandom[enPassantTargetSquare%8];

    for (int square = 0; square < 64; square++) {
        if (chessboard[square] != EMPTY) {
            ret ^= pieceOnSquareRandom[chessboard[square]][square];
        }
    }
    return ret;
}

unsigned inline long long Utilities::getWhiteToMoveZobrist() {
    return whiteToMoveRandom;
}

unsigned inline long long Utilities::getEnPassantSquareZobrist(int square) {
    return enPassantFileRandom[square%8];
}

unsigned inline long long Utilities::getCastlingRightsZobrist(const bool *castlingRights) {
    unsigned long long ret = 0;
    if (castlingRights[0])ret ^= castlingRightsRandom[0];
    if (castlingRights[1])ret ^= castlingRightsRandom[1];
    if (castlingRights[2])ret ^= castlingRightsRandom[2];
    if (castlingRights[3])ret ^= castlingRightsRandom[3];
    return ret;
}

unsigned inline long long Utilities::getPieceSquareZobrist(int piece, int square) {
    return pieceOnSquareRandom[piece][square];
}

int Utilities::pop_lsb(uint64_t &bb) {
    int index = __builtin_ctzll(bb);
    bb &= bb - 1;
    return index;
}
