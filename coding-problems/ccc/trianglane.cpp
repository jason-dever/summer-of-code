#include <iostream>
#include <vector>

int main() {
    int num_columns;
    std::cin >> num_columns;

    std::vector<int> lanes[2];

    for (int i = 0; i <= 1; i++) {
        for (int j = 0; j < num_columns; j++) {
            int input;
            std::cin >> input;
            lanes[i].push_back(input);
        }
    }

    int sum = 0;
    for (int row = 0; row <= 1; row++) {
        for (int column = 0; column < num_columns; column++) {
            int num_adjacent_wet_triangles = 0;

            if (column > 0)
                num_adjacent_wet_triangles += lanes[row][column-1];
            if (column < num_columns-1) {
                num_adjacent_wet_triangles += lanes[row][column+1];
            }
            num_adjacent_wet_triangles += (lanes[!row][column])*(column%2 == 0);

            sum += (3 - num_adjacent_wet_triangles)*lanes[row][column];
        }
    }
    std::cout << sum << std::endl;
    return 0;
}
