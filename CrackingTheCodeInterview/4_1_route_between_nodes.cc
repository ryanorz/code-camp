/**
 * Question: Route Between Nodes
 * Given a directed graph, design an algorithm to find out
 * whether there is a route between two nodes.
 * 
 * DFS: simple
 * BFS: can get the shortest path
 */

#include <vector>
using std::vector;
struct UndirectedGraphNode {
    int                          label;
    vector<UndirectedGraphNode*> neighbors;
    UndirectedGraphNode(int x)
        : label(x)
    {
    }
};

#include <queue>
#include <unordered_set>

class Solution {
private:
    std::unordered_set<UndirectedGraphNode*> node_set;

public:
    bool search(UndirectedGraphNode* start, UndirectedGraphNode* end)
    {
        if (start == nullptr || end == nullptr) {
            return false;
        }
        if (start == end) {
            return true;
        }
        std::queue<UndirectedGraphNode*> to_visit;
        node_set.insert(start);
        to_visit.push(start);
        while (!to_visit.empty()) {
            UndirectedGraphNode* node = to_visit.front();
            to_visit.pop();
            for (auto neighbor : node->neighbors) {
                if (node_set.find(neighbor) == node_set.end()) {
                    if (neighbor == end) {
                        return true;
                    } else {
                        node_set.insert(neighbor);
                        to_visit.push(neighbor);
                    }
                }
            }
        }
        return false;
    }
};
