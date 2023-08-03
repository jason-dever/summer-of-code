#pragma once

#include <string>
#include <iostream>
#include <immintrin.h>
#include <cstdint>

#define setBit(mask, index) mask |= 1ULL << index
#define zeroAndSetBit(mask, index) mask = 1ULL << index
#define resetBit(mask, index) mask &= ~(1ULL << index)
#define flipBit(mask, index) mask ^= 1ULL << index

#define shiftUp(x) x << 8
#define shiftDown(x) x >> 8
#define shiftLeft(x) x << 1
#define shiftRight(x) x >> 1

#define shiftUpLeft(x) x << 9
#define shiftUpRight(x) x << 7
#define shiftDownLeft(x) x >> 7
#define shiftDownRight(x) x >> 9

#define rookMoves(sq, occupancy) rook_moveboards[_pext_u64(occupancy, rook_relevant_occupancy[sq]) + rook_offsets[sq]]
#define bishopMoves(sq, occupancy) bishop_moveboards[_pext_u64(occupancy, bishop_relevant_occupancy[sq]) + bishop_offsets[sq]]

enum PawnMovesEnum {captures, pushes};

extern uint64_t rook_moveboards[102400];
extern uint64_t bishop_moveboards[5248];
extern uint64_t knight_moveboards[64];
extern uint64_t pawn_moveboards[2][2][64];
extern uint64_t king_moveboards[64];

extern uint64_t rook_relevant_occupancy[64];
extern uint64_t bishop_relevant_occupancy[64];

extern uint_fast16_t rook_offsets[64];
extern uint_fast16_t bishop_offsets[64];

void initLookupTables();

void initRookLookups();
void initBishopLookups();
void initKnightLookups();
void initPawnLookups();
void initKingLookups();

void __attribute__((constructor)) initLookupTables();