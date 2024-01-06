#include <vector>
#include <climits>
#include <cmath>
#include <iostream>

int computeAsymmetry(int mountain_heights[], int begin, int end, std::vector<std::vector<int>>& memo) {
    float mid = static_cast<float>(begin+end)/2;
    int left;
    int right;

    if (fmod(mid, 1)) {
        left = std::floor(mid);
        right = std::ceil(mid);
    }
    else {
        left = mid-1;
        right = mid+1;
    }

    auto asymmetry = 0;
    while (left >= begin && right <= end) {
        asymmetry += abs(mountain_heights[left] - mountain_heights[right]);
        memo[left][right] = asymmetry;
        --left;
        ++right;
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

    // Crops of size 0 (crops where there is only one mountain left) will always
    // be perfectly symmetrical, so we can set it to 0 preemptively and ignore
    // all combinations where there is only one mountain in the loop.
    asymmetry_scores[0] = 0;

    for (auto begin = 0; begin < num_mountains; begin++) {
        for (auto end = num_mountains-1; end > begin; end--) {
            auto crop_size = end-begin;

            auto asymmetry_score = computeAsymmetry(mountain_heights.data(),
                                                    begin, end);

            asymmetry_scores[crop_size] = std::min(asymmetry_score,
                                                   asymmetry_scores[crop_size]);
        }
    }

    for (auto asymmetry_score : asymmetry_scores) {
        std::cout << asymmetry_score << " ";
    }
    std::cout << "\n";
    return 0;
}
