#pragma once
#include "board.h"
#include "precompute.h"

U64 relevantOccupancyRook(int sq);
U64 relevantOccupancyBishop(int sq);

U64 moveboardRook(int sq, U64 blocker_mask);
U64 moveboardBishop(int sq, U64 blocker_mask);