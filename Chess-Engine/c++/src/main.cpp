#include <iostream>
#include "board.h"
#include "movegen.h"
#include "position.h"
#include "precompute.h"
#include "test.h"
#include "magic.h"

extern U64 rook_moveboards[64][4096];
extern U64 bishop_moveboards[64][512];

int main() {
    Board board;
    U64 index;
    // board.storeFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -- 0 0"); // Starting position
    board.storeFEN("r1b1kr2/ppp1qpb1/2Np2p1/1P4p1/N1B1n3/1P2Q3/PBP2PPP/3RK2R w Kq - 0 1");
    index = blockerboard(board, rook_relevant_occupancy[4]);

    index *= rook_magics[4];
    index >>= rook_shifts[4];
    printBitboard(rook_moveboards[4][index]);
    return 0;
}