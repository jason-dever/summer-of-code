#include <iostream>

#include <unordered_map>
#include <string>

int main() {
    auto num_lines = 0;
    auto len_lines = 0;

    std::cin >> num_lines >> len_lines;
    for (auto i = 0; i < num_lines; ++i) {
        std::string line;
        line.reserve(len_lines);

        std::cin >> line;

        std::unordered_map<char, int> letter_counts;
        for (auto ch : line)
            letter_counts[ch] = letter_counts[ch]+1;

        auto failed = false;
        for (auto i = 1; i < line.size(); ++i) {
            auto ch = line[i];
            auto ch_prev = line[i-1];

            if (letter_counts[ch] == 1) {
                if (letter_counts[ch_prev] == 1) {
                    std::cout << "F\n";
                    failed = true;
                    break;
                }
            }
            else {
                if (letter_counts[ch_prev] > 1) {
                    std::cout << "F\n";
                    failed = true;
                    break;
                }
            }
        }
        if (!failed)
            std::cout << "T\n";
    }
    return 0;
}
