#include <vector>
using std::vector;
struct UndirectedGraphNode {
    int label;
    vector<UndirectedGraphNode*> neighbors;
    UndirectedGraphNode(int x) : label(x) {}
};

#include <unordered_map>
#include <queue>
using HashMapUGN = std::unordered_map<UndirectedGraphNode*, UndirectedGraphNode*>;

class Solution {
private:
    HashMapUGN node_hmap;
public:
    UndirectedGraphNode* cloneGraphByDFS(UndirectedGraphNode* node)
    {
        if (node == nullptr) {
            return nullptr;
        }
        if (node_hmap.find(node) == node_hmap.end()) {
            node_hmap[node] = new UndirectedGraphNode(node->label);
            for (auto neighbor : node->neighbors) {
                node_hmap[node]->neighbors.push_back(cloneGraphByDFS(neighbor));
            }
        }
        return node_hmap[node];
    }

    UndirectedGraphNode* cloneGraphByBFS(UndirectedGraphNode* node)
    {
        if (node == nullptr) {
            return nullptr;
        }
        node_hmap[node] = new UndirectedGraphNode(node->label);
        std::queue<UndirectedGraphNode*> to_visit;
        to_visit.push(node);
        while (!to_visit.empty()) {
            UndirectedGraphNode* curr = to_visit.front();
            to_visit.pop();
            for (auto neighbor : curr->neighbors) {
                if (node_hmap.find(neighbor) == node_hmap.end()) {
                    node_hmap[neighbor] = new UndirectedGraphNode(neighbor->label);
                    to_visit.push(neighbor);
                }
                node_hmap[curr]->neighbors.push_back(node_hmap[neighbor]);
            }
        }
        return node_hmap[node];
    }
};