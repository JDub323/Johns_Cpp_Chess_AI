//
// Created by jwhal on 6/11/2024.
//

#ifndef JOHNS_CPP_CHESS_AI_CHESSCONSTANTS_H
#define JOHNS_CPP_CHESS_AI_CHESSCONSTANTS_H

using ull = unsigned long long;

namespace ChessConstants {
    static inline const std::string startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    static inline const short WHITE_CASTLE_SHORT = 0;
    static inline const short WHITE_CASTLE_LONG = 1;
    static inline const short BLACK_CASTLE_SHORT = 2;
    static inline const short BLACK_CASTLE_LONG = 3;



    static inline const ull a1=1;
    static inline const ull b1=a1 << 1;
    static inline const ull c1=b1 << 1;
    static inline const ull d1=c1 << 1;
    static inline const ull e1=d1 << 1;
    static inline const ull f1=e1 << 1;
    static inline const ull g1=f1 << 1;
    static inline const ull h1=g1 << 1;

    static inline const ull a2=h1 << 1;
    static inline const ull b2=a2 << 1;
    static inline const ull c2=b2 << 1;
    static inline const ull d2=c2 << 1;
    static inline const ull e2=d2 << 1;
    static inline const ull f2=e2 << 1;
    static inline const ull g2=f2 << 1;
    static inline const ull h2=g2 << 1;

    static inline const ull a3=h2 << 1;
    static inline const ull b3=a3 << 1;
    static inline const ull c3=b3 << 1;
    static inline const ull d3=c3 << 1;
    static inline const ull e3=d3 << 1;
    static inline const ull f3=e3 << 1;
    static inline const ull g3=f3 << 1;
    static inline const ull h3=g3 << 1;

    static inline const ull a4=h3 << 1;
    static inline const ull b4=a4 << 1;
    static inline const ull c4=b4 << 1;
    static inline const ull d4=c4 << 1;
    static inline const ull e4=d4 << 1;
    static inline const ull f4=e4 << 1;
    static inline const ull g4=f4 << 1;
    static inline const ull h4=g4 << 1;

    static inline const ull a5=h4 << 1;
    static inline const ull b5=a5 << 1;
    static inline const ull c5=b5 << 1;
    static inline const ull d5=c5 << 1;
    static inline const ull e5=d5 << 1;
    static inline const ull f5=e5 << 1;
    static inline const ull g5=f5 << 1;
    static inline const ull h5=g5 << 1;

    static inline const ull a6=h5 << 1;
    static inline const ull b6=a6 << 1;
    static inline const ull c6=b6 << 1;
    static inline const ull d6=c6 << 1;
    static inline const ull e6=d6 << 1;
    static inline const ull f6=e6 << 1;
    static inline const ull g6=f6 << 1;
    static inline const ull h6=g6 << 1;

    static inline const ull a7=h6 << 1;
    static inline const ull b7=a7 << 1;
    static inline const ull c7=b7 << 1;
    static inline const ull d7=c7 << 1;
    static inline const ull e7=d7 << 1;
    static inline const ull f7=e7 << 1;
    static inline const ull g7=f7 << 1;
    static inline const ull h7=g7 << 1;

    static inline const ull a8=h7 << 1;
    static inline const ull b8=a8 << 1;
    static inline const ull c8=b8 << 1;
    static inline const ull d8=c8 << 1;
    static inline const ull e8=d8 << 1;
    static inline const ull f8=e8 << 1;
    static inline const ull g8=f8 << 1;
    static inline const ull h8=g8 << 1;

    //files
    static inline const ull A_FILE= a1 | a2 | a3 | a4 | a5 | a6 | a7 | a8;
    static inline const ull B_FILE= b1 | b2 | b3 | b4 | b5 | b6 | b7 | b8;
    static inline const ull C_FILE= c1 | c2 | c3 | c4 | c5 | c6 | c7 | c8;
    static inline const ull D_FILE= d1 | d2 | d3 | d4 | d5 | d6 | d7 | d8;
    static inline const ull E_FILE= e1 | e2 | e3 | e4 | e5 | e6 | e7 | e8;
    static inline const ull F_FILE= f1 | f2 | f3 | f4 | f5 | f6 | f7 | f8;
    static inline const ull G_FILE= g1 | g2 | g3 | g4 | g5 | g6 | g7 | g8;
    static inline const ull H_FILE= h1 | h2 | h3 | h4 | h5 | h6 | h7 | h8;

