#include <deque>
#include <vector>

class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        std::vector<int> out; 
        std::deque<int> window_deque;

        for (auto i = 0; i < k; i++) {
            while (!window_deque.empty() && window_deque.back() < nums[i]) {
                window_deque.pop_back();
            }
            window_deque.push_back(nums[i]);
        }

        out.push_back(window_deque.front());

        for (auto prev_left = 0, right = k; right < nums.size(); ++right, ++prev_left) {
            auto new_num = nums[right];

            while (!window_deque.empty() && window_deque.back() < new_num) {
                window_deque.pop_back();
            }
            window_deque.push_back(new_num);

            if (window_deque.front() == nums[prev_left])
                window_deque.pop_front();

            out.push_back(window_deque.front());
        }
        return out;
    }
};
