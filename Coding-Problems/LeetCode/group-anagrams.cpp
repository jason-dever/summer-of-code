#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>

class Solution {
public:
    std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
        std::unordered_map<std::string, std::vector<std::string>> groups;

        for (auto word : strs) {
            auto temp = word;
            std::sort(temp.begin(), temp.end());

            groups[temp].push_back(word);
        }

        std::vector<std::vector<std::string>> output;
        for (auto& [key, value] : groups) {
             output.push_back(value);
        }
        return output;
    }
};
