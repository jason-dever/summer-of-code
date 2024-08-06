#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using Board = std::vector<std::vector<char>>;
using Coordinate = std::pair<int, int>;
using Path = std::vector<Coordinate>;

std::string reconstructPath(Board& board, Path& path) {
    std::string word = "";
    for (auto coordinate : path) {
        word += board[coordinate.first][coordinate.second];
    }
    return word;
}

void search(Board& board, Path& path, 
            std::unordered_set<std::string>& dict, 
            std::vector<std::string>& words) {

    if (path.size() == 0) {
        for (auto y = 0; y < board.size(); ++y) {
            for (auto x = 0; x < board[0].size(); ++x) {
                /* std::cout << "searching (" << x << ", " << y << ")" << std::endl; */
                auto start = Coordinate(y, x);

                path.push_back(start);
                search(board, path, dict, words);
                /* std::cout << path.size() << std::endl; */
                path.pop_back();
            }
        }
    }
    else {
        auto current = path.back();

        // '/' Is used for squares that the algorithm can't visit. 
        // You can use this to represent a non uniform shaped board.
        if (board[current.first][current.second] == '/') {
            return;
        }

        if (path.size() >= 3) {
            auto word = reconstructPath(board, path);
            if (dict.count(word) && std::find(words.begin(), words.end(), word) == words.end()) {
                words.push_back(word);
                std::cout << "found a word: " << word << std::endl;
            }
        }

        for (auto y = current.first-1; y <= current.first+1; ++y) {
            if (y < 0 || y >= board[0].size()) continue;

            for (auto x = current.second-1; x <= current.second+1; ++x) {
                auto neighbour = Coordinate(y, x);
                if (x < 0 || x >= board.size() || std::find(path.begin(), path.end(), neighbour) != path.end()) continue;
                /* std::cout << "(" << x << ", " << y << ")" << std::endl; */

                path.push_back(neighbour);
                search(board, path, dict, words);
                path.pop_back();
            }
        }
    }
}

bool compare(const std::string& lhs, const std::string& rhs) {
    return lhs.size() < rhs.size();
}

int getScore(std::string& word) {
    switch (word.size()) {
        case 3: return 100;
        case 4: return 400;
        case 5: return 800;
        default: return 1400 + 400*(word.size()-6);
    }
}

int main() {
    const auto filename = "dictionary";
    std::string word = "";
    std::ifstream f(filename);
    std::unordered_set<std::string> dict;

    if (f.is_open()) {
        while (getline(f, word)) {
            dict.insert(word);
        }
    }

    auto size = 0;
    std::cout << "what is the size of the board?\n";
    std::cin >> size;

    Board board;
    for (auto i = 0; i < size; ++i) {
        board.push_back(std::vector<char>());

        for (auto j = 0; j < size; ++j) {
            auto ch = ' ';
            std::cin >> ch;

            board[i].push_back(ch);
        }
    }

    /* Board board = { */
    /*     {'C', 'A', 'T', 'D'}, */
    /*     {'W', 'O', 'F', 'O'}, */
    /*     {'I', 'R', 'D', 'G'}, */
    /*     {'N', 'O', 'A', 'S'}, */
    /* }; */

    std::vector<std::string> words;
    Path path;
    search(board, path, dict, words);

    std::sort(words.begin(), words.end(), compare);
    for (auto word : words) {
        std::cout << word << ' ' << getScore(word) << '\n';
    }
}
