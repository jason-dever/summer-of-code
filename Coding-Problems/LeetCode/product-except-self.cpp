#include <vector>

class Solution {
public:
    std::vector<int> productExceptSelf(std::vector<int>& nums) {
        std::vector<int> prefix_products(nums.size());
        std::vector<int> suffix_products(nums.size());

        int product = 1;
        for (int i = 0; i < nums.size(); i++) {
            prefix_products[i] = product;
            product *= nums[i];
        }
        product = 1;
        for (int i = nums.size()-1; i >= 0; i--) {
            suffix_products[i] = product;
            product *= nums[i];
        }

        std::vector<int> output(nums.size());
        for (int i = 0; i < nums.size(); i++) {
            output[i] = prefix_products[i]*suffix_products[i];
        }
        return output;
    }
};
