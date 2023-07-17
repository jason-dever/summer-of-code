#include "position.h"
#include "precompute.h"
#include "test.h"

U64 relevantOccupancyRook(int sq) {
    U64 pos;
    zeroAndSetBit(pos, sq);

    U64 mask = 0;

    int col = sq % 8;
    int row = sq / 8;

    for (int i = 1; col-i > 0; i++) {
        mask |= shiftRight(pos)*i;
    }
    for (int i = 1; i+col < 7; i++) {
        mask |= shiftLeft(pos)*i;
    }

    for (int i = 1; i+row < 7; i++) {
        mask |= shiftUp(pos)*i;
    }
    for (int i = 1; row-i > 0; i++) {
        mask |= shiftDown(pos)*i;
    }
    return mask;
}

U64 relevantOccupancyBishop(int sq) {
    U64 pos;
    zeroAndSetBit(pos, sq);

    U64 mask = 0;

    int col = sq % 8;
    int row = sq / 8;

    for (int i = 1; col-i > 0 && i+row < 7; i++) {
        mask |= shiftUpRight(pos)*i;
    }
    for (int i = 1; i+col < 7 && i+row < 7; i++) {
        mask |= shiftUpLeft(pos)*i;
    }

    for (int i = 1; col-i > 0 && row-i > 0; i++) {
        mask |= shiftDownRight(pos)*i;
    }
    for (int i = 1; i+col < 7 && row-i > 0; i++) {
        mask |= shiftDownLeft(pos)*i;
    }
    return mask;
}

U64 moveboardRook(int sq, U64 blocker_mask) {
    U64 pos = 1ULL << sq;
    U64 moveboard = 0;

    int col = sq % 8;
    int row = sq / 8;

    for (int i = 1; ( (shiftRight(pos)*i) & shiftRight(blocker_mask) ) == 0 && col-i >= 0; i++) {
        moveboard |= shiftRight(pos)*i;
    }
    for (int i = 1; ( shiftLeft(pos)*i & shiftLeft(blocker_mask) ) == 0 && col+i <= 7; i++) {
        moveboard |= shiftLeft(pos)*i;
    }

    for (int i = 1; ( shiftUp(pos)*i & shiftUp(blocker_mask) ) == 0 && row+i <= 7; i++) {
            moveboard |= shiftUp(pos)*i;
    }
    for (int i = 1; ( shiftDown(pos)*i & shiftDown(blocker_mask) ) == 0 && row-i >= 0; i++) {
        moveboard |= shiftDown(pos)*i;
    }
    return moveboard;
}

U64 moveboardBishop(int sq, U64 blocker_mask) {
    U64 pos = 1ULL << sq;
    U64 moveboard = 0;

    int col = sq % 8;
    int row = sq / 8;

    for (int i = 1; ( shiftUpLeft(pos)*i & shiftUpLeft(blocker_mask) ) == 0 && col+i <= 7 && row+i <= 7; i++) {
        moveboard |= shiftUpLeft(pos)*i;
    } 
    for (int i = 1; ( shiftUpRight(pos)*i & shiftUpRight(blocker_mask) ) == 0  && col-i >= 0 && row+i <= 7; i++) {
        moveboard |= shiftUpRight(pos)*i;
    }

    for (int i = 1; ( shiftDownRight(pos)*i & shiftDownRight(blocker_mask) ) == 0 && col-i >= 0 && row-i >= 0; i++) {
        moveboard |= shiftDownRight(pos)*i;
    }
    for (int i = 1; ( shiftDownLeft(pos)*i & shiftDownLeft(blocker_mask) ) == 0  && col+i <= 7 && row-i >= 0; i++) {
        moveboard |= shiftDownLeft(pos)*i;
    }
    return moveboard;
}