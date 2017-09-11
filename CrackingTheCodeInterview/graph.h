#ifndef graph_h
#define graph_h

#include <vector>
#include <iostream>
using std::vector;

class GraphMatrix {
private:
    vector<vector<int> > array;

public:
    GraphMatrix(int size)
        : array(size)
    {
        for (auto& theRow : array) {
            theRow.resize(size);
        }
    }
    GraphMatrix(vector<vector<int> > v)
        : array{ v }
    {
    }

    GraphMatrix(vector<vector<int> >&& v)
        : array{ std::move(v) }
    {
    }

    const vector<int>& operator[](int row) const
    {
        return array[row];
    }

    vector<int>& operator[](int row)
    {
        return array[row];
    }

    int size() const
    {
        return array.size();
    }

    bool operator==(const GraphMatrix& rhs) const
    {
        return array == rhs.array;
    }

    void display() const
    {
        for (int i = 0; i < size(); i++) {
            std::cout << "[ ";
            for (int j = 0; j < size(); j++) {
                std::cout << array[i][j] << " ";
            }
            std::cout << "]\n";
        }
    }
};

#include <string>
using std::string;

struct Node {
    string name;
    bool visited;
    bool marked;
    vector<Node*> children;
};

struct Graph {
    vector<Node*> nodes;
};

// DFS: Depth-First Search
void searchDFS(Node *root)
{
    if (root == nullptr) {
        return;
    }
    // visit root
    root->visited = true;
    for (auto node : root->children) {
        if (!node->visited) {
            searchDFS(node);
        }
    }
}

#include <queue>
using std::queue;

// BFS : Breadth-First Search
void searchBFS(Node *root) {
    queue<Node *> theQueue;
    root->marked = true;
    theQueue.push(root);
    while (!theQueue.empty()) {
        auto node = theQueue.front();
        theQueue.pop();
        // visit node
        node->visited = true;
        for (auto n : node->children) {
            if (!n->marked) {
                n->marked = true;
                theQueue.push(n);
            }
        }
    }
}

#endif