#pragma once

#include "precompute.hpp"
#include "board.hpp"
#include <array>
#include <vector>

struct TestResult {
    int overhead;
    bool result;

    TestResult(int, bool);
};

void printBitboard(uint64_t);

void testMakeUnmake();

void testQuietMoves();
void testCaptures();
void testPawnPushes();
void testEnPassant();

void testMakeQuietMoves(std::vector<uint32_t>, std::vector<Board>);
void testUnmakeQuietMoves(std::vector<uint32_t>, std::vector<Board>);

void testMakeCaptures(std::vector<uint32_t>, std::vector<Board>);
void testUnmakeCaptures(std::vector<uint32_t>, std::vector<Board>);

void testMakePawnPushes(std::vector<uint32_t>, std::vector<Board>);
void testUnmakePawnPushes(std::vector<uint32_t>, std::vector<Board>);

void testMakeEnPassant(std::vector<uint32_t>, std::vector<Board>);
void testUnmakeEnPassant(std::vector<uint32_t>, std::vector<Board>);

void testCastling();
void testMakeCastling();
void testUnmakeCastling();

void pawnPromotions();

TestResult testMakeUnmakeLayout(std::vector<uint32_t>, std::vector<Board>, int);
TestResult testMakeUnmakeLayout(std::vector<uint32_t>, std::vector<Board>, 
                                std::vector<std::array<uint64_t, 2>>, int);

int performMethodAndGetOverhead(const uint32_t, Board*, const int);
bool assertBoardIsCorrect(Board, std::vector<Board>, int, int);

void perft();
