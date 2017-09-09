/**
 * Qestion: String Rotation
 * Assume you have a method isSubstring which checks if one word is a substring of another. 
 * Given two strings, sl and s2, write code to check if s2 is a rotation of sl using only
 * one call to isSubstring(e.g.,"waterbottle"is a rotation of"erbottlewat").
 * 
 * 确认细节：
 * 1. 大小写
 * 2. 空白符
 * 3. ascii
 */

#include <string>
using std::string;

bool isSubstring(string& s1, string& s2)
{
    if (s1.size() > s2.size()) {
        return false;
    }
    for (size_t i = 0; i <= s2.size() - s1.size(); i++) {
        bool match = true;
        for (size_t j = 0; j < s1.size(); j++) {
            if (s1[j] != s2[i + j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }
    }
    return false;
}

class Solution {
public:
    bool isStringRotation(string s1, string s2) {
        if (s1.size() != s2.size()) {
            return false;
        }
        string doubleS2 = s2 + s2;
        return isSubstring(s1, doubleS2);
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "") {
    Solution solution;
    REQUIRE(solution.isStringRotation("abc", "bca"));
    REQUIRE(!solution.isStringRotation("abc", "bac"));
}