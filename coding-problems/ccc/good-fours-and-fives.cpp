#include <iostream>

int main() {
    int num;
    std::cin >> num;
    
    auto num_ways = 0;

    while (num > 0) {
        num_ways += (num%5 == 0);
        num -= 4;
    }
    num_ways += (num == 0);

    std::cout << num_ways << '\n';
    return 0;
}
