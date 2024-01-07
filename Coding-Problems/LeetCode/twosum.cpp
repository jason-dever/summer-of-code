#include <vector>
#include <unordered_map>

using std::vector;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        std::unordered_map<int, int> values;

        for (int i = 0; i < nums.size(); i++) {
            int value_needed = target-nums[i];

            if (values[value_needed])
                return vector<int> { i, values[value_needed]-1 };
            
            values[nums[i]] = i+1;
        }
        return vector<int> { };
    }
};
