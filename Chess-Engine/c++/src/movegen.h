#include "precompute.h"
#include "board.h"
#include <intrin.h>
#include <immintrin.h>

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

extern U64 rook_moveboards[102400];
extern U64 bishop_moveboards[5248];
extern U64 knight_moveboards[64];
extern U64 pawn_moveboards[2][2][64];
extern U64 king_moveboards[64];

inline bool Board::makeMove(unsigned short move) {
    /* This function returns a bool to differentiate between legal and illegal moves.
    The move generator will rule out most illegal moves, but some illegal moves
    including en passant edge cases and movement of pinned pieces are caught here.
    The playing algorithm will use the boolean returned by this function to
    determine whether or not to search the move tree .*/

    // This if statement rules out both kingside and queenside castles.
    if (move == 0x3000) {
        pieces[turn][king] >>= 2;

        U64 castle_square = pieces[turn][king] >> 1; 

        pieces[turn][rooks] ^= castle_square;
        pieces[turn][rooks] |= (castle_square << 2);

        castle_squares ^= castle_square;
        return 1;
    }
    else if (move == 0xF000) {
        pieces[turn][king] <<= 2;

        U64 castle_square = pieces[turn][king] << 2; 

        pieces[turn][rooks] ^= castle_square;
        pieces[turn][rooks] |= (castle_square >> 3);

        castle_squares ^= castle_square;
        return 1;
    }

    int moving_piece;
    U64 from = 1ULL << (move & 0x3F);
    U64 to = 1ULL << ( (move >> 6) & 0x3F );

    for (moving_piece = pawns; moving_piece <= king; moving_piece++) {
        if (from & pieces[turn][moving_piece])
            break;
    }

    pieces[turn][moving_piece] ^= from;

    if (move & 0x1000) {
        for (int captured_piece = pawns; captured_piece < king; captured_piece++) {
            if (to & pieces[turn][captured_piece]) {

                pieces[!turn][captured_piece] ^= to;

                capture_stack[turn] <<= 3;
                capture_stack[turn] |= captured_piece;
                break;
            }
        }
    }
    else if (move & 0x2000) {
        pieces[opponent][pawns] ^= pawn_moveboards[!turn][pushes][_tzcnt_u64(en_passant_squares)];
        capture_stack[turn] <<= 3;
        en_passant_squares = 0;
    }
    
    if (move & 0x8000) {
        moving_piece = queens;
    }
    else if (move & 0x4000) {
        moving_piece = knights;
    }

    pieces[turn][moving_piece] |= to;

    turn = !turn;

    if (getEnemyCheckingPieces(opponent)) {
        unmakeMove(move);
        return 0;
    }

    return 1;
}

inline void Board::unmakeMove(unsigned short move) {

}

inline void Board::genMoves() {
    legal_captures = __UINT64_MAX__;
    legal_pushes = __UINT64_MAX__;

    opponent = !turn;

    friendly_occupancy = sidePieces(turn);
    enemy_occupancy = sidePieces(opponent);
    occupancy = friendly_occupancy | enemy_occupancy;

    U64 checking_pieces = getEnemyCheckingPieces(opponent);
    int num_checking_pieces = __popcnt64(checking_pieces);

    genKingMoves();

    if (num_checking_pieces == 1) {
        int checker_square = _tzcnt_u64(checking_pieces);
        legal_captures = checking_pieces;
        legal_pushes = 0;

        // from_square will still be the king square from when genKingMoves() was called.
        if (((checker_square%8) == (from_square%8)) || (checker_square/8) == (from_square/8)) {
            legal_pushes = rookMoves(from_square, occupancy) & rookMoves(checker_square, occupancy);
        }
        else if (checking_pieces & pieces[opponent][bishops] || checking_pieces & pieces[opponent][queens]) {
            legal_pushes = bishopMoves(from_square, occupancy) & bishopMoves(checker_square, occupancy);
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

const U64 castling[2][2] = { {0x1, 0x8}, {0x8000000000000000, 0x0100000000000000} };
const U64 castle_through_squares[2][2] = { {0x7, 0x38}, {0x0700000000000000, 0x3800000000000000} };

inline void Board::genKingMoves() {
    from_square = _tzcnt_u64(pieces[turn][king]);
    U64 enemy_attacked_squares = getEnemyAttackedSquares();

    moveboard = ( king_moveboards[from_square] 
        & ~( enemy_attacked_squares | friendly_occupancy ) ) & (legal_captures | legal_pushes);

    while (moveboard) {
        to_square = _tzcnt_u64(moveboard);

        /* If the to square is occupied, the move will be flagged as a capture. 
        If not, it's a quiet move. */
        move = (from_square) | (to_square << 6) | 
            ( ( _pext_u64( enemy_occupancy, 1ULL << to_square ) ) << 12 );
            
        moves.push_back(move);
        flipBit(moveboard, to_square);
    }

    if ( castle_squares & castling[turn][0] && !((enemy_attacked_squares | occupancy) & castle_through_squares[turn][0]) ) {
        moves.push_back(0x3000);
    }
    if ( castle_squares & castling[turn][1] && !((enemy_attacked_squares | occupancy) & castle_through_squares[turn][1]) ) {
        moves.push_back(0xF000);
    }
}

inline void Board::genKnightMoves() {
    U64 friendly_knights = pieces[turn][knights];

    while (friendly_knights) {
        from_square = _tzcnt_u64(friendly_knights);

        moveboard = ( knight_moveboards[from_square] 
            & ~friendly_occupancy ) & (legal_captures | legal_pushes);

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            move = (from_square) | (to_square << 6) | 
                ( ( _pext_u64( enemy_occupancy, 1ULL << to_square ) ) << 12 );
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_knights, from_square);
    }
}

inline void Board::genBishopMoves() {
    U64 friendly_bishops = pieces[turn][bishops];

    while (friendly_bishops) {
        from_square = _tzcnt_u64(friendly_bishops);

        moveboard = bishopMoves(from_square, occupancy) & (legal_captures | legal_pushes);

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            move = (from_square) | (to_square << 6) | 
                ( ( _pext_u64( enemy_occupancy, 1ULL << to_square ) ) << 12 );
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_bishops, from_square);
    }
}

inline void Board::genRookMoves() {
    U64 friendly_rooks = pieces[turn][rooks];

    while (friendly_rooks) {
        from_square = _tzcnt_u64(friendly_rooks);

        moveboard = rookMoves(from_square, occupancy) & (legal_captures | legal_pushes);

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            move = (from_square) | (to_square << 6) | 
                ( ( _pext_u64( enemy_occupancy, 1ULL << to_square ) ) << 12 );
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_rooks, from_square);
    }
}

