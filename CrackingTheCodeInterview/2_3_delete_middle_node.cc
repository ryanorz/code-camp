/**
 * Qestion: Delete Middle Node
 * Implement an algorithm to delete a node in the middle 
 * (i.e., any node but the first and last node, not necessarily the exact middle) 
 * of a singly linked list, given only access to that node.
 * EXAMPLE
 * lnput:the node c from the linked lista->b->c->d->e->f
 * Result: nothing is returned, but the new linked list looks likea->b->d->e->f
 */
#include "my_list.h"

class Solution {
public:
    void deleteMiddleNode(Node* node)
    {
        if (node == nullptr) {
            return;
        }
        Node* next = node->next;
        *node      = *next;
        delete next;
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "")
{
    Solution solution;
    Node*    list = createList({ 1, 2, 3 });
    solution.deleteMiddleNode(list->next);
    REQUIRE(listEquals(list, { 1, 3 }));
    destroyList(list);
}