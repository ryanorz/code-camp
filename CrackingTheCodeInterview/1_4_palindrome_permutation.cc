/**
 * Qestion: Palindrome Permutation
 * Given a string, write a function to check if it is a permutation of a palindrome. 
 * A palindrome is a word or phrase that is the same forwards and backwards. 
 * A permutation is a rearrangement of letters. 
 * The palindrome does not need to be limited tojust dictionary words.
 * EXAMPLE 
 * Input: Tact Coa
 * Output: True (permutations: "taco cat", "atco eta", etc.)
 * 
 * @确认问题：
 * 1. ASCII 码
 * 2. 空白符
 * 3. 大小写
 */
#include <string>
using namespace std;

class Solution {
  public:
    bool isPalindromePermutation(string str) {
        int count[26] = {0};
        for (auto c : str) {
            if (c >= 'a' && c <= 'z') {
                count[c - 'a']++;
            } else if (c >= 'A' && c <= 'Z') {
                count[c - 'A']++;
            }
        }
        bool singleExist = false;
        for (auto num : count) {
            if (num & 1) {
                if (singleExist) {
                    return false;
                }
                singleExist = true;
            }
        }

        return true;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "") {
    Solution solution;
    REQUIRE(solution.isPalindromePermutation(""));
    REQUIRE(solution.isPalindromePermutation("Tact Coa"));
    REQUIRE(!solution.isPalindromePermutation("Tact Co"));
}
