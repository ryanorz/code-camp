/**
 * Qestion: Triple Step
 * A child is running up a staircase with n steps and can hop either 1 step, 2 steps, or 3
 * steps at a time. Implement a method to count how many possible ways the child can run up the
 * stairs.
 */
#include <vector>
using namespace std;

class Solution {
public:
    int waysCountUpToStairs_BruteForce(int n)
    {
        if (n < 0) {
            return 0;
        } else if (n == 0) {
            return 1;
        }
        return (waysCountUpToStairs_BruteForce(n - 1)
            + waysCountUpToStairs_BruteForce(n - 2)
            + waysCountUpToStairs_BruteForce(n - 3));
    }

    int waysCountUpToStairs_DP(int n)
    {
        vector<int> records(n + 1, -1);
        records[0] = 1;
        return waysCountUpToStairs_DP(n, records);
    }

    int waysCountUpToStairs_DP(int n, vector<int>& records)
    {
        if (n < 0) {
            return 0;
        }
        if (records[n] == -1) {
            records[n] = (waysCountUpToStairs_DP(n - 1, records)
                + waysCountUpToStairs_DP(n - 2, records)
                + waysCountUpToStairs_DP(n - 3, records));
        }
        return records[n];
    }
};