inline void Board::genQueenMoves() {
    U64 friendly_queens = pieces[turn][queens];

    while (friendly_queens) {
        from_square = _tzcnt_u64(friendly_queens);

        moveboard = ( rookMoves(from_square, occupancy) | bishopMoves(from_square, occupancy) ) & (legal_captures | legal_pushes);

        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);

            move = (from_square) | (to_square << 6) | 
                ( ( _pext_u64( enemy_occupancy, 1ULL << to_square ) ) << 12 );
            
            moves.push_back(move);
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_queens, from_square);
    }
}

const U64 promotion_mask = 0xFF000000000000FF;

inline void Board::genPawnMoves() {
    /* This variable saves doing the to_square << 6 operation more than once.
    It's used in this function and not others because in the other movegen functions
    the to_square << 6 operation to place to_square into the move is only performed once. */
    U64 to_square_in_move;

    U64 friendly_pawns = pieces[turn][pawns];
    U64 capture_board;
    U64 en_passant;
    U64 to_square_bitboard;

    while (friendly_pawns) {
        from_square = _tzcnt_u64(friendly_pawns);

        capture_board = pawn_moveboards[turn][captures][from_square] & (enemy_occupancy | en_passant_squares)
            & (legal_captures | legal_pushes);
        moveboard = pawn_moveboards[turn][pushes][from_square] & ~(occupancy) & legal_pushes;

        while (capture_board) {
            to_square = _tzcnt_u64(capture_board);
            to_square_bitboard = 1ULL << to_square;
            to_square_in_move = to_square << 6;

            /* If to square & promotion_mask is > 0 then the move is a promotion.
            We need to make two moves; one for knight promotion and one for queen promotion.
            * This might mess with perft numbers because we don't generate moves for rook/bishop promotions. */

            if ( to_square_bitboard & promotion_mask ) {
                move = (from_square) | to_square_in_move | 0x9000;
                moves.push_back(move);

                move = (from_square) | to_square_in_move | 0x5000;
                moves.push_back(move);
            }
            else {
                en_passant = ( _pext_u64(en_passant_squares, to_square_bitboard) ) << 13;

                move = (from_square) | to_square_in_move | 0x1000 | en_passant;

                moves.push_back(move);
            }
            flipBit(capture_board, to_square);
        }
        while (moveboard) {
            to_square = _tzcnt_u64(moveboard);
            to_square_in_move = to_square << 6;

            if ( (1ULL << to_square) & promotion_mask ) {
                move = (from_square) | to_square_in_move | 0x8000;
                moves.push_back(move);

                move = (from_square) | to_square_in_move | 0x4000;
                moves.push_back(move);
            }
            else {
                move = (from_square) | to_square_in_move; // Quiet move

                moves.push_back(move);
            }
            flipBit(moveboard, to_square);
        }
        flipBit(friendly_pawns, from_square);
    }
}

inline U64 Board::getEnemyAttackedSquares() {
    /* I unroll a for loop here for a little more performance.
    It looks bad but it saves a few cpu cycles. */

    int sq;
    U64 opponent_attacked_squares = 0;
    U64 bitboard;
    U64 occupancy = (blackPieces() | whitePieces()) ^ pieces[!opponent][king];

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

inline U64 Board::getEnemyCheckingPieces(bool enemy) {
    U64 attackers = 0;
    unsigned int king_square = _tzcnt_u64(pieces[!enemy][king]);

    attackers |= knight_moveboards[king_square] & pieces[enemy][knights];
    attackers |= pawn_moveboards[!enemy][captures][king_square] & pieces[enemy][pawns];
    attackers |= bishopMoves(king_square, occupancy) & pieces[enemy][bishops];
    attackers |= rookMoves(king_square, occupancy) & pieces[enemy][rooks];
    attackers |= ( bishopMoves(king_square, occupancy) | rookMoves(king_square, occupancy) ) & pieces[enemy][queens];

    return attackers;
}