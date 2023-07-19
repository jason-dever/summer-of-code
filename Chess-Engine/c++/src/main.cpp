#include <iostream>
#include "board.h"
#include "position.h"
#include "precompute.h"
#include "test.h"
#include <iostream>
#include <immintrin.h>

int main() {
    Board board;
    board.storeFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -- 0 0"); // Starting position
    printBoard(board);
    printBitboard(0x8100000000000000);
    printBitboard(0x81);
    return 0;
}