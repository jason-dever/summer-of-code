#pragma once

#include "precompute.hpp"
#include "board.hpp"
#include <array>
#include <vector>
#include <string>
#include <chrono>

struct TestResult {
    int overhead;
    bool result;

    TestResult(int, bool);
};

struct Perft : Board{
    int nodes_visited = 0;
    std::vector<bool> depths_reached;

    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point stop;

    Perft(std::string); 
    void perft(const int, const int);
};

void printBitboard(uint64_t);

void testMakeUnmake();

void testQuietMoves();
void testCaptures();
void testPawnPushes();
void testEnPassant();
void testPromotions();
void testCastling();

void testMakeQuietMoves(std::vector<uint32_t>, std::vector<Board>);
void testUnmakeQuietMoves(std::vector<uint32_t>, std::vector<Board>);

void testMakeCaptures(std::vector<uint32_t>, std::vector<Board>);
void testUnmakeCaptures(std::vector<uint32_t>, std::vector<Board>);

void testMakePawnPushes(std::vector<uint32_t>, std::vector<Board>);
void testUnmakePawnPushes(std::vector<uint32_t>, std::vector<Board>);

void testMakeEnPassant(std::vector<uint32_t>, std::vector<Board>);
void testUnmakeEnPassant(std::vector<uint32_t>, std::vector<Board>);

void testMakePromotions(std::vector<uint32_t>, std::vector<Board>);
void testUnmakePromotions(std::vector<uint32_t>, std::vector<Board>);

void testMakeCastling(std::vector<uint32_t>, std::vector<Board>);
void testUnmakeCastling(std::vector<uint32_t>, std::vector<Board>);

TestResult testMakeUnmakeLayout(std::vector<uint32_t>, std::vector<Board>, int);
TestResult testMakeUnmakeLayout(std::vector<uint32_t>, std::vector<Board>, 
                                std::vector<std::array<uint64_t, 2>>, int);

int performMethodAndGetOverhead(const uint32_t, Board*, const int);
bool assertBoardIsCorrect(Board, std::vector<Board>, int, int);