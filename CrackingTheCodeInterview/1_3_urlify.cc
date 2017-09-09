/**
 * Qestion: URLify
 * 
 * @time  O(n)
 * @space O(1)
 */
#include <string.h>
class Solution {
  public:
    const char *urlify(char str[], int length) {
        int spaceCount = 0;
        for (int i = 0; i < length; i++) {
            if (str[i] == ' ') {
                spaceCount++;
            }
        }
        int index    = length + spaceCount * 2;
        str[index--] = '\0';
        for (int i = length - 1; i >= 0; i--) {
            if (str[i] == ' ') {
                str[index--] = '0';
                str[index--] = '2';
                str[index--] = '%';
            } else {
                str[index--] = str[i];
            }
        }
        return str;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("main", "main") {
    Solution solution;
    SECTION("") {
        char buffer[128] = "";
        REQUIRE(strcmp("", solution.urlify(buffer, strlen(buffer))) == 0);
    }
    SECTION("Mr John Smith") {
        char buffer[128] = "Mr John Smith";
        REQUIRE(strcmp("Mr%20John%20Smith",
                       solution.urlify(buffer, strlen(buffer))) == 0);
    }
    SECTION(" ") {
        char buffer[128] = " ";
        REQUIRE(strcmp("%20", solution.urlify(buffer, strlen(buffer))) == 0);
    }
}
