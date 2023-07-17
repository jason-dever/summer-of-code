#include "test.h"
#include "precompute.h"
#include <iostream>
#include <string>
#include <intrin.h>

using std::cout;

void printBitboard(U64 bitboard) {
    int rank = 8;
    std::string line = std::to_string(rank) + "    ";;

    for (int i = 63; i >= 0; i--) {
        if (bitboard >> i & 0x1) {
            line.append("1 ");
        }
        else {   
            line.append("0 ");
        }

        if (i%8 == 0) {
            cout << line << "\n";
            rank--;
            line = std::to_string(rank) + "    ";;
        }
    }
    cout << "\n     a b c d e f g h";
    cout << "\n     Bitset value: " << bitboard << "d\n\n";
}

void printBoard(Board board) {
    int rank = 8;
    std::string line = std::to_string(rank) + "    ";
    char files[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g','h'};
    char pieces[6] = {'p', 'n', 'b', 'r', 'q', 'k'};

    for (int i = 63; i >= 0; i--) {
        if (( board.blackPieces() & (1ULL << i) ) != 0) {
            for (int j = pawns; j <= king; j++) {
                if (board.pieces[black][j] & (1ULL << i)) {
                    line.append(std::string(1, pieces[j]));
                }
            }
        }
        else if (( board.whitePieces() & (1ULL << i) ) != 0) {
            for (int j = pawns; j <= king; j++) {
                if (board.pieces[white][j] & (1ULL << i)) {
                    line.append(std::string(1, pieces[j]-32));
                }
            }
        }
        else {
            line.append(".");
        }
        line.append(" ");

        if (i%8 == 0) {
            cout << line << "\n";
            rank--;
            line = std::to_string(rank) + "    ";
        }
    }
    std::string turn = (board.turn) ? "black" : "white";
    std::string en_passant = "no";
    std::string castle_rights = "no";

    if (board.en_passant_squares) {
        unsigned long index;
        _BitScanForward64(&index, board.en_passant_squares);
        char col = files[7-(index%8)];
        int row = (index/8)+1;
        en_passant = col + std::to_string(row);
    }

    if (board.castle_squares) {
        castle_rights = "";
        if (board.castle_squares & 0x1) castle_rights.append("K");
        if (board.castle_squares >> 7 & 0x1) castle_rights.append("Q");
        if (board.castle_squares >> 56 & 0x1) castle_rights.append("k");
        if (board.castle_squares >> 63) castle_rights.append("q");
    }

    cout << "\n     a b c d e f g h\n\n" << "     Turn: " << turn << 
            "\n     En passant: " << en_passant << "\n     Castle rights: " << castle_rights << 
            "\n     half moves: " << board.half_moves << "\n     full moves: " << board.full_moves << "\n\n";
}