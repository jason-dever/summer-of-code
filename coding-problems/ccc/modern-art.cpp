#include <algorithm>
#include <iostream>
#include <vector>

#define BLACK 0
#define GOLD 1

inline void swapRow(std::vector<std::vector<bool>>& canvas, int idx) {
    std::vector<bool>& row = canvas[idx];

    for (int i = 0; i < row.size(); ++i) {
        row[i] = !row[i];
    }
}

inline void swapCol(std::vector<std::vector<bool>>& canvas, int idx) {
    for (auto& row : canvas) {
        row[idx] = !row[idx];
    }
}

int main() {
    auto num_rows = 0;
    auto num_cols = 0;
    std::cin >> num_rows >> num_cols;

    std::vector<std::vector<bool>> canvas(num_rows);
    for (auto& row : canvas) {
        row.resize(num_cols);
        std::fill(row.begin(), row.end(), BLACK);
    }

    auto num_changes = 0;
    std::cin >> num_changes;

    for (auto i = 0; i < num_changes; ++i) {
        char type = 0;
        int idx_changed = 0;

        std::cin >> type >> idx_changed;
        --idx_changed;

        switch (type) {
            case 'R':
                swapRow(canvas, idx_changed);
                break;
            case 'C':
                swapCol(canvas, idx_changed);
                break;
            default:
                std::cerr << "got a type that isn't row or column\n";
                break;
        }
    }

    auto num_gold_squares = 0;
    for (auto row : canvas) {
        num_gold_squares += std::count(row.begin(), row.end(), GOLD);
    }
    std::cout << num_gold_squares << '\n';

    return 0;
}
