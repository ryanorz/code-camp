/**
 * Qestion: Intersection
 * Given two (singly) linked lists, determine if the two lists intersect. Return the 
 * intersecting node. Note that the intersection is defined based on reference, not value. That is, if the 
 * kth node of the first linked list is the exact same node (by reference) as the jth node of the second
 * linked list, then they are intersecting.
 */
#include "my_list.h"

class Solution {
private:
    struct TailAndLength {
        Node* tail;
        int   len;
    };

    TailAndLength getListTailAndLength(Node* head)
    {
        TailAndLength result = { nullptr, 0 };
        while (head) {
            result.tail = head;
            result.len++;
            head = head->next;
        }
        return result;
    }

public:
    Node* intersection(Node* l1, Node* l2)
    {
        if (!l1 || !l2) {
            return nullptr;
        }
        TailAndLength tal1 = getListTailAndLength(l1);
        TailAndLength tal2 = getListTailAndLength(l2);
        if (tal1.tail != tal2.tail) {
            return nullptr;
        }
        Node* longer   = (tal1.len > tal2.len) ? l1 : l2;
        Node* shorter  = (tal1.len > tal2.len) ? l2 : l1;
        int   len_diff = (tal1.len > tal2.len) ? (tal1.len - tal2.len) : (tal2.len - tal1.len);
        while (len_diff--) {
            longer = longer->next;
        }
        while (longer != shorter) {
            longer  = longer->next;
            shorter = shorter->next;
        }
        return longer;
    }
};