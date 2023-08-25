#include "test.hpp"
#include "precompute.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include "board.hpp"

using std::cout;
using std::clog;
using std::vector;

void printBitboard(uint64_t bitboard) {
    int_fast8_t rank = 8;
    
    std::string line = std::to_string(rank) + "    ";;

    for (int_fast8_t i = 63; i >= 0; i--) {
        if ((bitboard >> i) & 0x1) {
            line.append("1 ");
        }
        else {   
            line.append("0 ");
        }

        if (i%8 == 0) {
            cout << line << "\n";
            rank--;
            line = std::to_string(rank) + "    ";;
        }
    }
    cout << "\n     a b c d e f g h";
    cout << "\n     Bitset value: " << bitboard << "d\n\n";
}

enum BoardMethodsEnum { makeMove, unmakeMove };
std::string results[2] = {" fail", " pass" };

TestResult::TestResult(int test_overhead, bool test_result) {
        overhead = test_overhead;
        result = test_result;
}

void testMakeUnmake() {
    // quietMoves();
    // captures();
    
    // castling();

    // enPassant();
    // pawnPromotions();
    // pawnPushes();
}

void testQuietMoves() {
    vector<uint32_t> moves = { 0xf0130458, 0xf020066b, 0xf04005d2, 
                               0xf06007ad, 0xf0800107, 0xd0a00d3b };

    vector<Board> positions = { 
        Board("r2qkbnr/ppp4p/2npbp2/3Np1p1/2BPP2B/5Q2/PPP2PPP/R3K1NR w KQkq g6 0 8"), 
        
        Board("r2qkbnr/ppp4p/2npbp2/3Np1p1/2BPP3/5QB1/PPP2PPP/R3K1NR b KQkq - 1 8"),
        Board("r2qkbnr/ppp4p/2np1p2/3Np1p1/2BPP1b1/5QB1/PPP2PPP/R3K1NR w KQkq - 2 9"),
        Board("r2qkbnr/ppp4p/2np1p2/3Np1p1/2BPP1b1/Q5B1/PPP2PPP/R3K1NR b KQkq - 3 9"),
        Board("r2qkbnr/ppp4p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/R3K1NR w KQkq - 4 10"),
        Board("r2qkbnr/ppp4p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/3RK1NR b Kkq - 5 10"),
        Board("r2q1bnr/pppk3p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/3RK1NR w K - 6 11") 
    };
    
    testMakeQuietMoves(moves, positions);

    std::reverse(positions.begin(), positions.end());
    std::reverse(moves.begin(), moves.end());

    testUnmakeQuietMoves(moves, positions);

    cout << "\n";
}

void testMakeQuietMoves(vector<uint32_t> moves, vector<Board> positions) {
    TestResult out = testMoveTemplate(moves, positions, makeMove);

    clog << "Make quiet moves" << results[out.result] << 
        ". Average overhead: " << out.overhead/moves.size() << "ns\n"; 
}

void testUnmakeQuietMoves(vector<uint32_t> moves, vector<Board> positions) {
    TestResult out = testMoveTemplate(moves, positions, unmakeMove);

    clog << "Unmake quiet moves" << results[out.result] << 
        ". Average overhead: " << out.overhead/moves.size() << "ns\n"; 
}

TestResult testMoveTemplate(vector<uint32_t> moves, vector<Board> positions, int method_to_perform) {
    Board board;

    bool test_passed = true;
    int overhead = 0;

    for (uint32_t i = 0; i < moves.size(); i++) {
        board = positions[i];

        overhead += performMethodAndGetOverhead(moves[i], &board, method_to_perform);
        test_passed &= assertBoardIsCorrect(board, positions, i, method_to_perform);
    }

    return TestResult(overhead, test_passed);
}

int performMethodAndGetOverhead(const uint32_t move, Board* board_ptr, const int method) {
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point stop;

    /* By dereferencing the board beforehand, 
    no additional overhead is incurred during the benchmark.*/
    Board dereferenced_board = *board_ptr;

    if (method == makeMove) {
        start = std::chrono::high_resolution_clock::now();
        dereferenced_board.makeMove(move);
        stop = std::chrono::high_resolution_clock::now();
    }
    else {
        start = std::chrono::high_resolution_clock::now();
        dereferenced_board.unmakeMove(move);
        stop = std::chrono::high_resolution_clock::now();
    }
    *board_ptr = dereferenced_board;

    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count();
}

bool assertBoardIsCorrect(Board board, vector<Board> positions, int index, int method) {    
    bool move_passed = board == positions[index+1]; 

    if (!move_passed) {
        std::string methods[2] {"Make ", "Unmake "};

        clog << methods[method] << "move " << index << " fail\n";

        clog << "Expected:\n";
        positions[index+1].printOut();

        clog << "Actual:\n";
        board.printOut();
    }
    return move_passed;
}

void testMakeCastling() {
    Board board;
    Board positions[5] {
        Board("r3k2r/1pp1qpp1/p1np1n1p/1B2p1B1/1b2P1b1/2NP1N2/PPP1QPPP/R3K2R w KQkq - 2 10"),

        Board("r3k2r/1pp1qpp1/p1np1n1p/1B2p1B1/1b2P1b1/2NP1N2/PPP1QPPP/R4RK1 b kq - 3 10"),
        Board("2kr3r/1pp1qpp1/p1np1n1p/1B2p1B1/1b2P1b1/2NP1N2/PPP1QPPP/R4RK1 w - - 4 11"),

        Board("r3k2r/1pp1qpp1/p1np1n1p/1B2p1B1/1b2P1b1/2NP1N2/PPP1QPPP/2KR3R b kq - 3 10"),
        Board("r4rk1/1pp1qpp1/p1np1n1p/1B2p1B1/1b2P1b1/2NP1N2/PPP1QPPP/2KR3R w - - 4 11")
    };

    uint32_t moves[4] = { 0xf0403000, 0xc060f000, 0xf040f000, 0xc0603000 }; 

    bool test_failed = false;
    int overhead = 0;

    for (int i = 1; i <= 2; i++) {
        board = positions[i-1];

        auto start = std::chrono::high_resolution_clock::now();
        board.makeMove(moves[i]);
        auto stop = std::chrono::high_resolution_clock::now();

        overhead += std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count();

        bool move_failed = board != positions[i];
        test_failed |= move_failed;

        if (move_failed) {
            clog << "Make castling " << i << "fail.\n";
            
            clog << "Expected:\n";
            positions[i].printOut();

            clog << "Actual:\n";
            board.printOut();
        } 
    }
    board = positions[0];

    for (int i = 3; i <= 4; i++) {
        
    }
    

    std::string results[2] { " pass", " fail" };
    clog << "Make castling" << results[test_failed] << ", Avg overhead: " << overhead/4 << "ns\n";
}

void testUnmakeCastling() {

}