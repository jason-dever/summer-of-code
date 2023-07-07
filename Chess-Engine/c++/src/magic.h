#include "precompute.h"

/* I opted for preprogrammed magic numbers found by Dr. Kannan and decided not to use the
code I wrote to find magic numbers because if values are known at compile time, the 
compiler can make optimizations with the multiplication and shift right operations.
On top of that, startup is faster than if I feed in random mersenne twister numbers
and brute force my own magics. This also introduces consistent performance;
if I randomly generate magics at runtime, there is going to be
variance in the quality of magic numbers and therefore sliding piece movegen
is going to be a slightly different speed each time the program runs. */

extern U64 rook_magics[64];
extern U64 bishop_magics[64];

extern U64 rook_shifts[64];
extern U64 bishop_shifts[64];

extern U64 rook_relevant_occupancy[64];
extern U64 bishop_relevant_occupancy[64];
