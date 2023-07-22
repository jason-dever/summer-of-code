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
U64 castle_through_squares[2][2] = { {0x7, 0x38}, {0x0700000000000000, 0x3800000000000000} };

inline void Board::genMoves() {

}

inline void Board::genKingMoves() {
    bool opponent = !turn;
    unsigned int from_square = _tzcnt_u64(pieces[turn][king]);
    unsigned int to_square;
    U64 enemy_attacked_squares = getAttackedSquares(opponent);

    U64 moveboard = ( king_moveboards[from_square] 
        & ~( enemy_attacked_squares | sidePieces(turn) ) );

    U64 occupied;
    unsigned short move;

    while (moveboard) {
        to_square = _tzcnt_u64(moveboard);

        /* If the to square is occupied, the move will be flagged as a capture. 
        If not, it's a quiet move. */
        move = (from_square) | (to_square << 6) | 
            ( ( _pext_u64( sidePieces(opponent), 1ULL << to_square ) ) << 12 );
            
        moves.push_back(move);
        flipBit(moveboard, to_square);
    }

    if ( castle_squares & castling[turn][0] && ~(enemy_attacked_squares & castle_through_squares[turn][0]) ) {
        moves.push_back(0x3000);
    }
    if ( castle_squares & castling[turn][1] && ~(enemy_attacked_squares & castle_through_squares[turn][1]) ) {
        moves.push_back(0xF000);
    }
}

inline void Board::genKnightMoves() {
    U64 friendly_knights = pieces[turn][knights];
    U64 moveboard;
    unsigned short move;
    unsigned int from_square;
    unsigned int to_square;

    while (friendly_knights) {
        from_square = _tzcnt_u64(friendly_knights);

        moveboard = knight_moveboards[from_square];

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            move = (from_square) | (to_square << 6) | 
                ( ( _pext_u64( sidePieces(!turn), 1ULL << to_square ) ) << 12 );
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_knights, from_square);
    }
}

inline void Board::genBishopMoves() {
    U64 friendly_bishops = pieces[turn][bishops];
    U64 friendly_occupancy = sidePieces(turn);
    U64 enemy_occupancy = sidePieces(!turn);
    U64 moveboard;
    unsigned short move;
    unsigned int from_square;
    unsigned int to_square;

    while (friendly_bishops) {
        from_square = _tzcnt_u64(friendly_bishops);

        moveboard = bishop_moveboards[_pext_u64((friendly_occupancy | enemy_occupancy), 
            bishop_relevant_occupancy[from_square])];

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            move = (from_square) | (to_square << 6) | 
                ( ( _pext_u64( enemy_occupancy, 1ULL << to_square ) ) << 12 );
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_bishops, to_square);
    }
}

inline void Board::genRookMoves() {
    U64 friendly_rooks = pieces[turn][rooks];
    U64 friendly_occupancy = sidePieces(turn);
    U64 enemy_occupancy = sidePieces(!turn);
    U64 moveboard;
    unsigned short move;
    unsigned int from_square;
    unsigned int to_square;

    while (friendly_rooks) {
        from_square = _tzcnt_u64(friendly_rooks);

        moveboard = rook_moveboards[_pext_u64((friendly_occupancy | enemy_occupancy), 
            rook_relevant_occupancy[from_square])];

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            move = (from_square) | (to_square << 6) | 
                ( ( _pext_u64( enemy_occupancy, 1ULL << to_square ) ) << 12 );
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_rooks, to_square);
    }
}

inline void Board::genQueenMoves() {
    U64 friendly_queens = pieces[turn][queens];
    U64 friendly_occupancy = sidePieces(turn);
    U64 enemy_occupancy = sidePieces(!turn);
    U64 moveboard;
    unsigned short move;
    unsigned int from_square;
    unsigned int to_square;

    while (friendly_queens) {
        from_square = _tzcnt_u64(friendly_queens);

        moveboard = rook_moveboards[_pext_u64((friendly_occupancy | enemy_occupancy), rook_relevant_occupancy[from_square])]
            | bishop_moveboards[_pext_u64((friendly_occupancy | enemy_occupancy), bishop_relevant_occupancy[from_square])];

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            move = (from_square) | (to_square << 6) | 
                ( ( _pext_u64( enemy_occupancy, 1ULL << to_square ) ) << 12 );
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_queens, to_square);
    }
}

inline void Board::genPawnMoves() {
    U64 moveboard;
    unsigned short move;
    unsigned int from_square;
    unsigned int to_square;
}