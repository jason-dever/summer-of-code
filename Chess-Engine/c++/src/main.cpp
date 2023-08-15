#include <iostream>
#include "board.hpp"
#include "position.hpp"
#include "precompute.hpp"
#include "test.hpp"
#include "movegen.hpp"
#include <iostream>
#include <immintrin.h>

// 0b110010010101100100011 en passant r1bqkbnr/ppp1pppp/2n5/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2

int main() {
    Board board;
    // board.storeFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -- 0 0"); // Starting position

    board.storeFEN("rn1qkbnr/ppp1pppp/8/8/6b1/3P2P1/PPP2PpP/RNBQK2R b KQkq - 0 8");
    board.printOut();
    
    board.makeMove(0x5009);
    board.printOut();

    board.unmakeMove(0x5009);
    board.printOut();
    
    return 0;
}