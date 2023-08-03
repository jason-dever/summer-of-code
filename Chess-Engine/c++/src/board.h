#pragma once

#include <iostream>
#include <string>
#include "precompute.h"
#include <vector>

enum PiecesEnum {pawns, knights, bishops, rooks, queens, king};
enum ColoursEnum {white, black};

struct Board {
    bool turn = 0;

    uint64_t pieces[2][6] {0};

    uint64_t castle_squares = 0;
    uint64_t en_passant_squares = 0;

    int half_moves = 0;
    int full_moves = 0;

    void storeFEN(const std::string FEN);

    inline void genMoves();
    
    inline uint64_t whitePieces();
    inline uint64_t blackPieces();

    inline bool makeMove(unsigned int move);
    inline void unmakeMove(unsigned int move);

    private:
        const unsigned short capture_mask = 0x1000;
        const unsigned short en_passant_mask = 0x2000;

        const unsigned short queen_promo_mask = 0x8000;
        const unsigned short knight_promo_mask = 0x4000;

        const unsigned short kingcastle_mask = 0x3000;
        const unsigned short queencastle_mask = 0xF000;

        std::vector<unsigned int> moves;
        bool opponent;

        uint64_t occupancy;

        uint64_t friendly_occupancy;
        uint64_t enemy_occupancy;
        uint64_t enemy_attacked_squares;

        uint64_t legal_captures;
        uint64_t legal_pushes;

        inline uint64_t sidePieces(bool side);

        inline void genKingMoves();
        inline void genKnightMoves();
        inline void genBishopMoves();
        inline void genRookMoves();
        inline void genQueenMoves();
        inline void genPawnMoves();

        inline uint64_t getEnemyAttackedSquares();
        inline uint64_t getEnemyCheckingPieces(bool enemy);
};

inline bool Board::makeMove(unsigned int move) {
    /* This function returns a bool to differentiate between legal and illegal moves.
    The move generator will rule out most illegal moves, but some illegal moves
    including en passant edge cases and movement of pinned pieces are caught here.
    The playing algorithm will use the boolean returned by this function to
    determine whether or not to search the move tree .*/
    //TODO restore/remove other side castle in move 
    // Full moves will be incremented by 1 if black is moving and nothing if white is moving.
    full_moves += turn;

    half_moves++;

    if (move == kingcastle_mask) {
        pieces[turn][king] >>= 2;

        uint64_t castle_square = pieces[turn][king] >> 1; 

        pieces[turn][rooks] ^= castle_square;
        pieces[turn][rooks] |= (castle_square << 2);

        castle_squares ^= (0x81 << (56*turn));
        turn = !turn;

        return 1;
    }
    else if (move == queencastle_mask) {
        pieces[turn][king] <<= 2;

        uint64_t castle_square = pieces[turn][king] << 2; 

        pieces[turn][rooks] ^= castle_square;
        pieces[turn][rooks] |= (castle_square >> 3);

        castle_squares ^= (0x81 << (56*turn));
        turn = !turn;

        return 1;
    }

    int moving_piece = (move >> 16) & 0x7;
    uint64_t from = 1ULL << (move & 0x3F);
    uint64_t to = 1ULL << ( (move >> 6) & 0x3F );

    pieces[turn][moving_piece] ^= from;

    if (move & capture_mask) {
        int captured_piece = (move >> 18) & 0x7;

        pieces[!turn][captured_piece] ^= to;

        half_moves = 0;
    }
    else if (move & en_passant_mask) {
        pieces[opponent][pawns] ^= pawn_moveboards[!turn][pushes][_tzcnt_u64(en_passant_squares)];
        
        half_moves = 0;
    }
    
    if (moving_piece == pawns)
        half_moves = 0;

    if (move & queen_promo_mask) 
        moving_piece = queens;
    else if (move & knight_promo_mask) 
        moving_piece = knights;
    

    pieces[turn][moving_piece] |= to;

    turn = !turn; 

    if (getEnemyCheckingPieces(opponent)) {
        unmakeMove(move);
        return 0;
    }

    return 1;
}

