#include <vector>
#include <climits>
#include <cmath>
#include <iostream>

int computeAsymmetry(int mountain_heights[], int left, int right, std::vector<std::vector<int>>& memo) {
    auto asymmetry = 0;

    while (left < right) {
        if (memo[left][right] != -1)
            return asymmetry + memo[left][right];

        asymmetry += abs(mountain_heights[left]-mountain_heights[right]);

        ++left;
        --right;
    }
    return asymmetry;
}

int main() {
    auto num_mountains = 0;
    std::cin >> num_mountains;

    std::vector<int> mountain_heights(num_mountains);
    for (auto i = 0; i < num_mountains; i++) {
        std::cin >> mountain_heights[i];
    }
    
    std::vector<int> asymmetry_scores(num_mountains, INT_MAX); 
    asymmetry_scores[0] = 0;

    std::vector<std::vector<int>> memo(num_mountains);
    for (auto& vec : memo) {
        vec.resize(num_mountains);
        fill(vec.begin(), vec.end(), -1);
    }

    for (auto range = 1; range < num_mountains; range++) {
        for (auto start = 0; start+range < num_mountains; start++) {
            auto asymmetry = computeAsymmetry(mountain_heights.data(), 
                                              start, 
                                              start+range,
                                              memo);

            memo[start][start+range] = asymmetry;

            asymmetry_scores[range] = std::min(asymmetry_scores[range],
                                               asymmetry);
        }
    }
    for (auto asymmetry_score : asymmetry_scores) {
        std::cout << asymmetry_score << ' ';
    }
    std::cout << '\n';
    return 0;
}
