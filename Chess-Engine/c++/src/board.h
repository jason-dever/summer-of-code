#pragma once

#include <string>
#include "precompute.h"

enum PiecesEnum {pawns, knights, bishops, rooks, queens, king};
enum ColoursEnum {white, black};

struct Board {
    bool turn;

    U64 pieces[2][6] {0};

    U64 castle_squares = 0;
    U64 en_passant_squares = 0;

    int half_moves = 0;
    int full_moves = 0;

    void storeFEN(const std::string FEN);
    
    U64 whitePieces();
    U64 blackPieces();
    U64 sidePieces(bool side);

    inline void genMoves();
    
    private:
        inline void genKnightMoves();
        inline void genKingMoves();
        inline void genPawnMoves();
        inline void genBishopMoves();
        inline void genRookMoves();
        inline void genQueenMoves();

        inline U64 getAttackedSquares(bool opponent);
};