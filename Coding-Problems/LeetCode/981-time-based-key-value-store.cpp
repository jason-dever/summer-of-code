#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::unordered_map, std::string, std::pair, std::vector;

class TimeMap {
private:
    unordered_map<string, vector<pair<string, int>>> data;
public:
    TimeMap() {
         
    }
    
    void set(string key, string value, int timestamp) {
        data[key].push_back(pair<string, int>(value, timestamp)); 
    }
    
    string get(string key, int goal_timestamp) {
        vector<pair<string, int>>& values = data[key]; 

        string value = "";

        int left = 0;
        int right = values.size()-1;

        while (left <= right) {
            auto mid = (left+right)/2;
            auto timestamp = values[mid].second;

            if (timestamp == goal_timestamp) {
                value = values[mid].first;
                break;
            }
            else if (timestamp < goal_timestamp) {
                value = values[mid].first;
                left = mid+1;
            }
            else
                right = mid-1;
        }
        return value;
    }
};

/**
 * Your TimeMap object will be instantiated and called as such:
 * TimeMap* obj = new TimeMap();
 * obj->set(key,value,timestamp);
 * string param_2 = obj->get(key,timestamp);
 */
