//
// Created by jwhal on 1/19/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_POSITION_H
#define JOHNS_CPP_CHESS_AI_POSITION_H

#include <string>

class Position {
private:
    struct Move {
        unsigned char fromSquare;
        unsigned char toSquare;
        unsigned char capturedPiece;
        unsigned char moveType;
    };
    unsigned long pieceBitboards [15];
    unsigned short chessBoard [64];
    bool castlingRights [4];
    unsigned short halfmoveClock;
    unsigned long enPassantTargetSquare;
    bool whiteToMove;
    unsigned short plyNumber;
    unsigned short gameState;
    unsigned long zobristKey;
    Move legalMoves [];

public:
    explicit Position(std::string fen);
    void makeMove();
    void unmakeMove();
    void generateMoves();
    void generateCapturingMoves();
    void calculatePrecalculatedData();
    unsigned short *getChessBoard(){ return chessBoard;}
};


#endif //JOHNS_CPP_CHESS_AI_POSITION_H
