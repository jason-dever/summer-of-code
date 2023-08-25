#pragma once

#include "precompute.hpp"
#include "board.hpp"

struct TestResult {
    int overhead;
    bool result;

    TestResult(int, bool);
};

void printBitboard(uint64_t);

void testMakeUnmake();

void testQuietMoves();
void testMakeQuietMoves(std::vector<uint32_t>, std::vector<Board>);
void testUnmakeQuietMoves(std::vector<uint32_t>, std::vector<Board>);

void testCaptures();

void testCastling();
void testMakeCastling();
void testUnmakeCastling();

void enPassant();
void pawnPromotions();
void pawnPushes();

TestResult testMoveTemplate(std::vector<uint32_t>, std::vector<Board>, int);
int performMethodAndGetOverhead(const uint32_t, Board*, const int);
bool assertBoardIsCorrect(Board, std::vector<Board>, int, int);

void perft();
