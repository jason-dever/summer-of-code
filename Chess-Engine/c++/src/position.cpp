#include "position.h"
#include "precompute.h"
#include "test.h"
#include <iostream>

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

U64 blockerMask(Board board, U64 relevant_occupancy) {
    return relevant_occupancy & (board.whitePieces() | board.blackPieces());
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

extern int index_shift;

void storeAllRookBlockerCombos(U64* blocker_table) {
    U64 blocker_mask;
    U64 relevant_occupancy;
    int count;

    for (int sq = 0; sq <= 63; sq++) {
        blocker_mask = 0;
        count = 0;
        relevant_occupancy = relevantOccupancyRook(sq);

        // Uses the carry rippler effect to traverse all subsets of relevant_occupancy
        do {
            blocker_table[rook_index_offsets[sq]+count] = blocker_mask;
            blocker_mask = (blocker_mask - relevant_occupancy) & relevant_occupancy;
            count++;
        } while (blocker_mask);
    }
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
    std::cerr << n << "th set bit doesn't exist\n";
    printBitboard(mask);
    return -1;
}