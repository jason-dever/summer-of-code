#include <utility>
#include <stack>
#include <vector>

class Solution {
public:
    std::vector<int> dailyTemperatures(std::vector<int>& temperatures) {
        std::vector<int> times(temperatures.size()); 
        std::stack<std::pair<int, int>> temps;

        for (int i = 0; i < temperatures.size(); i++) {
            while (!temps.empty() && temperatures[i] > temps.top().first) {
                times[temps.top().second] = i - temps.top().second;
                temps.pop();
            }
            temps.push(std::pair<int, int>(temperatures[i], i));
        }
        return times;
    }
};
