#include <deque>
#include <string>

class Solution {
public:
    inline bool charNotInDeque(char ch, std::deque<char>& deque) {
        for (auto element : deque)
            if (ch == element) return false;
        return true;
    }
    int lengthOfLongestSubstring(std::string s) {
        int max_size = 0;
        std::deque<char> substr { };

        int left = 0;
        int right = 0;
        while (right < s.length()) {
            for (; charNotInDeque(s[right], substr) && right < s.length(); right++) {
                substr.push_back(s[right]);
            }
            max_size = std::max<size_t>(substr.size(), max_size);

            left++;
            substr.pop_front();
        }
        return max_size;
    }
};
