#include "test.hpp"
#include "precompute.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include "board.hpp"

using std::cout;
using std::clog;

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

void testMakeUnmake() {
    // quietMoves();
    // captures();
    
    // castling();

    // enPassant();
    // pawnPromotions();
    // pawnPushes();
}

void testQuietMoves() {
    testMakeQuietMoves();
    testUnmakeQuietMoves();

    cout << "\n";
}

void testCastling() {
    testMakeCastling();
    testUnmakeCastling();

    cout << "\n";
}

void testMakeQuietMoves() {
    Board board;
    Board positions[7] { 
        Board("r2qkbnr/ppp4p/2npbp2/3Np1p1/2BPP2B/5Q2/PPP2PPP/R3K1NR w KQkq g6 0 8"), 

        Board("r2qkbnr/ppp4p/2npbp2/3Np1p1/2BPP3/5QB1/PPP2PPP/R3K1NR b KQkq - 1 8"),
        Board("r2qkbnr/ppp4p/2np1p2/3Np1p1/2BPP1b1/5QB1/PPP2PPP/R3K1NR w KQkq - 2 9"),
        Board("r2qkbnr/ppp4p/2np1p2/3Np1p1/2BPP1b1/Q5B1/PPP2PPP/R3K1NR b KQkq - 3 9"),
        Board("r2qkbnr/ppp4p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/R3K1NR w KQkq -- 4 10"),
        Board("r2qkbnr/ppp4p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/3RK1NR b Kkq - 5 10"),
        Board("r2q1bnr/pppk3p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/3RK1NR w K - 6 11") 
    };

    uint32_t moves[6] { 0xf0130458, 0xf020066b, 0xf04005d2, 
                        0xf06007ad, 0xf0800107, 0xd0a00d3b };
    
    
    bool test_failed = false;
    bool move_failed;

    int overhead = 0;

    for (int i = 0; i <= 5; i++) {
        board = positions[i]; 

        auto start = std::chrono::high_resolution_clock::now();
        board.makeMove(moves[i]);
        auto stop = std::chrono::high_resolution_clock::now();

        overhead += std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count();

        move_failed = !(board == positions[i+1]);
        test_failed |= move_failed;

        if (move_failed) {
            clog << "Make quiet move " << i << "fail\n";

            clog << "Expected:\n";
            positions[i+1].printOut();

            clog << "Actual:\n";
            board.printOut();
        }
    }

    std::string results[2] { " pass", " fail" };
    clog << "Make quiet move" << results[test_failed] << ", Avg overhead: " << overhead/6 << "ns\n";
}

void testUnmakeQuietMoves() {
    Board board;
    Board positions[7] { 
        Board("r2qkbnr/ppp4p/2npbp2/3Np1p1/2BPP2B/5Q2/PPP2PPP/R3K1NR w KQkq g6 0 8"), 

        Board("r2qkbnr/ppp4p/2npbp2/3Np1p1/2BPP3/5QB1/PPP2PPP/R3K1NR b KQkq - 1 8"),
        Board("r2qkbnr/ppp4p/2np1p2/3Np1p1/2BPP1b1/5QB1/PPP2PPP/R3K1NR w KQkq - 2 9"),
        Board("r2qkbnr/ppp4p/2np1p2/3Np1p1/2BPP1b1/Q5B1/PPP2PPP/R3K1NR b KQkq - 3 9"),
        Board("r2qkbnr/ppp4p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/R3K1NR w KQkq - 4 10"),
        Board("r2qkbnr/ppp4p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/3RK1NR b Kkq - 5 10"),
        Board("r2q1bnr/pppk3p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/3RK1NR w K - 6 11") 
    };

    uint32_t moves[6] { 0xf0130458, 0xf020066b, 0xf04005d2, 
                        0xf06007ad, 0xf0800107, 0xd0a00d3b };
    
    bool test_failed = false;
    bool move_failed;

    int overhead = 0;

    for (int i = 5; i >= 0; i--) {
        board = positions[i+1];

        auto start = std::chrono::high_resolution_clock::now();
        board.unmakeMove(moves[i]);
        auto stop = std::chrono::high_resolution_clock::now();

        overhead += std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count();

        move_failed = !(board == positions[i]);
        test_failed |= move_failed;

        if (move_failed) {
            clog << "Unmake quiet move " << i << "fail.\n";
            
            clog << "Expected:\n";
            positions[i].printOut();

            clog << "Actual:\n";
            board.printOut();
        } 
    }
    std::string results[2] { " pass", " fail" };
    clog << "Unmake quiet move" << results[test_failed] << ", Avg overhead: " << overhead/6 << "ns\n";
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
    bool move_failed;

    int overhead = 0;

    for (int i = 1; i <= 2; i++) {
        board = positions[i-1];

        auto start = std::chrono::high_resolution_clock::now();
        board.makeMove(moves[i]);
        auto stop = std::chrono::high_resolution_clock::now();

        overhead += std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start).count();

        move_failed |= !(board == positions[i]);
        test_failed |= move_failed;

        if (move_failed) {
            clog << "Unmake quiet move " << i << "fail.\n";
            
            clog << "Expected:\n";
            positions[i].printOut();

            clog << "Actual:\n";
            board.printOut();
        } 
    }
    std::string results[2] { " pass", " fail" };
    clog << "Make castling" << results[test_failed] << ", Avg overhead: " << overhead/6 << "ns\n";
}

void testUnmakeCastling() {

}