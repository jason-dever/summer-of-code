#pragma once
#include "board.h"
#include "precompute.h"

uint64_t relevantOccupancyRook(uint_fast8_t sq);
uint64_t relevantOccupancyBishop(uint_fast8_t sq);

uint64_t moveboardRook(uint_fast8_t sq, uint64_t blocker_mask);
uint64_t moveboardBishop(uint_fast8_t sq, uint64_t blocker_mask);