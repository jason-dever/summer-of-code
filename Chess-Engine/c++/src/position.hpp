#pragma once

#include "board.hpp"
#include "precompute.hpp"

uint64_t relevantOccupancyRook(int_fast8_t sq);
uint64_t relevantOccupancyBishop(int_fast8_t sq);

uint64_t moveboardRook(int_fast8_t sq, uint64_t blocker_mask);
uint64_t moveboardBishop(int_fast8_t sq, uint64_t blocker_mask);