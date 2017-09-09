/**
 * Qestion: Zero Matrix
 * Write an algorithm such that if an element in an MxN matrix is 0, 
 * its entire row and column are set to 0.
 * 
 * @Time O(n*m)
 * @Space O(n+m)
 */

#include "matrix.h"
#include <set>
using std::set;

class Solution {
public:
    void zeroMatrix(Matrix<int>& matrix)
    {
        int m = matrix.rows();
        int n = matrix.cols();

        set<int> zeroRows;
        set<int> zeroCols;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 0) {
                    zeroRows.insert(i);
                    zeroCols.insert(j);
                }
            }
        }
        for (auto i : zeroRows) {
            for (int j = 0; j < n; j++) {
                matrix[i][j] = 0;
            }
        }
        for (auto j : zeroCols) {
            for (int i = 0; i < m; i++) {
                matrix[i][j] = 0;
            }
        }
    }
};

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("", "")
{
    vector<vector<int> > from = {
        { 1, 0, 2 },
        { 1, 2, 3 },
        { 0, 2, 3 }
    };
    vector<vector<int> > to = {
        { 0, 0, 0 },
        { 0, 0, 3 },
        { 0, 0, 0 }
    };
    Matrix<int> matrix{ from };
    Solution solution;
    solution.zeroMatrix(matrix);
    REQUIRE(matrix == to);
}
