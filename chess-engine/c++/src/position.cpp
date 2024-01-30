#include "position.hpp"
#include "precompute.hpp"
#include "test.hpp"
#include <cstdint>
#include <iostream>

uint64_t relevantOccupancyRook(int_fast8_t sq) {
    uint64_t pos;
    zeroAndSetBit(pos, sq);

    uint64_t mask = 0;

    int_fast8_t col = sq % 8;
    int_fast8_t row = sq / 8;

    for (int_fast8_t i = 1; col-i > 0; i++) {
        mask |= shiftRight(pos)*i;
    }
    for (int_fast8_t i = 1; i+col < 7; i++) {
        mask |= shiftLeft(pos)*i;
    }

    for (int_fast8_t i = 1; i+row < 7; i++) {
        mask |= shiftUp(pos)*i;
    }
    for (int_fast8_t i = 1; row-i > 0; i++) {
        mask |= shiftDown(pos)*i;
    }
    return mask;
}

uint64_t relevantOccupancyBishop(int_fast8_t sq) {
    uint64_t pos;
    zeroAndSetBit(pos, sq);

    uint64_t mask = 0;

    int_fast8_t col = sq % 8;
    int_fast8_t row = sq / 8;

    for (int_fast8_t i = 1; col-i > 0 && i+row < 7; i++) {
        mask |= shiftUpRight(pos)*i;
    }
    for (int_fast8_t i = 1; i+col < 7 && i+row < 7; i++) {
        mask |= shiftUpLeft(pos)*i;
    }

    for (int_fast8_t i = 1; col-i > 0 && row-i > 0; i++) {
        mask |= shiftDownRight(pos)*i;
    }
    for (int_fast8_t i = 1; i+col < 7 && row-i > 0; i++) {
        mask |= shiftDownLeft(pos)*i;
    }
    return mask;
}

uint64_t moveboardRook(int_fast8_t sq, uint64_t blocker_mask) {
    uint64_t pos = 1ULL << sq;
    uint64_t moveboard = 0;

    int_fast8_t col = sq % 8;
    int_fast8_t row = sq / 8;

    for (int_fast8_t i = 1; ( (shiftRight(pos)*i) & shiftRight(blocker_mask) )  == 0 && col-i >= 0; i++) {
        moveboard |= pos >> i;
    }
    for (int_fast8_t i = 1; ( (shiftLeft(pos)*i) & shiftLeft(blocker_mask) ) == 0 && col+i <= 7; i++) {
        moveboard |= pos << i;
    }

    for (int_fast8_t i = 1; ( (shiftUp(pos)*i) & shiftUp(blocker_mask) ) == 0 && row+i <= 7; i++) {
            moveboard |= shiftUp(pos)*i;
    }
    for (int_fast8_t i = 1; ( shiftDown(pos)*i & shiftDown(blocker_mask) ) == 0 && row-i >= 0; i++) {
        moveboard |= shiftDown(pos)*i;
    }
    return moveboard;
}

uint64_t moveboardBishop(int_fast8_t sq, uint64_t blocker_mask) {
    uint64_t pos = 1ULL << sq;
    uint64_t moveboard = 0;

    int_fast8_t col = sq % 8;
    int_fast8_t row = sq / 8;

    for (int_fast8_t i = 1; ( shiftUpLeft(pos)*i & shiftUpLeft(blocker_mask) ) == 0 && col+i <= 7 && row+i <= 7; i++) {
        moveboard |= shiftUpLeft(pos)*i;
    } 
    for (int_fast8_t i = 1; ( shiftUpRight(pos)*i & shiftUpRight(blocker_mask) ) == 0  && col-i >= 0 && row+i <= 7; i++) {
        moveboard |= shiftUpRight(pos)*i;
    }

    for (int_fast8_t i = 1; ( shiftDownRight(pos)*i & shiftDownRight(blocker_mask) ) == 0 && col-i >= 0 && row-i >= 0; i++) {
        moveboard |= shiftDownRight(pos)*i;
    }
    for (int_fast8_t i = 1; ( shiftDownLeft(pos)*i & shiftDownLeft(blocker_mask) ) == 0  && col+i <= 7 && row-i >= 0; i++) {
        moveboard |= shiftDownLeft(pos)*i;
    }
    return moveboard;
}