#pragma once

#include "precompute.hpp"
#include "board.hpp"
#include <immintrin.h>

/* Moves are encoded using four bytes.

    Bits 0-5: from square
    Bits 6-11: to square

    Bits 12-15:
        00 00 quiet move
        00 01 capture
        00 10 en passant

        10 00 queen promo
        01 00 knight promo

        00 11 king castle
        11 11 queen castle
    
    Bit 16: Is an en passant square on the board?
    Bits 17-20: The en passant square (if en passant)
    
    Bits 21-27: The halfmove clock before the move was made

    Bits 28-30:
        000 pawn move
        001 knight move
        010 bishop move
        011 rook move
        100 queen move
        101 king move

        These numbers also correspond to the 
        numbers for the pieces declared in PiecesEnum.
    
    Bit 31 is unused.
 */

extern uint64_t rook_moveboards[102400];
extern uint64_t bishop_moveboards[5248];
extern uint64_t knight_moveboards[64];
extern uint64_t pawn_moveboards[2][2][64];
extern uint64_t king_moveboards[64];

const uint16_t capture_mask = 0x1000;
const uint16_t en_passant_mask = 0x2000;

const uint16_t queen_promo_mask = 0x8000;
const uint16_t knight_promo_mask = 0x4000;

const uint16_t kingcastle_mask = 0x3000;
const uint16_t queencastle_mask = 0xF000;

inline void Board::genMoves() {
    legal_captures = __UINT64_MAX__;
    legal_pushes = __UINT64_MAX__;

    opponent = !turn;

    friendly_occupancy = sidePieces(turn);
    enemy_occupancy = sidePieces(opponent);
    occupancy = friendly_occupancy | enemy_occupancy;

    uint64_t checking_pieces = getEnemyCheckingPieces(opponent);
    uint_fast8_t num_checking_pieces = _popcnt64(checking_pieces);

    genKingMoves();

    if (num_checking_pieces == 1) {
        int checker_square = _tzcnt_u64(checking_pieces);
        legal_captures = checking_pieces;
        legal_pushes = 0;

        uint_fast8_t king_square = _tzcnt_u64(pieces[turn][king]);

        if (((checker_square%8) == (king_square%8)) || (checker_square/8) == (king_square/8)) {
            legal_pushes = rookMoves(king_square, occupancy) & rookMoves(checker_square, occupancy);
        }
        else if (checking_pieces & pieces[opponent][bishops] || checking_pieces & pieces[opponent][queens]) {
            legal_pushes = bishopMoves(king_square, occupancy) & bishopMoves(checker_square, occupancy);
        }
    }
    else if (num_checking_pieces > 1) {
        return;
    }

    genBishopMoves();
    genRookMoves();
    genQueenMoves();
    genPawnMoves();
    genKnightMoves();
}

const uint64_t castling[2][2] = { {0x1, 0x8}, {0x8000000000000000, 0x0100000000000000} };
const uint64_t castle_through_squares[2][2] = { {0x7, 0x38}, {0x0700000000000000, 0x3800000000000000} };

inline void Board::genKingMoves() {
    uint_fast8_t from_square = _tzcnt_u64(pieces[turn][king]);
    uint_fast8_t to_square;

    uint32_t move;

    uint64_t enemy_attacked_squares = getEnemyAttackedSquares(opponent);

    uint64_t moveboard = ( king_moveboards[from_square] 
        & ~( enemy_attacked_squares | friendly_occupancy ) ) & (legal_captures | legal_pushes);

    while (moveboard) {
        to_square = _tzcnt_u64(moveboard);

        bool move_is_capture = enemy_occupancy & (1ULL << to_square); 

        /* If the to square is occupied, the move will be flagged as a capture. 
        If not, it's a quiet move. */
        move = (from_square) | (to_square << 6) | 
            ( move_is_capture << 12 ) | (king << 16) | (half_moves << 21);
            
        moves.push_back(move);
        flipBit(moveboard, to_square);
    }

    if (!has_castled[turn]) {
        if ( castle_squares & castling[turn][0] && !((enemy_attacked_squares | occupancy) & castle_through_squares[turn][0]) ) {
            moves.push_back(kingcastle_mask | (half_moves << 21));
        }
        if ( castle_squares & castling[turn][1] && !((enemy_attacked_squares | occupancy) & castle_through_squares[turn][1]) ) {
            moves.push_back(queencastle_mask | (half_moves << 21));
        }
    }
}

inline void Board::genKnightMoves() {
    uint64_t friendly_knights = pieces[turn][knights];

    uint64_t moveboard;
    uint32_t move;

    uint_fast8_t from_square;
    uint_fast8_t to_square;

    while (friendly_knights) {
        from_square = _tzcnt_u64(friendly_knights);

        moveboard = ( knight_moveboards[from_square] 
            & ~friendly_occupancy ) & (legal_captures | legal_pushes);

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            bool move_is_capture = enemy_occupancy & (1ULL << to_square); 

            move = (from_square) | (to_square << 6) 
                | ( move_is_capture << 12 ) | (knights << 16) | (half_moves << 21);
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_knights, from_square);
    }
}

