/**
 * Qestion: Loop Detection
 * Given a circular linked list, implement an algorithm that returns the node at the
 * beginning of the loop.
 * DEFINITION
 * Circular linked list: A (corrupt) linked list in which a node's next pointer points to an earlier node, so
 * as to make a loop in the linked list.
 * EXAMPLE
 * Input: A - > B - > C - > D - > E - > C [the same C as earlier]
 * Output: C
 */
#include "my_list.h"
class Solution {
public:
    Node* loopDetection(Node* head)
    {
        Node* slow = head;
        Node* fast = head;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                break;
            }
        }
        if (fast == nullptr || fast->next == nullptr) {
            return nullptr;
        }
        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};
