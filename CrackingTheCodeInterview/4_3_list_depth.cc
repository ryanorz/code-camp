/**
 * Qestion: List of Depths
 * Given a binary tree, design an algorithm which creates a linked list of all the nodes
 * at each depth (e.g., if you have a tree with depth D, you'll have D linked lists)
 */

struct TreeNode {
    int       value;
    TreeNode* left;
    TreeNode* right;
};

#include <list>
#include <vector>
using namespace std;

class Solution {
    void createLevelLinkedListDFS(TreeNode* node, vector<list<TreeNode*> >& lists, int depth)
    {
        if (node == nullptr) {
            return;
        }
        if (depth > lists.size() - 1) {
            list<TreeNode*> new_list;
            lists.push_back(new_list);
        }
        lists[depth].push_back(node);
        createLevelLinkedListDFS(node->left, lists, depth + 1);
        createLevelLinkedListDFS(node->right, lists, depth + 1);
    }
public:
    vector<list<TreeNode*> > createLevelLinkedListDFS(TreeNode* node)
    {
        vector<list<TreeNode*> > lists;
        createLevelLinkedListDFS(node, lists, 0);
        return lists;
    }

    vector<list<TreeNode*> > createLevelLinkedListBFS(TreeNode* node)
    {
        vector<list<TreeNode*> > lists;
        list<TreeNode*> current;
        if (node != nullptr) {
            current.push_back(node);
        }
        while (current.size()) {
            lists.push_back(current);
            list<TreeNode*> prev_list(std::move(current));
            for (auto n : prev_list) {
                if (n->left) {
                    current.push_back(n->left);
                }
                if (n->right) {
                    current.push_back(n->right);
                }
            }
        }
        return lists;
    }
};
