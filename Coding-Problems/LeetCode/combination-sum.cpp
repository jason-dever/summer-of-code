#include <vector>

using std::vector;

class Solution {
public:
    void findCombinations(int index, vector<int>& candidates, int target, vector<int>& combination, vector<vector<int>>& combinations) {
        if (target < 0 || index >= candidates.size())
            return;
        
        if (target == 0) {
            combinations.push_back(combination);
            return;
        }

        // Can either include the element at index or not include it.
        // Not including it
        findCombinations(index+1, candidates, target, combination, combinations);

        // Including it
        combination.push_back(candidates[index]);
        findCombinations(index, candidates, target-candidates[index], combination, combinations);
        combination.pop_back();
    }


    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<int> combination;
        vector<vector<int>> combinations;
        
        findCombinations(0, candidates, target, combination, combinations);
        return combinations;
    }
};
