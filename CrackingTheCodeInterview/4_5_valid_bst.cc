/**
 * Qestion: Validate BST
 * Implement a function to check if a binary tree is a binary search tree
 */

struct TreeNode {
    int       value;
    TreeNode* left;
    TreeNode* right;
};

#include <limits>
#include <vector>
using namespace std;
class Solution {
    void inOrderedTraverse(TreeNode* node, vector<int>& in_order_vector)
    {
        if (node == nullptr) {
            return;
        }
        inOrderedTraverse(node->left, in_order_vector);
        in_order_vector.push_back(node->value);
        inOrderedTraverse(node->right, in_order_vector);
    }

    bool inOrderedTraverseIncrease(TreeNode* node, int& last)
    {
        if (node == nullptr) {
            return true;
        }
        if (!inOrderedTraverseIncrease(node->left, last)) {
            return false;
        }
        if (last > node->value) {
            return false;
        }
        last = node->value;
        return inOrderedTraverseIncrease(node->right, last);
    }

public:
    bool validBST(TreeNode* node)
    {
        vector<int> in_order_vector;
        inOrderedTraverse(node, in_order_vector);
        for (int i = 0; i < in_order_vector.size() - 2; i++) {
            if (in_order_vector[i] > in_order_vector[i + 1]) {
                return false;
            }
        }
        return true;
    }

    bool validBetterBST(TreeNode* node)
    {
        int last = std::numeric_limits<int>::min();
        return inOrderedTraverseIncrease(node, last);
    }
};