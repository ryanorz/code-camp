/**
 * Question: Build Order
 * You are given a list of projects and a list of dependencies (which is a list of pairs of
 * projects, where thesecond projectis dependent on the first project). All of a project's dependencies
 * must be built before the project is. Find a build order that will allow the projects to be built. If there
 * is no valid build order, return an error.
 * 
 * EXAMPLE
 * Input:
 * projects: a, b, c, d, e, f
 * dependencies: (a, d), (f, b), (b, d), (f, a), (d, c)
 * Output: f, e, a, b, d, c
 */
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Project {
private:
    vector<Project*> children_;
    unordered_map<string, Project*> mp;
    string name_;
    int    dependencies_;

public:
    Project(string str)
        : name_(str)
        , dependencies_(0)
    {
    }

    void addNeighbor(Project* node)
    {
        if (mp.find(node->name_) == mp.end()) {
            children_.push_back(node);
            mp[node->name_] = node;
            dependencies_++;
        }
    }

    void increaseDependencies()
    {
        dependencies_++;
    }

    void decreaseDependencies()
    {
        dependencies_--;
    }

    string name() const
    {
        return name_;
    }

    vector<Project*>& children()
    {
        return children_;
    }

    int dependencies() const
    {
        return dependencies_;
    }
};

class ProjectGraph {
private:
    unordered_map<string, Project*> mp_;
    vector<Project*> nodes_;

public:
    Project* getOrCreateNode(string& name)
    {
        if (mp_.find(name) == mp_.end()) {
            Project* node = new Project(name);
            nodes_.push_back(node);
            mp_[name] = node;
        }
        return mp_[name];
    }

    void addEdge(string start_name, string end_name)
    {
        Project* start = getOrCreateNode(start_name);
        Project* end   = getOrCreateNode(end_name);
        start->addNeighbor(end);
    }

    void buildProjectGraph(vector<string>& projects, vector<pair<string, string> >& dependencies)
    {
        for (auto name : projects) {
            getOrCreateNode(name);
        }
        for (auto dep : dependencies) {
            addEdge(dep.first, dep.second);
        }
    }

    vector<Project*>& nodes()
    {
        return nodes_;
    }
};

class Solution {
private:
    void addNonDependent(vector<Project*> &order, const vector<Project*>& nodes)
    {
        for (auto node : nodes) {
            if (node->dependencies() == 0) {
                order.push_back(node);
            }
        }
    }

    vector<Project*> orderProjects(const vector<Project*>& nodes)
    {
        vector<Project*> order;
        addNonDependent(order, nodes);
        int orderSize = order.size();
        int toBeProcessed = 0;
        while (toBeProcessed < order.size()) {
            Project* current = order[toBeProcessed];
            vector<Project*> children = current->children();
            for (auto child : children) {
                child->decreaseDependencies();
            }
            addNonDependent(order, children);
            toBeProcessed++;
        }
        return order;
    }

public:
    vector<Project*> findBuildOrder(vector<string>& projects, vector<pair<string, string> >& dependencies)
    {
        ProjectGraph project_graph;
        project_graph.buildProjectGraph(projects, dependencies);
        return orderProjects(project_graph.nodes());
    }
};