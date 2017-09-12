/**
 * Qestion: Paths with Sum
 * You are given a binary tree in which each node contains an integer value (which
 * might be positive or negative). Design an algorithm to count the number of paths that sum to a
 * given value. The path does not need to start or end at the root or a leaf, but it must go downwards
 * (traveling only from parent nodes to child nodes)
 */

struct TreeNode {
    int       value;
    TreeNode* left;
    TreeNode* right;
};

#include <unordered_map>
using namespace std;

class Solution {
private:
    int countPathsWithSum(TreeNode* node, int target_sum, int running_sum, unordered_map<int, int>& path_count)
    {
        if (node == nullptr) {
            return 0;
        }

        // count paths with sum ending at the current node
        running_sum += node->value;
        int sum         = running_sum - target_sum;
        int total_paths = path_count[sum];
        // if running_sum equals target_sum, then one additional path starts at root
        if (running_sum == target_sum) {
            total_paths++;
        }
        // Increment pathCount, recurse, then decrement pathCount.
        path_count[running_sum]++;
        total_paths += countPathsWithSum(node->left, target_sum, running_sum, path_count);
        total_paths += countPathsWithSum(node->right, target_sum, running_sum, path_count);
        path_count[running_sum]--;
        return total_paths;
    }

public:
    // O(n^2)
    int countPathsWithSum_BruteForce(TreeNode* root, int sum)
    {
        if (root == nullptr) {
            return 0;
        }
        int count = 0;
        if (root->value == sum) {
            count++;
        }
        count += countPathsWithSum_BruteForce(root->left, sum - root->value);
        count += countPathsWithSum_BruteForce(root->right, sum - root->value);
        count += countPathsWithSum_BruteForce(root->left, sum);
        count += countPathsWithSum_BruteForce(root->right, sum);
        return count;
    }

    int countPathsWithSum(TreeNode* root, int sum)
    {
        unordered_map<int, int> path_count;
        return countPathsWithSum(root, sum, 0, path_count);
    }
};