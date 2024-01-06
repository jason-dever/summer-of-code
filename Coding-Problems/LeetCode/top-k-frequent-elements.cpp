#include <algorithm>
#include <utility>
#include <unordered_map>
#include <vector>

class Solution {
public:
    std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> frequencies;

        for (auto num : nums) 
            ++frequencies[num];

        std::vector<std::pair<int, int>> pair_frequencies;

        for (auto& [key, value] : frequencies) {
            pair_frequencies.push_back(std::pair(value, key)); 
        }
        std::sort(pair_frequencies.rbegin(), pair_frequencies.rend());
    
        std::vector<int> output;
        for (int i = 0; i < k; i++)
            output.push_back(pair_frequencies[i].second); 

        return output;
    }
};
