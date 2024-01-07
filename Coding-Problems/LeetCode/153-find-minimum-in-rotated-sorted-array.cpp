#include <algorithm>
#include <vector>

class Solution {
public:
    int findMin(std::vector<int>& nums) {
        int left = 0;
        int right = nums.size()-1;

        const int first_element = nums[left];
        int min = first_element;

        while (left <= right) {
            int mid = (left+right)/2;
            int mid_val = nums[mid];

            min = std::min(min, mid_val);

            if (mid_val >= first_element)
                left = mid+1;
            else
                right = mid-1;
        }
        return min;
    }
};
