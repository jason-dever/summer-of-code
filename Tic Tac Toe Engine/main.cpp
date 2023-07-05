#include <iostream>
#include <raylib.h>
#include <intrin.h>
#include <map>

using namespace std;

#define GREY CLITERAL(Color){ 30, 30, 30, 255 }

const unsigned short win_positions[8] { 0x1C0, 0x38, 0x7, 0x124, 0x92, 0x49, 0x111, 0x54 };
map<unsigned int, unsigned short> ttable;

Rectangle sq_tl { 65.0f, 65.0f, 200.0f, 200.0f };
Rectangle sq_tm { 65.0f, 275.0f, 200.0f, 200.0f };
Rectangle sq_tr { 65.0f, 485.0f, 200.0f, 200.0f };

Rectangle sq_ml { 275.0f, 65.0f, 200.0f, 200.0f };
Rectangle sq_mm { 275.0f, 275.0f, 200.0f, 200.0f };
Rectangle sq_mr { 275.0f, 485.0f, 200.0f, 200.0f };

Rectangle sq_bl { 485.0f, 65.0f, 200.0f, 200.0f };
Rectangle sq_bm { 485.0f, 275.0f, 200.0f, 200.0f };
Rectangle sq_br { 485.0f, 485.0f, 200.0f, 200.0f };

Rectangle sq_bounds[9] { sq_br, sq_bm, sq_bl, sq_mr, sq_mm, sq_ml, sq_tr, sq_tm, sq_tl };

void drawBoard(unsigned int board, Texture2D x, Texture2D o);

int miniMax(unsigned int board);

bool isDraw(unsigned int board);
bool isXWin(unsigned int board);
bool isOWin(unsigned int board);

void makeMove(unsigned int* board, unsigned short move);
void unmakeMove(unsigned int* board, unsigned short move);

int getBestMoveIndex(int scores[9], int highest_move_index, bool o_turn);

int main() {
    unsigned int board {0};
    bool is_engine_turn {1};
    bool can_play_moves {1};

    Vector2 mouse_point { 0.0f, 0.0f };

    const short screenWidth = 750;
    const short screenHeight = 750;

    miniMax(board);

    InitWindow(screenWidth, screenHeight, "Tic Tac Toe");

    // We load as image first and load textures from the images after resizing
    // because you cannot resize textures.

    Image img_o {LoadImage("o.png")};
    Image img_x {LoadImage("x.png")};
    Image img_light_x {LoadImage("x_tprnt.png")};
    Image img_light_o {LoadImage("o_tprnt.png")};

    ImageResize(&img_x, 140, 140);
    ImageResize(&img_o, 140, 140);
    ImageResize(&img_light_x, 140, 140);
    ImageResize(&img_light_o, 140, 140);

    Texture2D x {LoadTextureFromImage(img_x)};
    Texture2D o {LoadTextureFromImage(img_o)};
    Texture2D light_x {LoadTextureFromImage(img_light_x)};
    Texture2D light_o {LoadTextureFromImage(img_light_o)};

    UnloadImage(img_x);
    UnloadImage(img_o);
    UnloadImage(img_light_x);
    UnloadImage(img_light_o);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        mouse_point = GetMousePosition();

        BeginDrawing();

        ClearBackground(GREY);

        if (is_engine_turn) {
            makeMove(&board, ttable[board]);
            is_engine_turn = false;
        }

        if (can_play_moves) {
            for (int i {0}; i < 9; i++) {
                if (CheckCollisionPointRec(mouse_point, sq_bounds[i])) {
                    if ( ( ( board & (1U << i) ) == 0 ) && ( board & ( 1U << (i + 9) ) ) == 0  ) { // Square is unoccupied
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            if ((board >> 18) == 1) { // o turn
                                makeMove(&board, ( ( 1U << i ) | (1U << 9) )); 
                            }
                            else {
                                makeMove(&board, (1U << i));
                            }
                            is_engine_turn = true;
                        }
                        else {
                            if ((board >> 18) == 1) { 
                                DrawTexture(light_o, sq_bounds[i].x + 30, sq_bounds[i].y + 30, RED);
                            }
                            else {
                                DrawTexture(light_x, sq_bounds[i].x + 30, sq_bounds[i].y + 30, BLUE);
                            }
                        }
                    }
                    break;
                }
            }
        }

        DrawRectangle(65, 265, 620, 10, WHITE);
        DrawRectangle(65, 475, 620, 10, WHITE);

        DrawRectangle(265, 65, 10, 620, WHITE);
        DrawRectangle(475, 65, 10, 620, WHITE);

        drawBoard(board, x, o);

        if (can_play_moves) {
            if (isXWin(board)) {
                cout << "x won!" << endl;
                can_play_moves = false;
            }
            else if (isOWin(board)) {
                cout << "o won!" << endl;
                can_play_moves = false;
            }
            else if (isDraw(board)) {
                cout << "draw!" << endl;
                can_play_moves = false;
            }
        }

        EndDrawing();
    }

    UnloadTexture(x);
    UnloadTexture(o);
    UnloadTexture(light_x);
    UnloadTexture(light_o);
    CloseWindow();

    return 0;
}