    static inline const ull NOT_H_FILE=~H_FILE;
    static inline const ull NOT_A_FILE=~A_FILE;
    static inline const ull NOT_GH_FILE=~(H_FILE | G_FILE);
    static inline const ull NOT_AB_FILE=~(A_FILE | B_FILE);

    static inline const ull ALL_FILES[] = {
            A_FILE,
            B_FILE,
            C_FILE,
            D_FILE,
            E_FILE,
            F_FILE,
            G_FILE,
            H_FILE
    };
    static inline const ull ADJACENT_FILES[] = {
            B_FILE,
            A_FILE | C_FILE,
            B_FILE | D_FILE,
            C_FILE | E_FILE,
            D_FILE | F_FILE,
            E_FILE | G_FILE,
            F_FILE | H_FILE,
            G_FILE
    };

    //ranks
    static inline const ull RANK_1= a1 | b1 | c1 | d1 | e1 | f1 | g1 | h1;
    static inline const ull RANK_2= a2 | b2 | c2 | d2 | e2 | f2 | g2 | h2;
    static inline const ull RANK_3= a3 | b3 | c3 | d3 | e3 | f3 | g3 | h3;
    static inline const ull RANK_4= a4 | b4 | c4 | d4 | e4 | f4 | g4 | h4;
    static inline const ull RANK_5= a5 | b5 | c5 | d5 | e5 | f5 | g5 | h5;
    static inline const ull RANK_6= a6 | b6 | c6 | d6 | e6 | f6 | g6 | h6;
    static inline const ull RANK_7= a7 | b7 | c7 | d7 | e7 | f7 | g7 | h7;
    static inline const ull RANK_8= a8 | b8 | c8 | d8 | e8 | f8 | g8 | h8;

    static inline const ull ENEMY_SQUARES[] = {//0 is white, 1 is black
            RANK_8 | RANK_7 | RANK_6 | RANK_5,
            RANK_4 | RANK_3 | RANK_2 | RANK_1
    };

    //special
    static inline const ull LIGHT_SQUARES=0x55AA55AA55AA55AA;
    static inline const ull DARK_SQUARES= ~LIGHT_SQUARES;
    static inline const ull BISHOP_TL_EDGE= A_FILE | RANK_8;
    static inline const ull BISHOP_TR_EDGE= H_FILE | RANK_8;
    static inline const ull BISHOP_BL_EDGE= A_FILE | RANK_1;
    static inline const ull BISHOP_BR_EDGE= H_FILE | RANK_1;
    static inline const ull ALL_EDGES= A_FILE | RANK_8 | H_FILE | RANK_1;
    static inline const ull NOT_EDGES=~ALL_EDGES;
    static inline const ull CORNERS= a1 | h1 | a8 | h8;

    static inline const ull whiteCanCSPieceMask = g1 | f1;
    static inline const ull whiteCanCSCheckMask = e1 | g1 | f1;
    static inline const ull whiteCanCLPieceMask= b1 | c1 | d1;
    static inline const ull whiteCanCLCheckMask= c1 | d1 | e1;

    static inline const ull blackCanCSPieceMask= g8 | f8;
    static inline const ull blackCanCSCheckMask= e8 | g8 | f8;
    static inline const ull blackCanCLPieceMask= b8 | c8 | d8;
    static inline const ull blackCanCLCheckMask= c8 | d8 | e8;

    static inline const ull CASTLING_TARGET_SQUARES [4] = {g1,c1,g8,c8};
}

#endif //JOHNS_CPP_CHESS_AI_CHESSCONSTANTS_H
