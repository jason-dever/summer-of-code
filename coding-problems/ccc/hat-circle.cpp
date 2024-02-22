#include <iostream>
#include <vector>

int main() {
    auto num_seats = 0;
    std::cin >> num_seats;

    std::vector<int> circle(num_seats);
    for (auto i = 0; i < num_seats; ++i)
        std::cin >> circle[i];

    auto matching_people = 0;
    for (auto i = num_seats-1; i >= num_seats/2; --i)
        if (circle[i] == circle[i-(num_seats/2)])
            matching_people += 2;

    std::cout << matching_people;
    return 0;
}
