#include <vector>

using std::vector;

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        vector<int> nums_hit;
        for (auto i = 0; i < nums.size(); i++) {
            for (auto j = 0; j < nums_hit.size(); j++) {
                if (nums_hit[j] == nums[i]) {
                    nums.erase(nums.begin()+i-1);
                    i--;
                    break;
                }
            }
            nums_hit.push_back(nums[i]);
        }
        return nums.size();
    }
};
