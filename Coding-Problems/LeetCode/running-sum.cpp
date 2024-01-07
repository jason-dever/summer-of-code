#include <vector>

using std::vector;

class Solution {
public:

    vector<int> runningSum(vector<int>& nums) {
        auto sum = 0;
        vector<int> out;

        for (auto i : nums) {
            sum += i;
            out.push_back(sum);
        }
        return out;
    }
};
