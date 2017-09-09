/**
 * Qestion: Return Kth to Last (返回倒数第n个节点)
 * Implement an algorithm to find the kth to last element of a singly linked list.
 * 
 * Time O(n)
 * Space O(1)
 */
#include "my_list.h"

class Solution {
public:
    Node* kthLastNode(Node* head, int k)
    {
        if (k <= 0) {
            return nullptr;
        }
        Node* slow = head;
        Node* fast = head;
        while (k--) {
            if (fast == nullptr) {
                return nullptr;
            }
            fast = fast->next;
        }
        while (fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "") {
    Solution solution;
    Node* head = createList({1, 2, 3});
    REQUIRE(solution.kthLastNode(head, 1)->value == 3);
    REQUIRE(solution.kthLastNode(head, 2)->value == 2);
    REQUIRE(solution.kthLastNode(head, 3)->value == 1);
    destroyList(head);
}
