#include <iostream>
#include "board.h"
#include "movegen.h"
#include "position.h"
#include "precompute.h"
#include "test.h"
#include <intrin.h>
#include <cmath>
#include <iostream>

int main() {
    Board board;
    U64 index;
    std::cout << "initialized!\n";
    // board.storeFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -- 0 0"); // Starting position
    board.storeFEN("r1b1kr2/ppp1qpb1/2Np2p1/1P4p1/N1B1n3/1P2Q3/PBP2PPP/3RK2R w Kq - 0 1");
    return 0;
}