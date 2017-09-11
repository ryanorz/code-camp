/**
 * Qestion: Successor
 * Write an algorithm to find the "next" node (i.e., in-order successor) 
 * of a given node in a binary search tree. You may assume that each node has a link to its parent.
 * 
 */
struct DoubleTreeNode {
    int             value;
    DoubleTreeNode* parent;
    DoubleTreeNode* left;
    DoubleTreeNode* right;
};

class Solution {
private:
    DoubleTreeNode* getLeftMostChild(DoubleTreeNode* root)
    {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->left) {
            return getLeftMostChild(root->left);
        } else {
            return root;
        }
    }

public:
    DoubleTreeNode* successor(DoubleTreeNode* node)
    {
        if (node->right) {
            return getLeftMostChild(node->right);
        } else {
            DoubleTreeNode* curr   = node;
            DoubleTreeNode* parent = curr->parent;
            while (parent && parent->left != curr) {
                curr   = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }
};
