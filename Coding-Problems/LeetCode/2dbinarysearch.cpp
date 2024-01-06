#include <vector>

class Solution {
public:
    bool searchMatrix(std::vector<std::vector<int>>& matrix, int target) {
        int left = 0;
        int right = matrix.size()-1;
        int vec_idx = -1;

        while (left <= right) {
            int mid_idx = (left+right)/2;

            if (target >= matrix[mid_idx][0] && target <= matrix[mid_idx][matrix[mid_idx].size()-1]) {
                vec_idx = mid_idx;
                break;
            }
            else if (target > matrix[mid_idx][0]) {
                left = mid_idx+1;
            }
            else { // target < last element in matrix[mid_idx]
                right = mid_idx-1;
            }
        }

        if (vec_idx == -1)
            return false;

        left = 0;
        right = matrix[vec_idx].size()-1;

        while (left <= right) {
            int mid_idx = (left+right)/2;
            int mid_val = matrix[vec_idx][mid_idx];

            if (target == mid_val)
                return true;
            else if (target > mid_val)
                left = mid_idx+1;
            else
                right = mid_idx-1;
        }
        return false;
    }
};

