#pragma once

#define U64 unsigned long long

#include <string>
#include <iostream>
#include <intrin.h>
#include <random>
#include <ctime>

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

extern U64 unplaced;

extern U64 rook_moveboards[102400];
extern U64 bishop_moveboards[5248];

extern U64 rook_relevant_occupancy[64];
extern U64 bishop_relevant_occupancy[64];

extern int rook_index_offsets[64];
extern int bishop_index_offsets[64];

extern U64 knight_moveboards[64];
extern U64 pawn_moveboards[2][64];

void initLookupTables();

void initKnightLookups();
void initPawnLookups();
void initRookLookups();
void initBishopLookups();

void initRelevantOccupancyLookups();
void initOffsetLookups();

void initRookMagic(int sq, const U64* blocker_tbl, U64* final_move_tbl);

// void __attribute__((constructor)) initLookupTables();