
/**
 * Qestion: One Away
 * There are three types of edits that can be performed on strings: insert a character,
 * remove a character, or replace a character. Given two strings, write a function to check 
 * if they are one edit (or zero edits) away
 * 
 * 确认问题:
 * 1. ASCII 码
 * 2. 大小写
 * 3. 空白符
 */
#include <cstdlib>
#include <string>
using namespace std;

class Solution {
  private:
    char lowercase(char c) {
        if (c >= 'A' && c <= 'Z') {
            return c - 'A' + 'a';
        }
        return c;
    }

    // s1 is longer than s2
    bool isInsertEdit(string &s1, string &s2) {
        size_t idx1      = 0;
        size_t idx2      = 0;
        bool   foundDiff = false;
        while (idx1 < s1.size() && idx2 < s2.size()) {
            if (lowercase(s1[idx1]) != lowercase(s2[idx2])) {
                if (foundDiff) {
                    return false;
                }
                foundDiff = true;
                idx1++;
            } else {
                idx1++;
                idx2++;
            }
        }
        return true;
    }

    bool isReplaceEdit(string &s1, string &s2) {
        int replaceCount = 0;
        for (size_t i = 0; i < s1.size(); i++) {
            if (lowercase(s1[i]) != lowercase(s2[i])) {
                replaceCount++;
                if (replaceCount > 1) {
                    return false;
                }
            }
        }
        return true;
    }

  public:
    bool isOneEditAway(string s1, string s2) {
        int s1Length = s1.size();
        int s2Length = s2.size();

        if (abs(s1Length - s2Length) > 1) {
            return false;
        } else if (s1Length - s2Length == 1) {
            return isInsertEdit(s1, s2);
        } else if (s1Length - s2Length == -1) {
            return isInsertEdit(s2, s1);
        } else {
            return isReplaceEdit(s1, s2);
        }
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "") {
    Solution solution;
    REQUIRE(solution.isOneEditAway("", ""));
    REQUIRE(solution.isOneEditAway("", "p"));
    REQUIRE(solution.isOneEditAway("pale", "pale"));
    REQUIRE(solution.isOneEditAway("pale", "ple"));
    REQUIRE(solution.isOneEditAway("pales", "pale"));
    REQUIRE(solution.isOneEditAway("pale", "bale"));
    REQUIRE(!solution.isOneEditAway("pale", "bake"));
}