/**
 * Qestion: Random Node
 * You are implementing a binary search tree class from scratch, which, in addition
 * to insert, find, and delete, has a method getRandomNode() which returns a random node
 * from the tree. All nodes should be equally likely to be chosen. Design and implement an algorithm
 * for getRandomNode, and explain how you would implement the rest of the methods.
 */
#include <cstdlib>
#include <ctime>

struct TreeNode {
    int       data_;
    TreeNode* left;
    TreeNode* right;
    int       size_;

    TreeNode(int data)
        : data_(data)
        , size_(1)
    {
    }

    int data() const { return data_; }
    int size() const { return size_; }

    int randomInt(int size) const
    {
        srand(time(nullptr));
        return rand() % size;
    }

    TreeNode* getRandomNode()
    {
        return getIthNode(randomInt(size_));
    }

    TreeNode* getIthNode(int index)
    {
        int left_size = (left == nullptr) ? 0 : left->size();
        if (index < left_size) {
            return left->getIthNode(index);
        } else if (index == left_size) {
            return this;
        } else {
            return right->getIthNode(index - left_size - 1);
        }
    }

    void insert(int data)
    {
        TreeNode*& node = (data <= data_) ? left : right;
        if (node == nullptr) {
            node = new TreeNode(data);
        } else {
            node->insert(data);
        }
        size_++;
    }

    TreeNode* find(int data)
    {
        if (data == data_) {
            return this;
        } else if (data < data_) {
            return (left == nullptr) ? nullptr : left->find(data);
        } else {
            return (right == nullptr) ? nullptr : right->find(data);
        }
    }

};