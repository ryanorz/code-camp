/**
 * Qestion: Remove Dups
 * Write code to remove duplicates from an unsorted linked list. 
 * FOLLOW UP
 * How would you solve this problem if a temporary buffer is not allowed?
 * 
 */
#include "my_list.h"
#include <unordered_set>
using std::unordered_set;

class Solution {
public:
    /**
     * time O(n)
     * space O(n)
     */
    void removeDups(Node* head)
    {
        unordered_set<int> hashSet;
        Node* prev = nullptr;
        while (head) {
            if (hashSet.find(head->value) == hashSet.end()) {
                hashSet.insert(head->value);
                prev = head;
                head = head->next;
            } else {
                prev->next = head->next;
                delete head;
                head = prev->next;
            }
        }
    }

    /**
     * time O(n^2)
     * space O(1)
     */
    void removeDupsWithoutTempBuffer(Node* head) {
        if (head == nullptr) {
            return;
        }
        Node* curr = head->next;
        Node* prev = nullptr;
        while (curr) {
            if (!findDup(head, curr)) {
                prev = curr;
                curr = curr->next;
            } else {
                prev->next = curr->next;
                delete curr;
                curr = prev->next;
            }
        }
    }

private:
    bool findDup(Node* head, Node* curr) {
        if (head == nullptr || curr == nullptr) {
            return false;
        }
        while (head != curr) {
            if (head->value == curr->value) {
                return true;
            }
            head = head->next;
        }
        return false;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "")
{
    Solution solution;
    Node* head = createList({ 1, 2, 3, 2, 1 });
    solution.removeDups(head);
    REQUIRE(listEquals(head, { 1, 2, 3 }));
    destroyList(head);
}
