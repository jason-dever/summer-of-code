#pragma once


#include <string>
#include <iostream>
#include <immintrin.h>

#define U64 unsigned long long

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

enum PawnMovesEnum {captures, pushes};

extern U64 rook_moveboards[102400];
extern U64 bishop_moveboards[5248];
extern U64 knight_moveboards[64];
extern U64 pawn_moveboards[2][2][64];
extern U64 king_moveboards[64];

extern U64 rook_relevant_occupancy[64];
extern U64 bishop_relevant_occupancy[64];

extern int rook_offsets[64];
extern int bishop_offsets[64];

void initLookupTables();

void initRookLookups();
void initBishopLookups();
void initKnightLookups();
void initPawnLookups();
void initKingLookups();

void __attribute__((constructor)) initLookupTables();