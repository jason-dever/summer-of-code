#include <vector>
#include <set>

class Solution {
public:
    int longestConsecutive(std::vector<int>& nums) {
        std::set<int> nums_set(nums.begin(), nums.end());

        int max_length = 1;
        for (auto num : nums_set) {
            if (!nums_set.count(num-1)) {
                int local_length = 1;

                while (nums_set.count(++num)) {
                    ++local_length;
                }

                max_length = std::max(max_length, local_length);
            }
        }
        return max_length;
    }
};
