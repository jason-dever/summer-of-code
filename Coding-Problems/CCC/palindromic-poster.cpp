#include <vector>
#include <iostream>

bool isValidPoster(std::vector<std::vector<char>>& poster, 
                   int target_valid_rows, 
                   int target_valid_cols) {

    auto num_valid_cols = 0;
    auto num_valid_rows = 0;

    for (auto row : poster) {
        bool is_valid_row = true;

        auto left = 0;
        auto right = row.size()-1;

        while (left < right) {
            is_valid_row &= (row[left] == row[right]);

            ++left;
            --right;
        }
        num_valid_rows += is_valid_row;
    }
    auto num_cols = poster[0].size();
    for (auto i = 0; i < num_cols; i++) {
        bool is_valid_col = true;

        auto top = 0;
        auto bottom = poster.size()-1;

        while (top < bottom) {
            is_valid_col &= (poster[top][i] == poster[bottom][i]);
            
            ++top;
            --bottom;
        }
        num_valid_cols += is_valid_col;
    }
    return (num_valid_cols == target_valid_cols)
        && (num_valid_rows == target_valid_rows);
}

void palindromicPoster() {
    auto num_rows = 0, num_cols = 0, valid_rows = 0, valid_cols = 0;


}

int main() {
    std::vector<std::vector<char>> poster = { {'u', 'n', 'i', 'o', 'n'}, 
                                              {'r', 'a', 'd', 'a', 'r'}, 
                                              {'b', 'a', 'd', 'g', 'e'},
                                              {'a', 'n', 'i', 'm', 'e'} };

    std::cout << isValidPoster(poster, 1, 2) << "\n";
    return 0;
}
