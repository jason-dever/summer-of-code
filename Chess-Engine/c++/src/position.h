#pragma once
#include "board.h"
#include "precompute.h"

U64 blockerMaskRook(int sq);
U64 blockerMaskBishop(int sq);

U64 blockerboard(Board board, U64 blocker_mask);

U64 moveboardRook(int sq, U64 blockerboard);
U64 moveboardBishop(int sq, U64 blockerboard);

void storeAllRookBlockerCombos(U64 blocker_mask, U64 blocker_board, U64 move_table[4096], int sq, int address_shift, int depth);
void storeAllBishopMoveCombos(U64 blocker_mask, U64 blocker_board, U64 move_table[512], int sq, int address_shift, int depth);
int getNthSetBitIndex(U64 mask, int n);