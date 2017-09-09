/**
 * Qestion: Check Permutation
 * Given two strings,write a method to decide if one is a permutation of the other.
 * 
 * 确认问题：
 *   1. 是否需要关心大小写
 *   2. 是否需要关心空白符
 *   3. 字符是否是 ASCII 码
 * @time  O(n)
 * @space O(1)
 */
#include <string>
using namespace std;

class Solution {
  public:
    bool checkPermutation(string s1, string s2) {
        if (s1.size() != s2.size()) {
            return false;
        }
        int    diff[128] = {0};
        size_t size      = s1.size();
        for (size_t i = 0; i < size; i++) {
            diff[(unsigned)s1[i]]++;
            diff[(unsigned)s2[i]]--;
        }
        for (size_t i = 0; i < 128; i++) {
            if (diff[i] != 0) {
                return false;
            }
        }
        return true;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("main", "main") {
    Solution solution;
    REQUIRE(solution.checkPermutation("", "") == true);
    REQUIRE(solution.checkPermutation("abc", "cab") == true);
    REQUIRE(solution.checkPermutation("abca", "abc") == false);
}
