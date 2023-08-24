#pragma once

#include "precompute.hpp"
#include "board.hpp"

void printBitboard(uint64_t bitboard);

void testMakeUnmake();

void testQuietMoves();
void testMakeQuietMoves();
void testUnmakeQuietMoves();

void testCaptures();

void testCastling();

void enPassant();
void pawnPromotions();
void pawnPushes();

void perft();
