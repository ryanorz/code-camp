/**
 * Qestion: Power Set
 * Write a method to return all subsets of a set.
 */
#include <vector>
using namespace std;
class Solution {
public:
    vector<vector<int>> getSubset(const vector<int> &theSet)
    {
        vector<vector<int>> subsets;
        vector<int> nullset;
        subsets.push_back(nullset);
        int set_size = theSet.size();
        for (auto element : theSet) {
            int subsets_size = subsets.size();
            for (int j = 0; j < subsets.size(); j++) {
                vector<int> subset = subsets[j];
                subset.push_back(element);
                subsets.push_back(std::move(subset));
            }
        }
        return subsets;
    }
};
