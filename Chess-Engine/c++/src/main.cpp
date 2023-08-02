#include <iostream>
#include "board.h"
#include "position.h"
#include "precompute.h"
#include "test.h"
#include "movegen.h"
#include <iostream>
#include <immintrin.h>

int main() {
    Board board;
    // board.storeFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -- 0 0"); // Starting position
    board.storeFEN("r3k2r/pppq1ppp/2n2n2/2b2bN1/2B2pP1/2N1Q3/PPP4P/R4RK1 w KQkq g3 0 0");
    printBoard(board);
    board.makeMove(0b000000000000000110000000100000010);
    printBoard(board);
    board.unmakeMove(0b000000000000000110000000100000010);
    printBoard(board);
    return 0;
}