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

void Test::makeUnmakeTest() {
    
}