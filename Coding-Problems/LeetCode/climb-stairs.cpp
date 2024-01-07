class Solution {
public:
    int climbStairs(int n) {
        int* lookups = new int[n+1];

        for (int i = 0; i <= n; i++) {
            if (i >= 2) {
                lookups[i] = lookups[i-1]+lookups[i-2];
            }
            else
                lookups[i] = 1;
        }
        int output = lookups[n];
        delete[] lookups;

        return output;
    }
};
