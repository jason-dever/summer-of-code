#include <vector>

class Solution {
public:
    void constructPermutations(std::vector<int>& nums, std::vector<bool>& chosen, 
                            std::vector<int>& permutation, 
                            std::vector<std::vector<int>>& permutations) {

        if (permutation.size() == nums.size()) {
            permutations.push_back(permutation);
            return;
        }

        for (int i = 0; i < nums.size(); i++) {
            if (chosen[i] == true) continue;

            permutation.push_back(nums[i]);
            chosen[i] = true;

            constructPermutations(nums, chosen, permutation, permutations);

            permutation.pop_back();
            chosen[i] = false;
        }
    }
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
        std::vector<bool> chosen(nums.size(), false);
        std::vector<int> permutation;
        std::vector<std::vector<int>> permutations;

        constructPermutations(nums, chosen, permutation, permutations);
        return permutations;
    }
};
