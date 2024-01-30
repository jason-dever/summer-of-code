#include <vector>

std::vector<int> plusOne(std::vector<int>& digits) {
    digits[digits.size()-1]++;

    for (auto i = digits.size()-1; i >= 0; i--) {
        if (digits[i] < 10) {
            break;
        }
        else {
            digits[i] -= 10;

            if (i == 0) {
                digits.insert(digits.begin(), 1);
                break;
            }
            digits[i-1]++;
        } 
    }
    return digits;
}
