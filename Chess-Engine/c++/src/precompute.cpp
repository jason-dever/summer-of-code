#include "precompute.h"
#include "board.h"
#include "position.h"
#include "movegen.h"
#include "test.h"
#include <cmath>
#include <immintrin.h>

U64 knight_moveboards[64];
U64 pawn_moveboards[2][2][64];
U64 rook_moveboards[102400];
U64 bishop_moveboards[5248];

U64 rook_relevant_occupancy[64];
U64 bishop_relevant_occupancy[64];

int rook_offsets[64];
int bishop_offsets[64];

void initLookupTables() {
    initRookLookups();
    initBishopLookups();
    initKnightLookups();
    initPawnLookups();
}

void initRookLookups() {
    int offset = 0;
    U64 subset = 0;

    for (int sq = 0; sq <= 63; sq++) {
        rook_relevant_occupancy[sq] = relevantOccupancyRook(sq);
        rook_offsets[sq] = offset;

        // Carry-Rippler trick enumerates all subsets of rook_relevant_occupancy[sq]
        // and populates rook_moveboards.
        do {
            rook_moveboards[offset + _pext_u64(subset, rook_relevant_occupancy[sq])] = moveboardRook(sq, subset);
            subset = (subset - rook_relevant_occupancy[sq]) & rook_relevant_occupancy[sq];
        } while (subset);

        offset += pow ( 2.0, __popcnt64(rook_relevant_occupancy[sq]) );
    }
}

void initBishopLookups() {
    int offset = 0;
    U64 subset = 0;

    for (int sq = 0; sq <= 63; sq++) {
        bishop_relevant_occupancy[sq] = relevantOccupancyBishop(sq);
        bishop_offsets[sq] = offset;

        // Same trick as in initRookLookups().
        do {
            bishop_moveboards[offset + _pext_u64(subset, bishop_relevant_occupancy[sq])] = moveboardBishop(sq, subset);
            subset = (subset - bishop_relevant_occupancy[sq]) & bishop_relevant_occupancy[sq];
        } while (subset);

        offset += pow ( 2.0, __popcnt64(bishop_relevant_occupancy[sq]) );
    }
}

void initKnightLookups() {
    U64 pos;
    int col;
    U64 moves; 

    for (int sq = 0; sq <= 63; sq++) {
        moves = 0;
        zeroAndSetBit(pos, sq);
        col = (sq % 8);

        if (col < 7) {
            moves |= (pos << 17);
            moves |= (pos >> 15);
        
            if (col < 6) {
                moves |= (pos << 10);
                moves |= (pos >> 6);
            }
        }
        if (col > 0) {
            moves |= (pos >> 17);
            moves |= (pos << 15);

            if (col > 1) {
                moves |= (pos >> 10);
                moves |= (pos << 6);
            }
        }
        knight_moveboards[sq] = moves;
    }
}

void initPawnLookups() {
   /* When using pawn lookup tables, you will need to rotate the square by indexing the array with
   63-sq and rotate the output. */

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
        if (sq <= 15 && sq >= 8) moves |= shiftUp(pos)*2;
        if (col > 0) takes |= shiftUpRight(pos);
        if (col < 7) takes |= shiftUpLeft(pos); 
        
        pawn_moveboards[white][captures][sq] = takes;
        pawn_moveboards[white][pushes][sq] = moves;
    }

    for (int sq = 55; sq >= 8; sq--) {
        moves = 0;
        takes = 0;
        col = (sq % 8);
        zeroAndSetBit(pos, sq);

        moves |= shiftDown(pos);
        if (sq <= 55 && sq >= 48) moves |= shiftDown(pos)*2;
        if (col > 0) takes |= shiftDownRight(pos);
        if (col < 7) takes |= shiftDownLeft(pos);
        
        pawn_moveboards[black][captures][sq] = takes;
        pawn_moveboards[black][pushes][sq] = moves;
    }
}