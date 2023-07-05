#include "precompute.h"
#include "board.h"
#include "position.h"
#include "movegen.h"
#include "test.h"
#include <iostream>
U64 rook_moveboards[64][4096] {0xFFFFFFFF};
U64 bishop_moveboards[64][512] {0};
U64 knight_moveboards[64] {0};
U64 pawn_moveboards[2][48] {0};

Magic magic_rook_tbl[64];
Magic magic_bishop_tbl[64];

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
    // Array has to be to be allocated on the heap to avoid stack overflow.
    U64* rook_blocker_combos = new U64[64*4096];  

    for (int sq = 0; sq <= 63; sq++) {
        magic_rook_tbl[sq].mask = blockerMaskRook(sq);
        storeAllRookBlockerCombos(magic_rook_tbl[sq].mask, 0, rook_blocker_combos, sq, 0, 0);
        initRookMagic(sq, rook_blocker_combos, rook_moveboards[sq]);
    }
    delete rook_blocker_combos;
    rook_blocker_combos = nullptr;
}

void initBishopLookups() {
    U64 b;
}

void initRookMagic(int sq, const U64* blocker_tbl, U64 final_move_tbl[4096]) {
    bool has_been_placed[4096] {false};

    U64 candidate_magic;
    bool magic_found = false;
    int index;

    while (!magic_found) {
        candidate_magic = mt();
        
        for (int i = 0; i <= 4096; i++) {
            index = (candidate_magic*blocker_tbl[i] >> (64 - __popcnt64(magic_rook_tbl[sq].mask)));
            
            if (has_been_placed[i]) {
                if (final_move_tbl[index] != moveboardRook(sq, blocker_tbl[i])) {
                    resetArray(has_been_placed, 4096);
                    break;
                }
                else {
                    continue;
                }
            }
            final_move_tbl[index] = moveboardRook(sq, blocker_tbl[i]);
            has_been_placed[index] = true;

            if (i >= (2^__popcnt64(magic_rook_tbl[sq].mask))) {
                magic_found = true;
                break;
            }
        }
    }
    magic_rook_tbl[sq].magic = candidate_magic;
}

void resetArray(bool arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[size] = 0;
    }
}