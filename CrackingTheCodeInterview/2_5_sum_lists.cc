/**
 * Qestion: Sum Lists
 * You have two numbers represented by a linked list, where each node contains a single
 * digit.The digits are stored in reverse order, such that the 1 's digit is at the head of the list. Write a
 * function that adds the two numbers and returns the sum as a linked list.
 * 
 * EXAMPLE
 * Input:(7-> 1 -> 6) + (5 -> 9 -> 2).That is,617 + 295.
 * Output: 2 -> 1 -> 9. That is, 912.
 * 
 * FOLLOW UP
 * Suppose the digits are stored in forward order. Repeat the above problem.
 * EXAMPLE
 * lnput:(6 -> 1 -> 7) + (2 -> 9 -> 5).That is,617 + 295.
 * Output: 9 -> 1 -> 2. That is, 912.
 */

#include "my_list.h"

class Solution {
private:
    Node* reverseList(Node* node)
    {
        Node* newList = nullptr;
        while (node) {
            Node* next = node->next;
            node->next = newList;
            newList    = node;
            node       = next;
        }
        return newList;
    }

public:
    Node* sumListsWithReverseOrder(Node* l1, Node* l2)
    {
        Node  sumHead(0);
        Node* sumList = &sumHead;
        int   step    = 0;
        while (l1 || l2 || step) {
            int v1        = l1 ? l1->value : 0;
            int v2        = l2 ? l2->value : 0;
            int sum       = v1 + v2 + step;
            step          = sum / 10;
            Node* node    = new Node(sum % 10);
            sumList->next = node;
            sumList       = sumList->next;
            if (l1) {
                l1 = l1->next;
            }
            if (l2) {
                l2 = l2->next;
            }
        }
        return sumHead.next;
    }

    Node* sumListsWithForwardOrder(Node* l1, Node* l2)
    {
        return reverseList(
            sumListsWithReverseOrder(reverseList(l1), reverseList(l2)));
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "")
{
    Solution solution;

    SECTION("reverse ordered")
    {
        Node* l1_ro  = createList({ 7, 1, 6 });
        Node* l2_ro  = createList({ 5, 9, 2 });
        Node* sum_ro = solution.sumListsWithReverseOrder(l1_ro, l2_ro);
        REQUIRE(listEquals(sum_ro, { 2, 1, 9 }));
        destroyList(l1_ro);
        destroyList(l2_ro);
        destroyList(sum_ro);
    }

    SECTION("Forward ordered")
    {
        Node* l1_fo  = createList({ 6, 1, 7 });
        Node* l2_fo  = createList({ 2, 9, 5 });
        Node* sum_fo = solution.sumListsWithForwardOrder(l1_fo, l2_fo);
        REQUIRE(listEquals(sum_fo, { 9, 1, 2 }));
        destroyList(l1_fo);
        destroyList(l2_fo);
        destroyList(sum_fo);
    }
}