/**
 * Qestion: First Common Ancestor
 * Design an algorithm and write code to find the first common ancestor 
 * of two nodes in a binary tree. Avoid storing additional nodes in a data structure.
 * NOTE: This is not necessarily a binary search tree
 */
#include <vector>
#include <algorithm>
using namespace std;

 struct TreeNode {
     int value;
     TreeNode* parent;
     vector<TreeNode*> children;
 };

 class Solution {
 private:
    int getLevel(TreeNode* node)
    {
        if (node == nullptr) {
            return 0;
        }
        int level = 0;
        while (node) {
            level++;
            node = node->parent;
        }
        return level;
    }

 public:
    TreeNode* commonAncestor(TreeNode *p, TreeNode* q)
    {
        if (!p || !q) {
            return nullptr;
        }
        int p_level = getLevel(p);
        int q_level = getLevel(q);
        TreeNode* longer = (p_level > q_level) ? p : q;
        TreeNode* shorter = (p_level > q_level) ? q : p;
        int diff = std::abs(p_level - q_level);
        while (diff--) {
            longer = longer->parent;
        }
        while (longer) {
            if (longer == shorter) {
                return longer;
            }
            longer = longer->parent;
            shorter = shorter->parent;
        }
        return nullptr;

    }
 };