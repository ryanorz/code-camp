#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Solution {
public:
    string sortChar(const string& s)
    {
        string key = s;
        std::sort(key.begin(), key.end());
        return key;
    }

    void sort(vector<string>& array)
    {
        unordered_multimap<string, string> multi_hashmap;
        for (auto& e : array) {
            multi_hashmap.insert({ { sortChar(e), e } });
        }
        int index = 0;
        for (auto& e : multi_hashmap) {
            array[index++] = e.second;
        }
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "")
{
    Solution       solution;
    vector<string> testcase = { "abc", "xyz", "cab", "yxz", "bac" };
    vector<string> testres  = { "bac", "cab", "abc", "yxz", "xyz" };
    solution.sort(testcase);
    REQUIRE(testcase == testres);
}