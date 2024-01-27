#include <iostream>

void countSolutions(int num, int& num_ways) {
    if (num == 0)
        ++num_ways;
    
    if (num <= 3)
        return;

    countSolutions(num-4, num_ways);
    num_ways += (num%5 == 0);
}

int main() {
    int num;
    std::cin >> num;
    
    int num_ways = 0;
    countSolutions(num, num_ways);

    std::cout << num_ways << '\n';
    return 0;
}
