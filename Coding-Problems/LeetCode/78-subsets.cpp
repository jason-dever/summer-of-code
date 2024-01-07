#include <vector>
#include <immintrin.h>

using std::vector;

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> subsets;

        int binary_set = (1 << nums.size())-1;
        int binary_subset = 0;
        do {
            vector<int> subset;
            int subset_copy = binary_subset;

            while (subset_copy) {
                int index = _mm_tzcnt_32(subset_copy);
                subset.push_back(nums[index]);
                subset_copy ^= 1 << index;
            }
            subsets.push_back(subset);
            
            binary_subset = (binary_subset - binary_set) & binary_set;
        } while (binary_subset);

        return subsets;
    }
};
