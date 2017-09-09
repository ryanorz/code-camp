/**
 * Qestion: Partition
 * Write code to partition a linked list around a value x, such that all nodes less than x come 
 * before all nodes greater than or equal to x. If x is contained within the list, the values of x only need
 * to be after the elements less than x (see below). The partition element x can appear anywhere in the
 * "right partition"; it does not need to appear between the left and right partitions.
 * EXAMPLE
 * Input: 3 -> 5 -> 8 -> 5 -> 10 -> 2 -> 1 [partition= 5]
 * Output: 3 -> 1 -> 2 -> 10 -> 5 -> 5 -> 8
 */

#include "my_list.h"
class Solution {
public:
    Node* partition(Node* node, int x)
    {
        Node  beforeHead(0);
        Node  afterHead(0);
        Node* before = &beforeHead;
        Node* after  = &afterHead;
        while (node) {
            Node* next = node->next;
            node->next = nullptr;
            if (node->value < x) {
                before->next = node;
                before       = before->next;
            } else {
                after->next = node;
                after       = after->next;
            }
            node = next;
        }
        if (beforeHead.next == nullptr) {
            return afterHead.next;
        } else {
            before->next = afterHead.next;
            return beforeHead.next;
        }
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "")
{
    Solution solution;
    Node*    head = createList({ 3, 5, 8, 5, 10, 2, 1 });
    head          = solution.partition(head, 5);
    listDisplay(head);
    destroyList(head);
}