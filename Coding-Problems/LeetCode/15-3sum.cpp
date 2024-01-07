#include <algorithm>
#include <vector>

class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        std::vector<std::vector<int>> output;

        for (int first_idx = 0; first_idx < nums.size(); first_idx++) {
            int left = first_idx+1;
            int right = nums.size()-1;

            if (first_idx > 0 && nums[first_idx] == nums[first_idx-1])
                continue;

            while (left < right) {
                int sum = nums[first_idx]+nums[left]+nums[right];

                if (sum == 0) {
                    output.push_back(std::vector<int> { nums[first_idx], nums[left], nums[right] });

                    int previous_left = nums[left];
                    int previous_right = nums[right];

                    while (nums[left] == previous_left && left < right)
                        left++;
                    while (nums[right] == previous_right && left < right)
                        right--;
                }

                if (sum < 0)
                    ++left;
                else if (sum > 0)
                    --right;
            }
        }
        return output;
    }
};
