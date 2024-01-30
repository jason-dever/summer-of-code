#include <vector>

using std::vector;

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int cheapest_buy = prices[0];
        int max_profit = 0;

        for (int i = 0; i < prices.size(); i++) {
            cheapest_buy = std::min(cheapest_buy, prices[i]);
            max_profit = std::max(max_profit, prices[i]-cheapest_buy);
        }
        return max_profit;
    }
};
