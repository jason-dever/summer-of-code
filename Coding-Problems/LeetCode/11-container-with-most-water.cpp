#include <vector>

class Solution {
public:
    int maxArea(std::vector<int>& height) {
        int max_volume = 0;

        int left = 0;
        int right = height.size()-1;
        while (left < right) {
            max_volume = std::max(max_volume, std::min(height[left], 
                                                       height[right])*(right-left));

            if (height[left] < height[right])
                ++left;
            else
                --right;
        }
        return max_volume;
    }
};
