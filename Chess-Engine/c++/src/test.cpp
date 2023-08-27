#include "test.hpp"
#include "precompute.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include "board.hpp"
#include <array>

using std::cout;
using std::clog;
using std::vector;
using std::array;

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

void testCaptures() {
    vector<uint32_t> moves = { 0x30111c9d, 0x30001cb3, 0x30001858, 0x30001660, 
                               0x30001a9b, 0x30001725, 0x30001704, 0x30001038 };

    vector<Board> positions = {
        Board("rnbq3r/ppppkp2/5n2/2b1p1pp/2BPN1PP/1P6/P1P1PP2/R1BQK1NR w KQ h6 0 6"),

        Board("rnbq3r/ppppkB2/5n2/2b1p1pp/3PN1PP/1P6/P1P1PP2/R1BQK1NR b KQ - 0 6"),
        Board("rnbq3r/pppp1k2/5n2/2b1p1pp/3PN1PP/1P6/P1P1PP2/R1BQK1NR w KQ - 0 7"),
        Board("rnbq3r/pppp1k2/5n2/2b1p1Pp/3PN1P1/1P6/P1P1PP2/R1BQK1NR b KQ - 0 7"),
        Board("rnbq3r/pppp1k2/5n2/2b1p1P1/3PN1p1/1P6/P1P1PP2/R1BQK1NR w KQ - 0 8"),
        Board("rnbq3r/pppp1k2/5N2/2b1p1P1/3P2p1/1P6/P1P1PP2/R1BQK1NR b KQ - 0 8"),
        Board("rnbq3r/pppp1k2/5N2/4p1P1/3b2p1/1P6/P1P1PP2/R1BQK1NR w KQ - 0 9"),
        Board("rnbq3r/pppp1k2/5N2/4p1P1/3Q2p1/1P6/P1P1PP2/R1B1K1NR b KQ - 0 9"),
        Board("rnbq4/pppp1k2/5N2/4p1P1/3Q2p1/1P6/P1P1PP2/R1B1K1Nr w Q - 0 10")
    };

    testMakeCaptures(moves, positions);

    std::reverse(positions.begin(), positions.end());
    std::reverse(moves.begin(), moves.end());

    testUnmakeCaptures(moves, positions);

    cout << "\n";
}

void testPawnPushes() {
    vector<uint32_t> moves = { 0xf00006cb, 0xf00708f3, 0xf017050c, 
                               0xf0000975, 0xf01b068a, 0xf00509b6, 
                               0xf01d089a, 0xf00007a6           };

    vector<Board> positions = {
        Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),

        Board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"),
        Board("rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2"),
        Board("rnbqkbnr/pppp1ppp/8/4p3/4P3/3P4/PPP2PPP/RNBQKBNR b KQkq - 0 2"),
        Board("rnbqkbnr/pp1p1ppp/8/2p1p3/4P3/3P4/PPP2PPP/RNBQKBNR w KQkq c6 0 3"),
        Board("rnbqkbnr/pp1p1ppp/8/2p1p3/4PP2/3P4/PPP3PP/RNBQKBNR b KQkq f3 0 3"),
        Board("rnbqkbnr/p2p1ppp/8/1pp1p3/4PP2/3P4/PPP3PP/RNBQKBNR w KQkq b6 0 4"),
        Board("rnbqkbnr/p2p1ppp/8/1pp1pP2/4P3/3P4/PPP3PP/RNBQKBNR b KQkq - 0 4"),
        Board("rnbqkbnr/p2p1ppp/8/2p1pP2/1p2P3/3P4/PPP3PP/RNBQKBNR w KQkq - 0 5")
    };

    testMakePawnPushes(moves, positions);

    std::reverse(positions.begin(), positions.end());
    std::reverse(moves.begin(), moves.end());

    testUnmakePawnPushes(moves, positions);
}

void testMakePawnPushes(vector<uint32_t> moves, vector<Board> positions) {
    TestResult out = testMakeUnmakeLayout(moves, positions, makeMove);
    
    
    clog << "Make pawn pushes" << results[out.result] << 
        ". Average overhead: " << out.overhead/moves.size() << "ns\n"; 
}
void testUnmakePawnPushes(vector<uint32_t> moves, vector<Board> positions) {
    TestResult out = testMakeUnmakeLayout(moves, positions, unmakeMove);

    clog << "Unmake pawn pushes" << results[out.result] << 
        ". Average overhead: " << out.overhead/moves.size() << "ns\n"; 
}

void testMakeQuietMoves(vector<uint32_t> moves, vector<Board> positions) {
    TestResult out = testMakeUnmakeLayout(moves, positions, makeMove);

    clog << "Make quiet moves" << results[out.result] << 
        ". Average overhead: " << out.overhead/moves.size() << "ns\n"; 
}
void testUnmakeQuietMoves(vector<uint32_t> moves, vector<Board> positions) {
    TestResult out = testMakeUnmakeLayout(moves, positions, unmakeMove);

    clog << "Unmake quiet moves" << results[out.result] << 
        ". Average overhead: " << out.overhead/moves.size() << "ns\n"; 
}

void testMakeCaptures(vector<uint32_t> moves, vector<Board> positions) {
    TestResult out = testMakeUnmakeLayout(moves, positions, makeMove);

    clog << "Make captures" << results[out.result] <<
        ". Average overhead: " << out.overhead/moves.size() << "ns\n";
}
void testUnmakeCaptures(vector<uint32_t> moves, vector<Board> positions) {
    vector<array<uint64_t, 2>> capture_stacks = { 
        array<uint64_t, 2>{10, 1027}, array<uint64_t, 2>{10, 128}, array<uint64_t, 2>{1, 128},
        array<uint64_t, 2>{1, 16}, array<uint64_t, 2>{0, 16}, array<uint64_t, 2>{0, 2},
        array<uint64_t, 2>{0, 2}, array<uint64_t, 2>{0, 0} 
    };

    TestResult out = testMakeUnmakeLayout(moves, positions, capture_stacks, unmakeMove);

    clog << "Unmake captures" << results[out.result] <<
        ". Average overhead: " << out.overhead/moves.size() << "ns\n";   
}

TestResult testMakeUnmakeLayout(vector<uint32_t> moves, vector<Board> positions, int method_to_perform) {
    Board board = positions[0];

    bool test_passed = true;
    int overhead = 0;

    for (uint32_t i = 0; i < moves.size(); i++) {
        overhead += performMethodAndGetOverhead(moves[i], &board, method_to_perform);
        test_passed &= assertBoardIsCorrect(board, positions, i, method_to_perform);
    }

    return TestResult(overhead, test_passed);
}

TestResult testMakeUnmakeLayout(vector<uint32_t> moves, vector<Board> positions, 
                                vector<array<uint64_t, 2>> capture_stacks, int method_to_perform) {
    
    Board board = positions[0];

    bool test_passed = true;
    int overhead = 0;

    for (uint32_t i = 0; i < moves.size(); i++) {
        board.capture_stack[0] = capture_stacks[i][0];
        board.capture_stack[1] = capture_stacks[i][1];
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

        clog << "Previous:\n";
        positions[index].printOut();
    }
    return move_passed;
}