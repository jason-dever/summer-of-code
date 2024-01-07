#include <iostream>
#include <vector>

class Solution {
public:
    int uniquePathsWithObstacles(std::vector<std::vector<int>>& obstacleGrid) {
        std::vector<std::vector<int>> memo(obstacleGrid.size(),
                                           std::vector<int>(obstacleGrid[0].size()));
        
        // Set row zero in memo to 1
        for (int i = 0; i < memo[0].size(); i++) {
            if (obstacleGrid[0][i])
                break;
            memo[0][i] = 1;
        }
        
        // Set column zero in memo to 1
        for (int i = 0; i < memo.size(); i++) {
            if (obstacleGrid[i][0])
                break;
            memo[i][0] = 1;
        }

        // Construct rest of memoization array
        for (int i = 1; i < memo.size(); i++) {
            for (int j = 1; j < memo[0].size(); j++) {
                if (!obstacleGrid[i][j])
                    memo[i][j] = memo[i-1][j]+memo[i][j-1];
            }
        }

        return memo[obstacleGrid.size()-1][obstacleGrid[0].size()-1];
    }
};
