#include <iostream>
#include "board.h"
#include "movegen.h"
#include "position.h"
#include "precompute.h"
#include "test.h"

extern U64 rook_moveboards[64][4096];
extern U64 bishop_moveboards[64][512];

int main() {
    Board board;
    extern Magic magic_rook_tbl[64];
    // board.storeFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -- 0 0"); // Starting position
    board.storeFEN("r1b1kr2/ppp1qpb1/2Np2p1/1P4p1/N1B1n3/1P2Q3/PBP2PPP/3RK2R w Kq - 0 1");
    // U64 index = blockerboard(board, magic_rook_tbl[4].mask);
    // index *= magic_rook_tbl[4].magic;
    // index >>= (64-__popcnt64(magic_rook_tbl[4].mask));
    // printBitboard(rook_moveboards[4][index]);

    // U64 blocker = blockerboard(board, magic_rook_tbl[4].mask);
    // printBitboard(moveboardRook(4, blocker));
    printBitboard(magic_rook_tbl[0].magic);
    return 0;
}