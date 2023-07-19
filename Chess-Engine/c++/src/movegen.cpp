#include "precompute.h"
#include "board.h"
#include <vector>

/* 
Moves are encoded using two bytes.

    6 bits from square
    6 bits to square

    Last 4 bits:
        00 00 quiet move
        00 01 capture
        00 10 en passant

        10 00 queen promo
        01 00 knight promo

        00 11 king castle
        11 11 queen castle
*/

std::vector<unsigned short> moves;

extern U64 rook_moveboards[102400];
extern U64 bishop_moveboards[5248];
extern U64 knight_moveboards[64];
extern U64 pawn_moveboards[2][2][64];
extern U64 king_moveboards[64];

U64 castling[2][2] = { {0x1, 0x8}, {0x80000000000000000, 0x0100000000000000} };

inline void Board::genMoves() {

}

inline void Board::genKingMoves() {
    bool opponent = !turn;
    unsigned int from_square = _tzcnt_u64(pieces[turn][king]);
    unsigned int to_square;

    U64 moveboard = ( king_moveboards[from_square] 
        & ~( getAttackedSquares(opponent) | sidePieces(turn) ) );

    U64 occupied;
    unsigned short move;

    while (moveboard) {
        to_square = _tzcnt_u64(moveboard);

        /* If the to square is occupied, the move will be flagged as a capture. 
        If not, it's a quiet move. */
        occupied = _pext_u64(sidePieces(opponent), 1ULL << to_square);

        move = (from_square) | (to_square << 6) | (occupied << 13);
        moves.push_back(move);
        flipBit(moveboard, to_square);
    }

    if (castle_squares & castling[turn][0]) {
        moves.push_back(0x3000);
    }
    if (castle_squares & castling[turn][1]) {
        moves.push_back(0xF000);
    }


}