void drawBoard(unsigned int board, Texture2D x, Texture2D o) {
    unsigned long index;

    while (board != 0) {
        _BitScanForward(&index, board);

        if (index <= 8) {
            DrawTexture(o, sq_bounds[index].x + 30, sq_bounds[index].y + 30, RED);
        }
        else if (index >= 9 && index <= 17 ) {
            DrawTexture(x, sq_bounds[index-9].x + 30, sq_bounds[index-9].y + 30, BLUE);
        }
        board ^= (1U << index);
    }
}

int miniMax(unsigned int board) {
    /* Minimax: algorithm determines the best move on the board
     against a perfect opponent by recursively playing against itself
     and only using the score of the opponent's best move in move evaluation.
     This implementation also stores the best move into the map ttable. */

    bool o_turn { static_cast<bool>(board >> 18) };
    unsigned short legal_moves { static_cast<unsigned short>( (~((board | (board >> 9)))) & 0x1FF ) };
    unsigned short move;
    int scores[9] {0};
    int played_moves[9] {0};
    int highest_move_index {-1};
    unsigned long lsb_index;
    int best_move_index;

    if (isOWin(board)) {
        return -10;
    }
    if (isXWin(board)) {
        return 10;
    }
    if (isDraw(board)) {
        return 0;
    } 

    while (legal_moves != 0) {
        highest_move_index++;

        move = (o_turn << 9);
        _BitScanForward(&lsb_index, legal_moves);
        move |= (1U << lsb_index);

        makeMove(&board, move);

        played_moves[highest_move_index] = move;
        scores[highest_move_index] = miniMax(board);

        unmakeMove(&board, move);
        legal_moves ^= (1UL << lsb_index);
    }
    best_move_index = getBestMoveIndex(scores, highest_move_index, o_turn);

    ttable[board] = played_moves[best_move_index];
    return scores[best_move_index];
}

bool isDraw(unsigned int board) {
    if ( ( ( board | (board >> 9) ) & 0x1FF ) == 0x1FF) {
        return true;
    }
    return false; 
}

bool isXWin(unsigned int board) {
    for (unsigned short i : win_positions) {
        if ( ( i & (board >> 9) ) == i ) {
            return true;
        }
    }
    return false;
}

bool isOWin(unsigned int board) {
    for (unsigned short i : win_positions) {
        if ( ( i & (board) ) == i ) {
            return true;
        }
    }
    return false;
}

void makeMove(unsigned int* board, unsigned short move) {
    if ( (move & (1U << 9)) == (1U << 9) ) { // o move
        move ^= (1U << 9);
        *board |= move;
    }
    else {
        *board |= (move << 9);
    }
    *board ^= (1U << 18);
}

void unmakeMove(unsigned int* board, unsigned short move) {
    if ( (move & (1U << 9)) == (1U << 9) ) { // o move
        move ^= 1U << 9;
        *board ^= move;
    }
    else {
        *board ^= (move << 9);
    }
    *board ^= (1U << 18);
}

int getBestMoveIndex(int scores[9], int highest_move_index, bool o_turn) {
    int best_score {0};
    int best_move_index;

    // We perform different calculations for x and o because
    // o wants to minimize the score and x wants to maximize the score.

    if (o_turn) {
        best_score = 11;
        for (int i {0}; i <= highest_move_index; i++) {
            if (scores[i] < best_score) {
                best_move_index = i;
                best_score = scores[i];
            }
        }
    }
    else {
        best_score = -11;
        for (int i {0}; i <= highest_move_index; i++) {
            if (scores[i] > best_score) {
                best_move_index = i;
                best_score = scores[i];
            }
        }
    }  
    return best_move_index;
}