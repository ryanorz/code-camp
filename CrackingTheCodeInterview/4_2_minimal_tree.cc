/**
 * Qestion: Minimal Tree
 * Given a sorted (increasing order) array with unique integer elements, 
 * write an algorithm to create a binary search tree with minimal height.
 */

struct TreeNode {
    int       value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x)
        : value(x)
    {
    }
};

#include <vector>
using namespace std;
class Solution {
private:
    TreeNode* createMinimalTree(vector<int>& array, int start, int end)
    {
        if (start > end) {
            return nullptr;
        }
        int       mid  = (start + end) / 2;
        TreeNode* root = new TreeNode(array[mid]);
        root->left     = createMinimalTree(array, start, mid - 1);
        root->right    = createMinimalTree(array, mid + 1, end);
        return root;
    }

public:
    TreeNode* createMinimalTree(vector<int>& array)
    {
        return createMinimalTree(array, 0, array.size() - 1);
    }
};
