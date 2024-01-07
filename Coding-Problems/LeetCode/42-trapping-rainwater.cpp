#include <vector>

class Solution {
public:
    int trap(std::vector<int>& height) {
        std::vector<int> max_height(height.size());

        int local_max = 0;
        for (int left = 0; left < height.size(); left++) {
            local_max = std::max(local_max, height[left]);
            max_height[left] = local_max;
        }

        local_max = 0;
        for (int right = height.size()-1; right >= 0; right--) { 
            local_max = std::max(local_max, height[right]);
            max_height[right] = std::min(max_height[right], local_max);
        }
            
        int volume = 0;
        for (int i = 0; i < height.size(); i++) {
            volume += max_height[i]-height[i];
        }
        return volume;
    }
};
