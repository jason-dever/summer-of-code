#include <stack>
#include <string>
#include <vector>

class Solution {
private:
    void parenthesesGenHelper(std::vector<std::string>& parentheses_list, 
                              std::string& combination,
                              int num_openings,
                              int num_closings) {
        
        if (!num_openings && !num_closings) {
            parentheses_list.push_back(combination);
            return;
        }

        if (num_closings > num_openings) {
            combination += ')';
            parenthesesGenHelper(parentheses_list, combination, num_openings, num_closings-1);
            combination.pop_back();
        }
        if (num_openings) {
            combination += '(';
            parenthesesGenHelper(parentheses_list, combination, num_openings-1, num_closings);
            combination.pop_back();
        }
    }
public:
    std::vector<std::string> generateParenthesis(int n) {
        std::vector<std::string> output;
        std::string combination = "";

        parenthesesGenHelper(output, combination, n, n);
        return output;
    }
};
