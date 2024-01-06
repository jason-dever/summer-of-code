#include <utility>
#include <stack>
#include <vector>

class Solution {
public:
    int largestRectangleArea(std::vector<int>& heights) {
        int max_area = 0;
        std::stack<std::pair<int, int>> bars_stack;        
        
        for (int i = 0; i < heights.size(); i++) {
            int bar = heights[i];
            int push_idx = i;

            while (!bars_stack.empty() && bar < bars_stack.top().second) {
                 max_area = std::max(max_area, (i-bars_stack.top().first)
                                                * bars_stack.top().second);

                push_idx = bars_stack.top().first;
                bars_stack.pop();
            }
            bars_stack.push(std::pair(push_idx, bar));
        }
        while (!bars_stack.empty()) {
            max_area = std::max<size_t>(max_area, (heights.size()-bars_stack.top().first)
                                                   * bars_stack.top().second);
            bars_stack.pop();
        }
        return max_area;
    }
};
