#pragma once

#include <string>
#include "precompute.h"

enum PiecesEnum {pawns, knights, bishops, rooks, queens, king};

struct Board {
    bool black_turn;

    U64 w_pieces[6] {0};
    U64 b_pieces[6] {0};

    U64 castle_squares = 0;
    U64 en_passant_squares = 0;

    int half_moves = 0;
    int full_moves = 0;

    void storeFEN(const std::string FEN); 
    
    U64 whitePieces();
    U64 blackPieces();
};