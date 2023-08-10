#pragma once

#include "precompute.h"
#include "board.h"

void printBitboard(uint64_t bitboard);

class Test {
    void makeUnmakeTest();
    void perft();
};