#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using Board = std::vector<std::vector<char>>;
using Coordinate = std::pair<int, int>;
using Path = std::vector<Coordinate>;

void search(Board& board, Path& path, 
            std::unordered_set<std::string>& dict, 
            std::vector<std::string>& words, std::string& word) {

    // This restriction keeps runtime low on larger boards at the cost
    // of potentially missing out on extremely long words.
    if (path.size() > 11) return;

    // if (path.size() > 12) {
    //     auto worth_searching = false;
    //     for (auto potential_word : dict) {
    //         if (potential_word.starts_with(word)) {
    //             worth_searching = true;
    //             break;
    //         }
    //     }
    //     if (!worth_searching) return;
    // }

    if (path.size() == 0) {
        for (auto y = 0; y < board.size(); ++y) {
            for (auto x = 0; x < board[0].size(); ++x) {
                auto start = Coordinate(y, x);

                path.push_back(start);

                word += board[start.first][start.second];
                search(board, path, dict, words, word);
                path.pop_back();
                word.pop_back();
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
            if (dict.count(word) && std::find(words.begin(), words.end(), word) == words.end()) {
                words.push_back(word);
                /* std::cout << "found a word: " << word << std::endl; */
            }
        }

        for (auto y = current.first-1; y <= current.first+1; ++y) {
            if (y < 0 || y >= board[0].size()) continue;

            for (auto x = current.second-1; x <= current.second+1; ++x) {
                auto neighbour = Coordinate(y, x);
                if (x < 0 || x >= board.size() || std::find(path.begin(), path.end(), neighbour) != path.end()) continue;

                path.push_back(neighbour);
                word += board[neighbour.first][neighbour.second];

                search(board, path, dict, words, word);
                path.pop_back();
                word.pop_back();
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
            unsigned char ch = ' ';
            std::cin >> ch;

            board[i].push_back(toupper(ch));
        }
    }

    std::vector<std::string> words;
    Path path;
    std::string s = "";

    auto start = std::chrono::high_resolution_clock::now();

    search(board, path, dict, words, s);
    std::sort(words.begin(), words.end(), compare);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

    for (auto word : words) {
        std::cout << word << ' ' << getScore(word) << '\n';
    }
    std::cout << words.size() << " words found in " << duration << '\n';

    return 0;
}
