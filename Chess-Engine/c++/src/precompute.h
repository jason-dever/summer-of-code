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

struct Magic {
    U64 mask;
    U64 magic;
};

void initLookupTables();

void initKnightLookups();
void initPawnLookups();
void initRookLookups();
void initBishopLookups();

void initRookMagic(int sq, const U64* blocker_tbl, U64 final_move_tbl[4096]);

void resetArray(bool arr[], int size);

void __attribute__((constructor)) initLookupTables();