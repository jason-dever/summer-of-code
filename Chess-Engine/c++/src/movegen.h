#pragma once
#include "precompute.h"

enum PawnMoves {captures, pushes};

U64 genKnightMoves(int sq);
U64 genKingMoves(int sq);
U64 genPawnMoves(int sq);
U64 genBishopMoves(int sq);
U64 genRookMoves(int sq);
U64 genQueenMoves(int sq);

void genMoves(int sq);