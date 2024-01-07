#include <vector>

using std::vector;

class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int left = 0;
        int right = numbers.size()-1;

        while (left < right) {
            int sum = numbers[left]+numbers[right];

            if (sum == target)
                break;
            else if (sum > target)
                --right;
            else // sum < target
                ++left;
        }
        return vector<int> { left+1, right+1 };
    }
};
