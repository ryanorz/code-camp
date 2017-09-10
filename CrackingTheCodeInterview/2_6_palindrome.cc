/**
 * Qestion: Palindrome
 * Implement a function to check if a linked list is a palindrome.
 */

#include "my_list.h"
#include <stack>
using std::stack;
class Solution {
public:
    bool palindrome(Node* head)
    {
        stack<int> st;
        Node*      node = head;
        while (node) {
            st.push(node->value);
            node = node->next;
        }
        node = head;
        while (node) {
            if (node->value != st.top()) {
                return false;
            }
            node = node->next;
            st.pop();
        }
        return true;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "")
{
    Solution solution;
    Node*    head  = createList({ 1, 2, 3, 2, 1 });
    Node*    head2 = createList({ 1, 2, 3, 1 });
    REQUIRE(solution.palindrome(head));
    REQUIRE(!solution.palindrome(head2));
    destroyList(head);
    destroyList(head2);
}
