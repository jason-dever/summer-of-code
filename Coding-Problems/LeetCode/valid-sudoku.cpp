#include <vector>
#include <string>
#include <unordered_map>

class Solution {
public:
    bool isValidSudoku(std::vector<std::vector<char>>& board) {
        std::unordered_map<char, bool> rows[9];
        std::unordered_map<char, bool> cols[9];
        std::unordered_map<char, bool> squares[9];

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                char ch = board[row][col];

                if (ch != '.') {
                    if (squares[(col/3)+(row/3)*3][ch] || cols[col][ch] || rows[row][ch])
                        return false;

                    squares[(col/3)+(row/3)*3][ch] = true;
                    cols[col][ch] = true;
                    rows[row][ch] = true;
                }
            }
        }
        return true;
    }
};
