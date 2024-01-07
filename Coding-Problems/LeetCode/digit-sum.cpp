#include <string>

using std::string;

class Solution {
public:
    string digitSum(string s, int k) {
        if (s.length() <= k)
            return s;
        
        string digits = "";

        for (auto index = 0; index < s.length(); index += k) {
            auto slice = s.substr(index, k);
            auto temp_value = 0;

            for (auto j : slice) {
                temp_value += j-'0';
            }

            digits += std::to_string(temp_value);
        }
    return digitSum(digits, k);
    }
};
