/**
 * Qestion: Check Balanced
 * Implement a function to check if a binary tree is balanced. For the purposes of
 * this question, a balanced tree is defined to be a tree such that the heights of 
 * the two subtrees of any node never differ by more than one
 */

struct TreeNode {
    int       value;
    TreeNode* left;
    TreeNode* right;
};

#include <algorithm>

class Solution {
private:
    int treeDepth(TreeNode* node)
    {
        if (node == nullptr) {
            return 0;
        }
        return 1 + std::max(treeDepth(node->left), treeDepth(node->right));
    }

    int checkDepth(TreeNode* root)
    {
        if (root == nullptr) {
            return 0;
        }
        int left_depth = checkDepth(root->left);
        if (left_depth == -1) {
            return -1;
        }
        int right_depth = checkDepth(root->right);
        if (right_depth == -1) {
            return -1;
        }
        if (std::abs(left_depth - right_depth) > 1) {
            return -1;
        }
        return 1 + std::max(left_depth, right_depth);
    }

public:
    bool isBalancedDFS(TreeNode* root)
    {
        if (root == nullptr) {
            return true;
        }
        if (std::abs(treeDepth(root->left) - treeDepth(root->right)) > 1) {
            return false;
        }
        return (isBalancedDFS(root->left) && isBalancedDFS(root->right));
    }

    bool isBalanced(TreeNode* root)
    {
        return (checkDepth(root) > 0);
    }
};