
struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
};

#include <string>
using namespace std;

class Solution {
private:
    void getOrderString(TreeNode* node, string& str)
    {
        if (node == nullptr) {
            str.append("X");
            return;
        }
        str.append(to_string(node->value));
        getOrderString(node->left, str);
        getOrderString(node->right, str);
    }
public:
    bool subtree(TreeNode* t1, TreeNode* t2)
    {
        string str1;
        string str2;
        getOrderString(t1, str1);
        getOrderString(t2, str2);
        return str1.find(str2) != string::npos;
    }
};