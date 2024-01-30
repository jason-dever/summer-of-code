#include <climits>
#include <vector>
#include <string>

class Solution {
public:
    std::string minWindow(std::string s, std::string t) {
        std::vector<int> char_frequencies(128);

        for (auto ch : t)
             ++char_frequencies[ch];

        auto chars_missing = t.size();

        auto min_left = 0;
        auto min_window_size = INT_MAX;

        auto left = 0;
        auto right = 0;

        while (right < s.size()) {
            auto ch = s[right];

            if (char_frequencies[ch] > 0) {
                --chars_missing;
            }
            --char_frequencies[ch];
            ++right;

            while (chars_missing == 0) {
                ch = s[left];
                auto window_size = right-left;

                if (window_size < min_window_size) {
                    min_window_size = window_size;
                    min_left = left;
                }
                if (char_frequencies[ch] == 0)
                    ++chars_missing;

                ++char_frequencies[ch];
                ++left;
            }
        }
        return min_window_size == INT_MAX ? "" : s.substr(min_left, min_window_size);
    }
};
