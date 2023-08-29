#pragma once

#include "board.hpp"
#include "precompute.hpp"

uint64_t relevantOccupancyRook(int_fast8_t);
uint64_t relevantOccupancyBishop(int_fast8_t);

uint64_t moveboardRook(int_fast8_t, uint64_t);
uint64_t moveboardBishop(int_fast8_t, uint64_t);