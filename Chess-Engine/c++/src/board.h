#pragma once

#include <iostream>
#include <string>
#include "precompute.h"
#include <vector>
#include "test.h"

enum PiecesEnum { pawns, knights, bishops, rooks, queens, king };
enum ColoursEnum { white, black };

class Board {
    public:
    
        void storeFEN(const std::string FEN);
        void printOut();

        inline void genMoves();

        inline bool makeMove(const uint32_t move);
        inline void unmakeMove(const uint32_t move);

        Board copy();

    // private:
        bool turn = 0;

        uint64_t pieces[2][6] { 0 };

        uint64_t castle_squares = 0;
        uint64_t en_passant_squares = 0;

        int_fast16_t half_moves;
        int_fast16_t full_moves;

        const uint_fast16_t capture_mask = 0x1000;
        const uint_fast16_t en_passant_mask = 0x2000;

        const uint_fast16_t queen_promo_mask = 0x8000;
        const uint_fast16_t knight_promo_mask = 0x4000;

        const uint_fast16_t kingcastle_mask = 0x3000;
        const uint_fast16_t queencastle_mask = 0xF000;

        std::vector<uint32_t> moves;

        uint64_t capture_stack[2] { 0 };

        bool opponent;

        bool has_castled[2] { 0 };

        uint64_t occupancy;

        uint64_t friendly_occupancy;
        uint64_t enemy_occupancy;
        uint64_t enemy_attacked_squares;

        uint64_t legal_captures;
        uint64_t legal_pushes;

        inline uint64_t whitePieces();
        inline uint64_t blackPieces();
        inline uint64_t sidePieces(const bool side);

        inline void genKingMoves();
        inline void genKnightMoves();
        inline void genBishopMoves();
        inline void genRookMoves();
        inline void genQueenMoves();
        inline void genPawnMoves();

        inline uint64_t getEnemyAttackedSquares(const bool enemy);
        inline uint64_t getEnemyCheckingPieces(const bool enemy);
};

inline bool Board::makeMove(const uint32_t move) {
    /* This function returns a bool to differentiate between legal and illegal moves.
    The move generator will rule out most illegal moves, but some illegal moves
    including en passant edge cases and movement of pinned pieces are caught here.
    The playing algorithm will use the boolean returned by this function to
    determine whether or not to search the move tree .*/

    // Full moves will be incremented by 1 if black is moving and nothing if white is moving.
    full_moves += turn;
    half_moves++;
    opponent = !turn;

    if ((move & queencastle_mask) == queencastle_mask) {

        pieces[turn][king] <<= 2;

        uint64_t castle_square = pieces[turn][king] << 2; 

        pieces[turn][rooks] ^= castle_square;
        pieces[turn][rooks] |= (castle_square >> 3);


        castle_squares ^= castle_square;
        has_castled[turn] = true;
        turn = opponent;
        return 1;
    }
    else if ((move & kingcastle_mask) == kingcastle_mask) {
        
        pieces[turn][king] >>= 2;

        uint64_t castle_square = pieces[turn][king] >> 1; 

        pieces[turn][rooks] ^= castle_square;
        pieces[turn][rooks] |= (castle_square << 2);


        castle_squares ^= castle_square;
        has_castled[turn] = true;
        turn = opponent;
        return 1;
    }

    uint_fast8_t moving_piece = (move >> 28);
    uint64_t from = 1ULL << (move & 0x3F);
    uint64_t to = 1ULL << ( (move >> 6) & 0x3F );

    pieces[turn][moving_piece] ^= from;

    if (move & capture_mask) {
        uint_fast8_t captured_piece;

        for (captured_piece = pawns; captured_piece < king; captured_piece++) {
            if (to & pieces[opponent][captured_piece])
                break;
        }

        pieces[!turn][captured_piece] ^= to;

        capture_stack[turn] <<= 3;
        capture_stack[turn] |= captured_piece;

        half_moves = 0;
    }
    else if (move & en_passant_mask) {
        pieces[opponent][pawns] ^= pawn_moveboards[opponent][pushes][_tzcnt_u64(en_passant_squares)];
        capture_stack[turn] <<= 3;

        half_moves = 0;
    }

    if (moving_piece == pawns) {
        half_moves = 0;
        uint64_t temp_shifts[2] = { from << 8, from >> 8 };

        en_passant_squares = (pawn_moveboards[opponent][pushes][_tzcnt_u64(to)]) & temp_shifts[turn];
    
        if (move & queen_promo_mask) 
            moving_piece = queens;
        else if (move & knight_promo_mask) 
            moving_piece = knights;
    }
    
    pieces[turn][moving_piece] |= to;

    turn = opponent; 

    if (getEnemyCheckingPieces(turn)) {
        std::cout << "bad move\n";
        unmakeMove(move);
        return 0;
    }

    return 1;
}

