#include <string>
#include <cstdint>
#include "board.h"
#include "movegen.h"
#include "precompute.h"

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
        char col;
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