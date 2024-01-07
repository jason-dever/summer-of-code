#include <unordered_map>
#include <stack>
#include <string>

class Solution {
public:
    bool isValid(std::string s) {
        std::unordered_map<char, char> openings { {']', '['}, {')', '('}, {'}', '{'} };
        std::stack<char> brackets;

        for (auto ch : s) {
            if (ch == '[' || ch == '{' || ch == '(') 
                brackets.push(ch);
            else
                if (brackets.size() == 0 || brackets.top() != openings[ch])
                    return false;
                else
                    brackets.pop();
        }
        return brackets.empty();
    }
};
