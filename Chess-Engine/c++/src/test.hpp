#pragma once

#include "precompute.hpp"
#include "board.hpp"

void printBitboard(uint64_t bitboard);

class Test {
    void makeUnmakeTest();
    void perft();
};