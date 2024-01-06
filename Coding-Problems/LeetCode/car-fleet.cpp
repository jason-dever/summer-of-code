#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

class Solution {
public:
    int carFleet(int target, std::vector<int>& position, std::vector<int>& speed) {
        std::vector<std::pair<int, int>> input(position.size());
        for (int i = 0; i < input.size(); i++) {
            input[i].first = position[i];
            input[i].second = speed[i];
        }
        std::sort(input.rbegin(), input.rend());
        
        std::stack<float> fleets;
        for (auto i : input) {
            auto time = static_cast<float>(target-i.first)/i.second;

            if (fleets.empty())
                fleets.push(time);
            else if (time > fleets.top())
                fleets.push(time);
        }
        return fleets.size();
    }
};
