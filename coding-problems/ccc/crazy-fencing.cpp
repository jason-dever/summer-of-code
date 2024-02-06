#include <cmath>
#include <iostream>
#include <vector>

int main() {
    auto num_fences = 0;
    std::cin >> num_fences;

    std::vector<float> fence_heights(num_fences+1);
    for (auto i = 0; i < num_fences+1; ++i) {
        std::cin >> fence_heights[i];
    }

    std::vector<float> fence_widths(num_fences);
    for (auto i = 0; i < num_fences; ++i) {
        std::cin >> fence_widths[i];
    }

    float area = 0;

    auto left = 0, right = 1;
    while (right < fence_heights.size()) {
        auto height_l = fence_heights[left];
        auto height_r = fence_heights[right];

        auto triangle_height = fabs(height_r-height_l);
        auto rectangle_height = std::max(height_l, height_r)-triangle_height;

        auto fence_width = fence_widths[left];

        area += (rectangle_height*fence_width);
        area += (fence_width*triangle_height)/2;

        ++left;
        ++right;
    }
    std::cout << area << '\n';
    return 0;
}
