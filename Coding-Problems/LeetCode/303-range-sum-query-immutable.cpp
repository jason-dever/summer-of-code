#include <vector>

using std::vector;

class NumArray {
public:
    NumArray(vector<int>& nums) {
        int sum = 0;
        for (auto num : nums) {
            sum += num;
            prefix_sum_array.push_back(sum);
        }
        
    }
    
    int sumRange(int left, int right) {
        if (left > 0)
            return prefix_sum_array[right]-prefix_sum_array[left-1]; 
        else
            return prefix_sum_array[right];
    }
private:
    vector<int> prefix_sum_array;
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * int param_1 = obj->sumRange(left,right);
 */
