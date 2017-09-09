/**
 * Qestion: Is Unique
 * Implement an algorithm to determine if a string has all unique characters.
 * What if you cannot use additional data structures?
 * 
 * 确认问题： 字符是 ASCII 还是 Unicode
 * @time  O(1)
 * @space O(1)
 */
#include <string>
using namespace std;

class Solution {
  public:
    bool isUnique(string s) {
        if (s.size() > 128) {
            return false;
        }
        bool char_set[128] = {0};
        for (unsigned char c : s) {
            if (char_set[c]) {
                return false;
            }
            char_set[c] = true;
        }
        return true;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("main", "main") {
    Solution solution;
    REQUIRE(solution.isUnique("") == true);
    REQUIRE(solution.isUnique("abc") == true);
    REQUIRE(solution.isUnique("abca") == false);
}