inline void Board::unmakeMove(const uint32_t move) {
    turn = !turn;
    full_moves -= turn;
    half_moves = (move >> 21) & 0x7F;

    int_fast16_t possible_en_passant_squares[16] { 16, 17, 18, 19, 20, 21, 22, 23,
                                                   40, 41, 42, 43, 44, 45, 46, 47 };

    en_passant_squares = static_cast<uint64_t>((move >> 16) & 1) << possible_en_passant_squares[(move >> 17) & 0xF];

    if ((move & queencastle_mask) == queencastle_mask) {
        uint64_t castle_square = pieces[turn][king] << 2;
        pieces[turn][king] >>= 2;

        pieces[turn][rooks] ^= (castle_square >> 3);
        pieces[turn][rooks] |= castle_square;

        castle_squares |= castle_square;
        has_castled[turn] = false;
        return;
    }
    else if ((move & kingcastle_mask) == kingcastle_mask) {
        uint64_t castle_square = pieces[turn][king] >> 1;

        pieces[turn][king] <<= 2;

        pieces[turn][rooks] ^= (castle_square << 2);
        pieces[turn][rooks] |= castle_square;

        castle_squares |= castle_square;
        has_castled[turn] = false;
        return;
    }

    uint64_t from = 1ULL << (move & 0x3F);
    uint64_t to = 1ULL << ( (move >> 6) & 0x3F );

    if (move & capture_mask) {
        pieces[!turn][capture_stack[turn] & 0x7] |= to;
        capture_stack[turn] >>= 3;
    }
    else if (move & en_passant_mask) {
        pieces[!turn][pawns] |= pawn_moveboards[!turn][pushes][_tzcnt_u64(to)];
        capture_stack[turn] >>= 3;
    }

    if (move & queen_promo_mask) {
        pieces[turn][pawns] |= from;
        pieces[turn][queens] ^= to;
    }
    else if (move & knight_promo_mask) {
        pieces[turn][pawns] |= from;
        pieces[turn][knights] ^= to;
    }
    else {
        uint_fast8_t moving_piece = move >> 28;

        pieces[turn][moving_piece] ^= to;
        pieces[turn][moving_piece] |= from;
    }
}

inline uint64_t Board::getEnemyAttackedSquares(const bool enemy) {
    /* I unroll a for loop here for a little more performance.
    It looks bad but it saves a few cpu cycles. */

    uint_fast8_t sq;
    uint64_t opponent_attacked_squares = 0;
    uint64_t bitboard;
    uint64_t occupancy = (blackPieces() | whitePieces()) ^ pieces[!enemy][king];

    bitboard = pieces[enemy][pawns];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= pawn_moveboards[enemy][captures][sq];
        flipBit(bitboard, sq);
    }
    bitboard = pieces[enemy][knights];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= knight_moveboards[sq];
        flipBit(bitboard, sq);
    }
    bitboard = pieces[enemy][bishops];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= bishopMoves(sq, occupancy);
        flipBit(bitboard, sq);
    }
    bitboard = pieces[enemy][rooks];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= rookMoves(sq, occupancy);
        flipBit(bitboard, sq);
    }
    bitboard = pieces[enemy][queens];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= rookMoves(sq, occupancy) | bishopMoves(sq, occupancy);
        flipBit(bitboard, sq);
    }
    opponent_attacked_squares |= king_moveboards[_tzcnt_u64(pieces[enemy][king])];
    
    return opponent_attacked_squares;
}

inline uint64_t Board::getEnemyCheckingPieces(const bool enemy) {
    uint64_t attackers = 0;
    uint_fast8_t king_square = _tzcnt_u64(pieces[!enemy][king]);

    occupancy = blackPieces() | whitePieces();

    attackers |= knight_moveboards[king_square] & pieces[enemy][knights];
    attackers |= pawn_moveboards[!enemy][captures][king_square] & pieces[enemy][pawns];
    attackers |= bishopMoves(king_square, occupancy) & pieces[enemy][bishops];
    attackers |= rookMoves(king_square, occupancy) & pieces[enemy][rooks];
    attackers |= queenMoves(king_square, occupancy) & pieces[enemy][queens];

    return attackers;
}

inline uint64_t Board::whitePieces() {
    return (pieces[white][pawns] | pieces[white][knights] | pieces[white][bishops] 
            | pieces[white][rooks] | pieces[white][queens] | pieces[white][king]);
}

inline uint64_t Board::blackPieces() {
    return (pieces[black][pawns] | pieces[black][knights] | pieces[black][bishops] 
            | pieces[black][rooks] | pieces[black][queens] | pieces[black][king]); 
}

inline uint64_t Board::sidePieces(const bool side) {
    return (pieces[side][pawns] | pieces[side][knights] | pieces[side][bishops] 
            | pieces[side][rooks] | pieces[side][queens] | pieces[side][king]);
}