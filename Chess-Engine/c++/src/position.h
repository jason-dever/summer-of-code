#pragma once
#include "board.h"
#include "precompute.h"

extern int index_shift;

U64 relevantOccupancyRook(int sq);
U64 relevantOccupancyBishop(int sq);

U64 blockerMask(Board board, U64 relevant_occupancy);

U64 moveboardRook(int sq, U64 blocker_mask);
U64 moveboardBishop(int sq, U64 blocker_mask);

void storeAllRookBlockerCombos(U64 blocker_mask, U64* blocker_table, int sq, int depth);
void storeAllBishopMoveCombos(U64 blocker_mask, U64 blocker_board, U64 move_table[512], int sq, int address_shift, int depth);

int getNthSetBitIndex(U64 mask, int n);