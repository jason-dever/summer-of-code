#include <algorithm>

#include <string>
#include <vector>

using std::string, std::vector;

class Solution {
public:
    static bool compare(std::string lhs, std::string rhs) {
        return (lhs+rhs) > (rhs+lhs);
    }    

    string largestNumber(vector<int>& nums) {
        std::vector<std::string> str_nums;
        for (auto num : nums)
            str_nums.push_back(std::to_string(num));

        string output = "";
        std::sort(str_nums.begin(), str_nums.end(), compare);
        for (auto num : str_nums)
            output += num;
        
        if (output[0] == '0') 
            return "0";
        else 
            return output;
    }
};
