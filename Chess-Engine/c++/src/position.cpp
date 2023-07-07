#include "position.h"
#include "precompute.h"
#include "test.h"
#include <iostream>

U64 blockerMaskRook(int sq) {
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

U64 blockerMaskBishop(int sq) {
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

U64 blockerboard(Board board, U64 blocker_mask) {
    return blocker_mask & (board.whitePieces() | board.blackPieces());
}

U64 moveboardRook(int sq, U64 blockerboard) {
    U64 pos = 1ULL << sq;
    U64 moveboard = 0;

    int col = sq % 8;
    int row = sq / 8;

    for (int i = 1; ( (shiftRight(pos)*i) & shiftRight(blockerboard) ) == 0 && col-i >= 0; i++) {
        moveboard |= shiftRight(pos)*i;
    }
    for (int i = 1; ( shiftLeft(pos)*i & shiftLeft(blockerboard) ) == 0 && col+i <= 7; i++) {
        moveboard |= shiftLeft(pos)*i;
    }

    for (int i = 1; ( shiftUp(pos)*i & shiftUp(blockerboard) ) == 0 && row+i <= 7; i++) {
            moveboard |= shiftUp(pos)*i;
    }
    for (int i = 1; ( shiftDown(pos)*i & shiftDown(blockerboard) ) == 0 && row-i >= 0; i++) {
        moveboard |= shiftDown(pos)*i;
    }
    return moveboard;
}

U64 moveboardBishop(int sq, U64 blockerboard) {
    U64 pos = 1ULL << sq;
    U64 moveboard = 0;

    int col = sq % 8;
    int row = sq / 8;

    for (int i = 1; ( shiftUpLeft(pos)*i & shiftUpLeft(blockerboard) ) == 0 && col+i <= 7 && row+i <= 7; i++) {
        moveboard |= shiftUpLeft(pos)*i;
    } 
    for (int i = 1; ( shiftUpRight(pos)*i & shiftUpRight(blockerboard) ) == 0  && col-i >= 0 && row+i <= 7; i++) {
        moveboard |= shiftUpRight(pos)*i;
    }

    for (int i = 1; ( shiftDownRight(pos)*i & shiftDownRight(blockerboard) ) == 0 && col-i >= 0 && row-i >= 0; i++) {
        moveboard |= shiftDownRight(pos)*i;
    }
    for (int i = 1; ( shiftDownLeft(pos)*i & shiftDownLeft(blockerboard) ) == 0  && col+i <= 7 && row-i >= 0; i++) {
        moveboard |= shiftDownLeft(pos)*i;
    }
    return moveboard;
}

extern int index_shift;

void storeAllRookBlockerCombos(U64 relevant_occupancy, U64 blocker_mask, U64* blocker_table, int sq, int depth) {
    int max_depth = __popcnt64(blocker_mask)-1;
    int current_bit_index = getNthSetBitIndex(blocker_mask, depth);
    
    if (depth == max_depth) {
        blocker_table[4096*sq + index_shift] = moveboardRook(sq, blocker_mask);
        return;
    }

    storeAllRookBlockerCombos(relevant_occupancy, blocker_mask, blocker_table, sq, depth+1);
    flipBit(blocker_mask, current_bit_index);
    storeAllRookBlockerCombos(relevant_occupancy, blocker_mask, blocker_table, sq, depth+1);
}

void storeAllBishopBlockerCombos(U64 blocker_mask, U64 blocker_board, U64* blocker_table, int sq, int address_shift, int depth) {
    
}

int getNthSetBitIndex(U64 mask, int n) {
    unsigned long index;
    int count = 0;

    while (mask) {
        _BitScanForward64(&index, mask);

        if (count == n) {
            return index;
        }
        count++;
        resetBit(mask, index);
    }
    std::cerr << "nth set bit doesn't exist\n" << n << "\n";
    return -1;
}