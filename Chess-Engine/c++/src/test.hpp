#pragma once

#include "precompute.hpp"
#include "board.hpp"

void printBitboard(uint64_t bitboard);

namespace Test {
    void makeUnmake();

    void testQuietMoves();
    void testMakeQuietMoves();
    void testUnmakeQuietMoves();
    
    void captures();
    
    void castling();

    void enPassant();
    void pawnPromotions();
    void pawnPushes();

    void perft();
}