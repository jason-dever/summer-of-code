#include "test.hpp"
#include "precompute.hpp"
#include <iostream>
#include <string>
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

void Test::makeUnmake() {
    // quietMoves();
    // captures();
    
    // castling();

    // enPassant();
    // pawnPromotions();
    // pawnPushes();
}

void Test::quietMoves() {
    Board positions[7] { Board("r2qkbnr/ppp4p/2npbp2/3Np1p1/2BPP2B/5Q2/PPP2PPP/R3K1NR w KQkq g6 0 8"), 
                         Board("r2qkbnr/ppp4p/2npbp2/3Np1p1/2BPP3/5QB1/PPP2PPP/R3K1NR b KQkq - 1 8"),
                         Board("r2qkbnr/ppp4p/2np1p2/3Np1p1/2BPP1b1/5QB1/PPP2PPP/R3K1NR w KQkq - 2 9"),
                         Board("r2qkbnr/ppp4p/2np1p2/3Np1p1/2BPP1b1/Q5B1/PPP2PPP/R3K1NR b KQkq - 3 9"),
                         Board("r2qkbnr/ppp4p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/R3K1NR w KQkq -- 4 10"),
                         Board("r2qkbnr/ppp4p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/3RK1NR b Kkq - 5 10"),
                         Board("r2q1bnr/pppk3p/3p1p2/3Np1p1/1nBPP1b1/Q5B1/PPP2PPP/3RK1NR w K - 6 11") };

    Board board = positions[0];

    uint32_t moves[6] { 0xf0130458, 0xf020066b, 0xf04005d2, 
                        0xf06007ad, 0xf0800107, 0xd0a00d3b };
    
    std::string results[2] { " fail", " pass" };
    bool test_passed;

    for (int i = 0; i <= 5; i++) {
        board.makeMove(moves[i]);

        test_passed = (board == positions[i+1]);
        clog << "Make quiet move " << i << results[test_passed];

        if (!test_passed) {
            cout << "Expected:\n";
            positions[i+1].printOut();

            cout << "Actual:\n";
            board.printOut();
            break;
        } 
    }

    cout << "\n";

    board = positions[6];
    for (int i = 5; i >= 0; i--) {
        board.unmakeMove(moves[i]);

        test_passed = (board == positions[i]);
        clog << "Unmake quiet move " << i << results[test_passed];

        if (!test_passed) {
            cout << "Expected:\n";
            positions[i].printOut();

            cout << "Actual:\n";
            board.printOut();
            break;
        } 
    }
    cout << "\n";
}