inline void Board::unmakeMove(unsigned int move) {
    turn = !turn;
    full_moves -= turn;
    half_moves = move << 21;

    if (move == kingcastle_mask) {
        uint64_t castle_square = pieces[turn][king] >> 1;

        pieces[turn][king] <<= 2;

        pieces[turn][rooks] ^= (castle_square << 2);
        pieces[turn][rooks] |= castle_square;

        castle_squares |= castle_square;
        return;
    }
    else if (move == queencastle_mask) {
        uint64_t castle_square = pieces[turn][king] << 2;
        pieces[turn][king] >>= 2;

        pieces[turn][rooks] ^= (castle_square >> 3);
        pieces[turn][rooks] |= castle_square;

        castle_squares |= castle_square;
        return;
    }

    uint64_t from = 1ULL << (move & 0x3F);
    uint64_t to = 1ULL << ( (move >> 6) & 0x3F );

    if (move & capture_mask)
        pieces[!turn][move >> 18] |= to;

    else if (move & en_passant_mask) 
        pieces[!turn][pawns] |= 1ULL << (pawn_moveboards[!turn][pushes][_tzcnt_u64(to)]);
    
    if (move & queen_promo_mask) {
        pieces[turn][pawns] |= from;
        pieces[turn][queens] ^= to;
    }
    else if (move & knight_promo_mask) {
        pieces[turn][pawns] |= from;
        pieces[turn][knights] ^= to;
    }
    else {
        int moving_piece = move >> 16;

        pieces[turn][moving_piece] ^= to;
        pieces[turn][moving_piece] |= from;
    }
}

inline uint64_t Board::getEnemyAttackedSquares() {
    /* I unroll a for loop here for a little more performance.
    It looks bad but it saves a few cpu cycles. */

    int sq;
    uint64_t opponent_attacked_squares = 0;
    uint64_t bitboard;
    uint64_t occupancy = (blackPieces() | whitePieces()) ^ pieces[!opponent][king];

    bitboard = pieces[opponent][pawns];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= pawn_moveboards[opponent][captures][sq];
        flipBit(bitboard, sq);
    }
    bitboard = pieces[opponent][knights];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= knight_moveboards[sq];
        flipBit(bitboard, sq);
    }
    bitboard = pieces[opponent][bishops];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= bishopMoves(sq, occupancy);
        flipBit(bitboard, sq);
    }
    bitboard = pieces[opponent][rooks];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= rookMoves(sq, occupancy);
        flipBit(bitboard, sq);
    }
    bitboard = pieces[opponent][queens];
    while (bitboard) {
        sq = _tzcnt_u64(bitboard);
        opponent_attacked_squares |= rookMoves(sq, occupancy) | bishopMoves(sq, occupancy);
        flipBit(bitboard, sq);
    }
    opponent_attacked_squares |= king_moveboards[_tzcnt_u64(pieces[opponent][king])];
    
    return opponent_attacked_squares;
}

inline uint64_t Board::getEnemyCheckingPieces(bool enemy) {
    uint64_t attackers = 0;
    uint_fast8_t king_square = _tzcnt_u64(pieces[!enemy][king]);

    attackers |= knight_moveboards[king_square] & pieces[enemy][knights];
    attackers |= pawn_moveboards[!enemy][captures][king_square] & pieces[enemy][pawns];
    attackers |= bishopMoves(king_square, occupancy) & pieces[enemy][bishops];
    attackers |= rookMoves(king_square, occupancy) & pieces[enemy][rooks];
    attackers |= ( bishopMoves(king_square, occupancy) | rookMoves(king_square, occupancy) ) & pieces[enemy][queens];

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

inline uint64_t Board::sidePieces(bool side) {
    return (pieces[side][pawns] | pieces[side][knights] | pieces[side][bishops] 
            | pieces[side][rooks] | pieces[side][queens] | pieces[side][king]);
}