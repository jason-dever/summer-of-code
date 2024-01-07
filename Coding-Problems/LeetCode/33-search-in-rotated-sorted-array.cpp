#include <vector>

class Solution {
public:
    int search(std::vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size()-1;

        const int first_element = nums[left];
        int min = 0x7FFFFFFF;
        int pivot_idx = -1;

        while (left <= right) {
            int mid = (left+right)/2;
            int mid_val = nums[mid];

            pivot_idx = (mid_val < min) ? mid : pivot_idx;
            min = std::min(min, mid_val);

            if (mid_val >= first_element)
                left = mid+1;
            else
                right = mid-1;
        }
        pivot_idx = (first_element < min) ? 0 : pivot_idx;
        const int last_element = nums[nums.size()-1];

        if (last_element < target) {
            left = 0;
            right = pivot_idx;
        }
        else {
            left = pivot_idx;
            right = nums.size()-1;
        }

        while (left <= right) {
            int mid = (left+right)/2;
            int mid_val = nums[mid];

            if (mid_val == target)
                return mid;
            else if (mid_val < target)
                left = mid+1;
            else
                right = mid-1;
        }
        return -1;
    }
};
