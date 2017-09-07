/**
 * Question: String Compression
 * Implement a method to perform basic string compression using the counts 
 * of repeated characters. For example, the string aabcccccaaa would become a2b1c5a3. If the 
 * "compressed" string would not become smaller than the original string, your method should return 
 * the original string. You can assume the string has only uppercase and lowercase letters (a - z).
 * 
 * @time O(n)
 * @space O(n)
 */
#include <string>
using namespace std;
class Solution {
public:
    string stringCompression(string str)
    {
        if (str.size() < 2) {
            return str;
        }
        string comStr;
        comStr.reserve(str.size());
        int count = 0;
        for (size_t i = 0; i < str.size(); i++) {
            count++;
            if (i + 1 >= str.size() || str[i] != str[i + 1]) {
                comStr += str[i] + to_string(count);
                count = 0;
                if ((comStr.size() + 2) >= str.size()) {
                    return str;
                }
            }
        }
        return (comStr.size() >= str.size()) ? str : comStr;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "")
{
    Solution solution;
    REQUIRE(solution.stringCompression("aabcccccaaa") == "a2b1c5a3");
    REQUIRE(solution.stringCompression("abcd") == "abcd");
}