#include <algorithm>
#include <cmath>
#include <functional>
#include <map>
#include <queue>
#include <random>
#include <raylib.h>
#include <utility>
#include <vector>

using Coordinate = std::pair<int, int>;

#define BOARD_SIZE 32
#define SCREEN_SIZE 960
#define SQUARE_SIZE SCREEN_SIZE/BOARD_SIZE

void drawBoard() {
    for (auto row = 0; row < BOARD_SIZE; ++row) {
        for (auto col = 0; col < BOARD_SIZE; ++col) {
            auto dark_green = Color ({ 15, 90, 15, 255});

            auto colour = (row%2 == col%2) ? GREEN : dark_green;
            DrawRectangle(col*SQUARE_SIZE, row*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, colour);
        }
    }
}

void drawFood(Coordinate food) {
        DrawRectangle(food.first*SQUARE_SIZE, food.second*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, RED);
}

void drawSnake(std::vector<Coordinate>& SSL) {
    // Coordinates are stored in SSL as { x, y }
    for (auto& segment : SSL) {
        DrawRectangle(segment.first*SQUARE_SIZE, segment.second*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, BLUE);
    }
}
void drawGame(std::vector<Coordinate>& SSL, Coordinate food) {
    drawBoard();
    drawSnake(SSL);
    drawFood(food);
}

std::random_device r;
std::uniform_int_distribution<int> distribution(0, BOARD_SIZE-1);
std::default_random_engine generator(r());
auto rng = std::bind(distribution, generator);

Coordinate getNewFood(std::vector<Coordinate>& SSL) {
    Coordinate food = { rng(), rng() };
    auto food_in_SSL = (std::find(SSL.begin(), SSL.end(), food) != SSL.end());

    if (food_in_SSL) {
        // Move the food in a random direction until no longer in SSL, 
        // maintains randomness with a time complexity of O(n**2)

        auto direction_x = (rng()%2) ? -1 : 1;
        auto direction_y = (rng()%2) ? -1 : 1;
        do {
            auto y_changed = false;

            food.first += direction_x;
            if (food.first < 0) {
                food.first = BOARD_SIZE-1;

                food.second += direction_y;
                y_changed = true;
            }
            else if (food.first >= BOARD_SIZE) {
                food.first = 0;

                food.second += direction_y;
                y_changed = true;
            }
            if (y_changed) {
                if (food.second < 0) {
                    food.second = BOARD_SIZE-1;
                }
                else if (food.second >= BOARD_SIZE) {
                    food.second = 0;
                }
            }
        } while (std::find(SSL.begin(), SSL.end(), food) != SSL.end());

    }

    return food;
}

void moveSnake(std::vector<Coordinate>& SSL, Coordinate& direction, Coordinate& food) {
    auto tail = SSL.back();

    for (auto i = SSL.size()-1; i >= 1; --i) {
        SSL[i] = SSL[i-1];
    }
    SSL[0] = Coordinate(SSL[0].first + direction.first, SSL[0].second + direction.second);

    if (SSL[0] == food) {
        food = getNewFood(SSL);
        SSL.push_back(tail);
    }
}

// For a human to play, just change findDirection() to setDirection()
// in the main loop.
void setDirection(Coordinate& direction) {
    switch (GetKeyPressed()) {
        case KEY_DOWN: 
            if (direction.second == 0) direction = { 0, 1 }; break;
        case KEY_UP: 
            if (direction.second == 0) direction = { 0, -1 }; break;
        case KEY_LEFT: 
            if (direction.first == 0) direction = { -1, 0 }; break;
        case KEY_RIGHT: 
            if (direction.first == 0) direction = { 1, 0 }; break;
        }
}

bool isGameOver(std::vector<Coordinate>& SSL) {
    auto& head = SSL[0];
    auto game_over = false;

    game_over |= (head.first < 0 || head.first >= BOARD_SIZE || 
                  head.second < 0 || head.second >= BOARD_SIZE);

    game_over |= (std::find(SSL.begin()+1, SSL.end(), head) != SSL.end());

    return game_over;
}

int heuristic(Coordinate a, Coordinate b) {
    return std::abs(a.first-b.first) + std::abs(a.second-b.second);
}

std::vector<Coordinate> getNeighbours(Coordinate node) {
    std::vector<Coordinate> neighbours;

    if (node.first > 0)
        neighbours.push_back(Coordinate(node.first-1, node.second));
    if (node.first < BOARD_SIZE-1)
        neighbours.push_back(Coordinate(node.first+1, node.second));
    if (node.second > 0)
        neighbours.push_back(Coordinate(node.first, node.second-1));
    if (node.second < BOARD_SIZE-1)
        neighbours.push_back(Coordinate(node.first, node.second+1));

    return neighbours;
}

void findDirection(std::vector<Coordinate>& SSL, Coordinate& food, Coordinate& direction) {
    auto& head = SSL[0];
    using Node = std::pair<int, Coordinate>;

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_list;
    std::map<Coordinate, int> cost_to_reach;
    std::map<Coordinate, Coordinate> parent_of;

    open_list.push(Node(0, food));
    cost_to_reach[food] = 0;

    while (!open_list.empty()) {
        auto a = open_list;

        auto current = open_list.top().second;
        open_list.pop();

        if (current == head) {
            auto parent = parent_of[current];
            direction = Coordinate(parent.first-head.first, parent.second-head.second);
            return;
        }

        for (auto& neighbour : getNeighbours(current)) {
            auto new_cost = cost_to_reach[current]+1;

            auto location = std::find(SSL.begin(), SSL.end(), neighbour);
            if (location != SSL.end() && neighbour != head) {
                continue;
            }

            if (cost_to_reach.count(neighbour) == 0 || new_cost < cost_to_reach.at(neighbour)) {
                cost_to_reach[neighbour] = new_cost;
                parent_of[neighbour] = current;

                auto priority = new_cost + heuristic(head, neighbour);
                open_list.push(Node(priority, neighbour));
            }
        }
    }
}

int main() {
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Snake A*");
    SetTargetFPS(50);

    auto SSL = std::vector<Coordinate>{ {6, 8}, {5, 8}, {4, 8}, {3, 8} };
    auto direction = Coordinate(1, 0);
    auto food = Coordinate(7, 9);

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        if (isGameOver(SSL)) break;
        findDirection(SSL, food, direction);
        drawGame(SSL, food);
        moveSnake(SSL, direction, food);

        EndDrawing();
    }
    return 0;
}
