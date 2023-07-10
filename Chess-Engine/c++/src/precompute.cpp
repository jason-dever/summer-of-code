#include "precompute.h"
#include "board.h"
#include "position.h"
#include "movegen.h"
#include "test.h"
#include <cmath>

U64 unplaced = UINT64_MAX;

U64 rook_moveboards[102400] {unplaced};
U64 bishop_moveboards[5248] {unplaced};

U64 rook_relevant_occupancy[64];
U64 bishop_relevant_occupancy[64];

int rook_index_offsets[64];
int bishop_index_offsets[64];

int rook_shifts[64];
int bishop_shifts[64];

U64 knight_moveboards[64];
U64 pawn_moveboards[2][64];

std::mt19937_64 mt(time(nullptr));

void initLookupTables() {
    initRelevantOccupancyLookups();
    initOffsetLookups();
    initShiftLookups();

    initRookLookups();
    initKnightLookups();
    initPawnLookups();
    initBishopLookups();
}

void initRelevantOccupancyLookups() {
    for (int i = 0; i <= 63; i++) {
        rook_relevant_occupancy[i] = relevantOccupancyRook(i);
        bishop_relevant_occupancy[i] = relevantOccupancyBishop(i);
    }
}

void initOffsetLookups() {
    int rook_offset = 0;
    int bishop_offset = 0;

    for (int i = 0; i <= 63; i++) {
        rook_index_offsets[i] = rook_offset;
        bishop_index_offsets[i] = bishop_offset;

        rook_offset += pow( 2.0, __popcnt64(rook_relevant_occupancy[i]) );
        bishop_offset += pow( 2.0, __popcnt64(bishop_relevant_occupancy[i]) );
    }
}

void initShiftLookups() {
    for (int i = 0; i <= 63; i++) {
        rook_shifts[i] = 64-__popcnt64(rook_relevant_occupancy[i]);
        bishop_shifts[i] = 64-__popcnt64(bishop_relevant_occupancy[i]);
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
        pawn_moveboards[captures][sq] = takes;
        pawn_moveboards[pushes][sq] = moves;
    }
}

int index_shift;

void initRookLookups() {
    // This array needs to be allocated on the heap to avoid stack overflow.
    U64* rook_blocker_combos = new U64[102400];

    U64 moves;
    U64 index;

    for (int sq = 0; sq <= 63; sq++) {
        storeAllRookBlockerCombos(rook_relevant_occupancy[sq], 0, rook_blocker_combos, sq, 0);
        index_shift = 0;
    }
    delete rook_blocker_combos;
    rook_blocker_combos = nullptr;
}

void initBishopLookups() {
    
}

U64 computeRookMagic(int sq, const U64* blocker_tbl) {
    U64 candidate_magic;
    U64 blocker_board;

    int offset = rook_index_offsets[sq];
    int magical_index;

    while (true) {
        candidate_magic = mt();

        for (int i = 0; true; i++) {
            blocker_board = blocker_tbl[offset+i];

            magical_index = (candidate_magic*blocker_board) >> rook_shifts[sq];

            if (rook_moveboards[offset+magical_index] == unplaced) {
                rook_moveboards[offset+magical_index] = moveboardRook(sq, blocker_board);
            }
            else { // Collision
                if (rook_moveboards[offset+magical_index] != moveboardRook(sq, blocker_board)) {
                    resetArraySegment(rook_moveboards, offset, rook_index_offsets[sq+1]-1);
                    break;
                }
            }

            if ( i == pow( 2.0, __popcnt64(rook_relevant_occupancy[sq])-1 ) ) {
                return candidate_magic;
            }

        }
    }
}

inline void resetArraySegment(U64* arr, int start, int end) {
    for (int i = start; i <= end; i++) {
        arr[i] = unplaced;
    }
}