inline void Board::genBishopMoves() {
    uint64_t friendly_bishops = pieces[turn][bishops];

    uint64_t moveboard;
    uint32_t move;

    uint_fast8_t from_square;
    uint_fast8_t to_square;

    while (friendly_bishops) {
        from_square = _tzcnt_u64(friendly_bishops);

        moveboard = bishopMoves(from_square, occupancy) & (legal_captures | legal_pushes);

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            bool move_is_capture = enemy_occupancy & (1ULL << to_square); 

            move = (from_square) | (to_square << 6) 
                | ( move_is_capture << 12 ) | (bishops << 16) | (half_moves << 21);
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_bishops, from_square);
    }
}

inline void Board::genRookMoves() {
    uint64_t friendly_rooks = pieces[turn][rooks];

    uint64_t moveboard;
    uint32_t move;

    uint_fast8_t from_square;
    uint_fast8_t to_square;

    while (friendly_rooks) {
        from_square = _tzcnt_u64(friendly_rooks);

        moveboard = rookMoves(from_square, occupancy) & (legal_captures | legal_pushes);

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            bool move_is_capture = enemy_occupancy & (1ULL << to_square); 

            move = (from_square) | (to_square << 6) 
                | ( move_is_capture << 12 ) | (rooks << 16) | (half_moves << 21);
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_rooks, from_square);
    }
}

inline void Board::genQueenMoves() {
    uint64_t friendly_queens = pieces[turn][queens];

    uint64_t moveboard;
    uint32_t move;

    uint_fast8_t from_square;
    uint_fast8_t to_square;

    while (friendly_queens) {
        from_square = _tzcnt_u64(friendly_queens);

        moveboard = ( rookMoves(from_square, occupancy) | bishopMoves(from_square, occupancy) ) & (legal_captures | legal_pushes);

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            bool move_is_capture = enemy_occupancy & (1ULL << to_square); 

            move = (from_square) | (to_square << 6) 
                | ( move_is_capture << 12 ) | (queens << 16) | (half_moves << 21);
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_queens, from_square);
    }
}

const uint64_t promotion_mask = 0xFF000000000000FF;

inline void Board::genPawnMoves() {
    uint64_t friendly_pawns = pieces[turn][pawns];

    /* This variable saves doing the to_square << 6 operation more than once.
    It's used in this function and not others because in the other movegen functions
    the to_square << 6 operation to place to_square into the move is only performed once. */
    uint_fast32_t to_square_in_move;

    uint64_t to_square_bitboard;

    uint64_t capture_board;
    uint64_t moveboard;
    uint32_t move;

    uint_fast8_t from_square;
    uint_fast8_t to_square;

    while (friendly_pawns) {
        from_square = _tzcnt_u64(friendly_pawns);

        capture_board = pawn_moveboards[turn][captures][from_square] & (enemy_occupancy | en_passant_squares)
            & (legal_captures | legal_pushes);
        moveboard = pawn_moveboards[turn][pushes][from_square] & ~(occupancy) & legal_pushes;

        while (capture_board) {
            to_square = _tzcnt_u64(capture_board);
            to_square_bitboard = 1ULL << to_square;
            to_square_in_move = to_square << 6;

            // If en passant is played this boolean will be false.
            bool move_is_capture = enemy_occupancy & (1ULL << to_square); 

            /* If to square & promotion_mask is > 0 then the move is a promotion.
            We need to make two moves; one for knight promotion and one for queen promotion.
            * This might mess with perft numbers because we don't generate moves for rook/bishop promotions. */
            if ( to_square_bitboard & promotion_mask ) {
                move = from_square | to_square_in_move | (capture_mask | queen_promo_mask)
                    | (half_moves << 21);

                moves.push_back(move);

                move ^= queen_promo_mask;
                move |= knight_promo_mask;

                moves.push_back(move);
            }
            else {
                bool move_is_en_passant = ( _pext_u64(en_passant_squares, to_square_bitboard) );

                move = (from_square) | to_square_in_move | (move_is_capture << 12) 
                    | (move_is_en_passant << 13) | (half_moves << 21);

                moves.push_back(move);
            }
            flipBit(capture_board, to_square);
        }
        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);
            to_square_in_move = to_square << 6;

            if ( (1ULL << to_square) & promotion_mask ) {
                move = from_square | to_square_in_move | queen_promo_mask | (half_moves << 21);
                moves.push_back(move);

                move ^= queen_promo_mask;
                move |= knight_promo_mask;

                moves.push_back(move);
            }
            else { // Quiet move
                move = from_square | to_square_in_move | (half_moves << 21);

                moves.push_back(move);
            }
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_pawns, from_square);
    }
}