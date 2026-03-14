#include <vector>
#include <array>
#include <iostream>

static auto num_rings = 4;
using Board = std::array<std::vector<int>, 3>;
enum Tower {first, second, third};

void print_board(Board& board) {
    for (auto tower : board) {
        for (auto ring : tower) {
            std::cout << ring << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void construct_board(Board& board) {
    for (auto i = 0; i < num_rings; ++i) {
        board.at(first).push_back(num_rings-i-1);
    }
}

void move(Board& board, Tower from, Tower to, int num_to_move) {
    if (num_to_move == 0) { // Reach leaf of move tree
        return;
    }
    print_board(board);
    if (board.at(from).empty()) {
        puts("removing from empty tower");
        return;
    }
    if (!board.at(to).empty() && board.at(from).back() > board.at(to).back()) {
        std::cout << "invalid move " << to << " " << from << " " << num_to_move << std::endl;
        return;
    }
    if (num_to_move == 1) {
        // std::cout << "here" << std::endl;
        auto ring = board.at(from).back();
        board.at(from).pop_back();
        board.at(to).push_back(ring);
    }
    else {
        Tower other = static_cast<Tower>(((~from)^to) & 0x3); // Gives the third tower; other is not from or to.
        move(board, from, other, num_to_move-1);
        move(board, from, to, 1);
        move(board, other, to, num_to_move-1);
    }
}

int main() {
    Board board;
    construct_board(board);
    // print_board(board);
    move(board, first, third, num_rings);
    // move(board, first, second, 3);
    print_board(board);
    return 0;
}
