#include "precompute.h"
#include "board.h"
#include "position.h"
#include "movegen.h"
#include "test.h"
#include "magic.h"
#include <cmath>

U64 rook_moveboards[64][4096];
U64 bishop_moveboards[64][512];
U64 knight_moveboards[64];
U64 pawn_moveboards[2][48];

std::mt19937_64 mt(time(nullptr));

void initLookupTables() {
    initRookLookups();
    initKnightLookups();
    initPawnLookups();
    initBishopLookups();
}

void initKnightLookups() {
    U64 pos;
    int col;
    U64 moves; 

    for (int sq = 0; sq <= 63; sq++) {
        moves = 0;
        zeroAndSetBit(pos, sq);
        col = (sq % 8);

        if (col > 0) {
            moves |= (pos << 17);
            moves |= (pos >> 15);
        
            if (col > 1) {
                moves |= (pos << 10);
                moves |= (pos >> 6);
            }
        }
        if (col < 7) {
            moves |= (pos >> 17);
            moves |= (pos << 15);

            if (col < 6) {
                moves |= (pos >> 10);
                moves |= (pos << 6);
            }
        }
        knight_moveboards[sq] = moves;
    }
}

void initPawnLookups() {
    /* When using the pawn lookup table you'll need to subtract the square by 8
    because even though arrays start at 0, pawns do not. */
   // TODO: figure out how to rotate the board square for black pawns

    U64 pos;
    int col;
    U64 moves;
    U64 takes;

    for (int sq = 8; sq <= 55; sq++) {
        moves = 0;
        takes = 0;
        col = (sq % 8);
        zeroAndSetBit(pos, sq);

        moves |= shiftUp(pos);

        if (sq <= 15 && sq >= 8) {
            moves |= shiftUp(pos)*2;
        }

        if (col > 0) {
            takes |= shiftUpLeft(pos);
        }
        if (col < 7) {
            takes |= shiftUpRight(pos); 
        }
        pawn_moveboards[captures][sq-8] = takes;
        pawn_moveboards[pushes][sq-8] = moves;
    }
}

void initRookLookups() {
    // This array needs to be allocated on the heap to avoid stack overflow.
    U64* rook_blocker_combos = new U64[64*4096];

    U64 moves;
    U64 index;

    for (int sq = 0; sq <= 63; sq++) {
        for (int i = 0; i < pow(2.0, __popcnt64(rook_relevant_occupancy[sq])); i++) {
            storeAllRookBlockerCombos(rook_relevant_occupancy[sq], 0, rook_blocker_combos, sq, 0);
            moves = moveboardRook(sq, rook_blocker_combos[4096*sq + i]);
            index = (moves * rook_magics[sq]) >> rook_shifts[sq];
            rook_moveboards[sq][index] = moves;
        }
    }
    delete rook_blocker_combos;
    rook_blocker_combos = nullptr;
}

void initBishopLookups() {
    
}