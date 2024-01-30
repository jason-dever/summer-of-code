#include <algorithm>
#include <cstdint>
#include <vector>

class Solution {
public:
    int minEatingSpeed(std::vector<int>& piles, int h) {
        int64_t min = 1;        
        int64_t max = 1e10;
        int64_t output = 0;

        while (min <= max) {
            int mid = (min+max)/2;
            int eating_time = getEatingTime(piles, mid);

            if (eating_time <= h) {
                max = mid-1;
                output = mid;
            }
            else
                min = mid+1;
        }
        return static_cast<int>(output);
    }
private:
    int getEatingTime(std::vector<int>& piles, int bite_size) {
        int64_t time = 0;

        for (auto pile : piles)
            time += (pile+(bite_size-1))/bite_size;

        return static_cast<int>(time);
    }
};
