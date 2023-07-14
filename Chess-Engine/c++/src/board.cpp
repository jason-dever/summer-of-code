#include <string>
#include "board.h"
#include "precompute.h"

// This function sucks. Too bad!
void Board::storeFEN(const std::string FEN) {
    int sq = 63;
    int i = 0;

    while (FEN[i] != ' ') {
        if (FEN[i] < 58 && FEN[i] > 48) { // X number of empty squares
            sq -= (FEN[i] - 48);
        }
        else if (FEN[i] < 123 && FEN[i] > 96) { // Black piece
            switch (FEN[i]) {
                case 'p': setBit(b_pieces[pawns], sq); break;
                case 'n': setBit(b_pieces[knights], sq); break;
                case 'b': setBit(b_pieces[bishops], sq); break;
                case 'r': setBit(b_pieces[rooks], sq); break;
                case 'q': setBit(b_pieces[queens], sq); break;
                case 'k': setBit(b_pieces[king], sq); break;
            }
            sq--;
        }
        else if (FEN[i] < 91 && FEN[i] > 64) { // White piece
            switch (FEN[i]) {
                case 'P': setBit(w_pieces[pawns], sq); break;
                case 'N': setBit(w_pieces[knights], sq); break;
                case 'B': setBit(w_pieces[bishops], sq); break;
                case 'R': setBit(w_pieces[rooks], sq); break;
                case 'Q': setBit(w_pieces[queens], sq); break;
                case 'K': setBit(w_pieces[king], sq); break;
            }
            sq--;
        }
        i++;
    }
    i++;
    black_turn = (FEN[i] == 'b') ? true : false;
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
        int col;
        char cols[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

        for (int j = 0; j <= 7; j++) {
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

U64 Board::whitePieces() {
    return (w_pieces[pawns] | w_pieces[knights] | w_pieces[bishops] 
            | w_pieces[rooks] | w_pieces[queens] | w_pieces[king]);
}

U64 Board::blackPieces() {
    return (b_pieces[pawns] | b_pieces[knights] | b_pieces[bishops] 
            | b_pieces[rooks] | b_pieces[queens] | b_pieces[king]); 
}