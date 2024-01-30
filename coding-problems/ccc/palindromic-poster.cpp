#include <iostream>
#include <string>
#include <vector>

inline int countPalindromicRows(std::vector<std::vector<char>>& poster) {
    auto num_palindromic_rows = 0;

    for (auto row : poster) {
        bool is_palindromic_row = true;

        auto left = 0;
        auto right = row.size()-1;

        while (left < right) {
            is_palindromic_row &= (row[left] == row[right]);

            ++left;
            --right;
        }
        num_palindromic_rows += is_palindromic_row;
    }
    return num_palindromic_rows;
}

inline int countPalindromicCols(std::vector<std::vector<char>>& poster) {
    auto num_palindromic_cols = 0;

    auto num_cols = poster[0].size();
    for (auto i = 0; i < num_cols; i++) {
        bool is_palindromic_col = true;

        auto top = 0;
        auto bottom = poster.size()-1;

        while (top < bottom) {
            is_palindromic_col &= (poster[top][i] == poster[bottom][i]);
            
            ++top;
            --bottom;
        }
        num_palindromic_cols += is_palindromic_col;
    }
    return num_palindromic_cols;
}

bool isValidPoster(std::vector<std::vector<char>>& poster, 
                   int target_valid_rows, 
                   int target_valid_cols) {

    return target_valid_rows == countPalindromicRows(poster)
        && target_valid_cols == countPalindromicCols(poster);
}

std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

bool searchPosters(int depth, std::vector<std::vector<char>>& poster, 
                       int target_rows, int target_cols) {

    int num_rows = poster.size();
    int num_cols = poster[0].size();

    if (depth == (num_rows*num_cols)) {
        if (isValidPoster(poster, target_rows, target_cols)) {
            for (auto row : poster) {
                for (auto ch : row) {
                    std::cout << ch;
                }
                std::cout << '\n';
            }
            return true;
        }
        else {
            return false;
        }
    }

    auto row = depth/num_cols;
    auto col = depth%num_cols;

    bool found_valid_poster = false;
    for (auto letter : alphabet) {
        poster[row][col] = letter;
        if (searchPosters(depth+1, poster, target_rows, target_cols))
            return true;
    }
    return false;
}

int main() {
    auto num_rows = 0, num_cols = 0, target_rows = 0, target_cols = 0;
    std::cin >> num_rows;
    std::cin >> num_cols;
    std::cin >> target_rows;
    std::cin >> target_cols;

    std::vector<std::vector<char>> poster(num_rows);
    for (auto& vec : poster)
        vec.resize(num_cols);

    if (!searchPosters(0, poster, target_rows, target_cols))
        std::cout << "IMPOSSIBLE\n";
    return 0;
}
