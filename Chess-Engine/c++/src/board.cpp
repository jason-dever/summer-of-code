#include <string>
#include <cstdint>
#include "board.hpp"
#include "movegen.hpp"
#include "precompute.hpp"

Board::Board(const std::string FEN) {
    storeFEN(FEN);
}

// This function sucks. Too bad!
void Board::storeFEN(const std::string FEN) {
    int16_t sq = 63;
    int16_t i = 0;

    while (FEN[i] != ' ') {
        if (FEN[i] < 58 && FEN[i] > 48) { // X number of empty squares
            sq -= (FEN[i] - 48);
        }
        else if (FEN[i] != '/') {
            switch (FEN[i]) {
                case 'p': setBit(pieces[black][pawns], sq); break;
                case 'n': setBit(pieces[black][knights], sq); break;
                case 'b': setBit(pieces[black][bishops], sq); break;
                case 'r': setBit(pieces[black][rooks], sq); break;
                case 'q': setBit(pieces[black][queens], sq); break;
                case 'k': setBit(pieces[black][king], sq); break;

                case 'P': setBit(pieces[white][pawns], sq); break;
                case 'N': setBit(pieces[white][knights], sq); break;
                case 'B': setBit(pieces[white][bishops], sq); break;
                case 'R': setBit(pieces[white][rooks], sq); break;
                case 'Q': setBit(pieces[white][queens], sq); break;
                case 'K': setBit(pieces[white][king], sq); break;

            }
            sq--;
        }
        i++;
    }
    i++;
    turn = (FEN[i] == 'b') ? true : false;
    i++;

    do {
        switch (FEN[i]) {
            case 'K': setBit(castle_squares, 0); break;
            case 'Q': setBit(castle_squares, 7); break;
            case 'k': setBit(castle_squares, 56); break;
            case 'q': setBit(castle_squares, 63); break;
         }
    i++;
    }
    while (FEN[i] != ' ');

    i++;
    if (FEN[i] < 105 && FEN[i] > 96) {
        char col = 0;
        char cols[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

        for (int_fast8_t j = 0; j <= 7; j++) {
            if (cols[j] == FEN[i]) {
                col = j;
            }
        }
        en_passant_squares = 1ULL << ((FEN[i+1]-49)*8 + (7-col));
        i += 2;
    }
    
    while (FEN[i] == '-' || FEN[i] == ' ') {
        i++;
    }

    if (FEN[i+1] != ' ') {
        half_moves = (FEN[i]-48) * 10 + (FEN[i+1] - 48);
        i += 3;
    }
    else {
        half_moves = (FEN[i] - 48);
        i += 2;
    }

    full_moves = atoi(FEN.substr(i).c_str());
}

void Board::printOut() {
    int_fast8_t rank = 8;
    std::string line = std::to_string(rank) + "    ";
    char files[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g','h'};
    char piece_characters[6] = {'p', 'n', 'b', 'r', 'q', 'k'};

    for (int_fast8_t i = 63; i >= 0; i--) {
        if (( blackPieces() & (1ULL << i) )) {
            for (int_fast8_t j = pawns; j <= king; j++) {
                if (pieces[black][j] & (1ULL << i)) {
                    line.append(std::string(1, piece_characters[j]));
                }
            }
        }
        else if (( whitePieces() & (1ULL << i) )) {
            for (int_fast8_t j = pawns; j <= king; j++) {
                if (pieces[white][j] & (1ULL << i)) {
                    line.append(std::string(1, piece_characters[j]-32));
                }
            }
        }
        else {
            line.append(".");
        }
        line.append(" ");

        if (i%8 == 0) {
            std::cout << line << "\n";
            rank--;
            line = std::to_string(rank) + "    ";
        }
    }
    std::string whose_turn = (turn) ? "black" : "white";
    std::string en_passant = "no";
    std::string castle_rights = "no";

    if (en_passant_squares) {
        uint_fast8_t index = _tzcnt_u64(en_passant_squares);
        char col = files[7-(index%8)];
        int_fast8_t row = (index/8)+1;
        en_passant = col + std::to_string(row);
    }

    if (castle_squares) {
        castle_rights = "";
        if (castle_squares & 1) castle_rights.append("K");
        if ((castle_squares >> 7) & 1) castle_rights.append("Q");
        if ((castle_squares >> 56) & 1) castle_rights.append("k");
        if (castle_squares >> 63) castle_rights.append("q");
    }

    std::cout << "\n     a b c d e f g h\n\n" << "     Turn: " << whose_turn << 
            "\n     En passant: " << en_passant << "\n     Castle rights: " << castle_rights << 
            "\n     half moves: " << half_moves << "\n     full moves: " << full_moves << "\n\n";
}

void Board::operator=(const Board original) {
    turn = original.turn;
    
    castle_squares = original.castle_squares;
    en_passant_squares = original.en_passant_squares;
    
    full_moves = original.full_moves;
    half_moves = original.half_moves;

    for (int_fast8_t colour = white; colour <= black; colour++) {
        for (int_fast8_t piece = pawns; piece <= king; piece++) {
            pieces[colour][piece] = original.pieces[colour][piece];
        }
        capture_stack[colour] = original.capture_stack[colour];
    }
}

bool Board::operator==(const Board other_board) {
    bool boards_are_different = false;

    boards_are_different |= (turn != other_board.turn);
    boards_are_different |= (castle_squares != other_board.castle_squares);
    boards_are_different |= (en_passant_squares != other_board.en_passant_squares);
    boards_are_different |= (full_moves != other_board.full_moves);
    boards_are_different |= (half_moves != other_board.half_moves);

    for (int_fast8_t colour = white; colour <= black; colour++) {
        for (int_fast8_t piece = pawns; piece <= king; piece++) {
            boards_are_different |= (pieces[colour][piece] != other_board.pieces[colour][piece]);    
        }
        boards_are_different |= (capture_stack[colour] != other_board.capture_stack[colour]);
    }
    return !boards_are_different;
}