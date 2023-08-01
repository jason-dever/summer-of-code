#pragma once

#include <string>
#include "precompute.h"
#include <vector>

enum PiecesEnum {pawns, knights, bishops, rooks, queens, king};
enum ColoursEnum {white, black};

struct Board {
    bool turn;

    U64 pieces[2][6] {0};

    U64 castle_squares = 0;
    U64 en_passant_squares = 0;

    int half_moves = 0;
    int full_moves = 0;

    void storeFEN(const std::string FEN);

    inline void genMoves();
    
    inline U64 whitePieces();
    inline U64 blackPieces();

    private:
        std::vector<unsigned short> moves;
        bool opponent;

        U64 occupancy;

        U64 capture_stack[2] {0};

        U64 friendly_occupancy;
        U64 enemy_occupancy;
        U64 enemy_attacked_squares;

        U64 legal_captures;
        U64 legal_pushes;

        U64 horizontal_pins;
        U64 diagonal_pins;

        U64 moveboard;
        unsigned short move;

        int from_square;
        int to_square;

        inline U64 sidePieces(bool side);

        inline bool makeMove(unsigned short move);
        inline void unmakeMove(unsigned short move);

        inline void genKingMoves();
        inline void genKnightMoves();
        inline void genBishopMoves();
        inline void genRookMoves();
        inline void genQueenMoves();
        inline void genPawnMoves();

        inline U64 getEnemyAttackedSquares();
        inline U64 getEnemyCheckingPieces(bool enemy);
};

inline U64 Board::whitePieces() {
    return (pieces[white][pawns] | pieces[white][knights] | pieces[white][bishops] 
            | pieces[white][rooks] | pieces[white][queens] | pieces[white][king]);
}

inline U64 Board::blackPieces() {
    return (pieces[black][pawns] | pieces[black][knights] | pieces[black][bishops] 
            | pieces[black][rooks] | pieces[black][queens] | pieces[black][king]); 
}

inline U64 Board::sidePieces(bool side) {
    return (pieces[side][pawns] | pieces[side][knights] | pieces[side][bishops] 
            | pieces[side][rooks] | pieces[side][queens] | pieces[side][king]);
}