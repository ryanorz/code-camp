/**
 * Question: Rotate Matrix
 * Given an image represented by an NxN matrix, where each pixel in the image is 4
 * bytes, write a method to rotate the image by 90 degrees. Can you do this in place?
 * 
 * @time O(n^2)
 */
#include "matrix.h"

 class Solution {
 public:
    bool rotateMatrix(Matrix<int> &matrix) {
        if (matrix.rows() != matrix.cols() || matrix.rows() == 0) {
            return false;
        }
        int n = matrix.rows();
        for (int layer = 0; layer < n/2; layer++) {
            int first = layer;
            int last = n - layer - 1;
            for (int idx = first; idx < last; idx++) {
                int offset = idx - first;
                int topElement = matrix[first][idx];
                // left -> top
                matrix[first][idx] = matrix[last-offset][first];
                // bottom -> left
                matrix[last-offset][first] = matrix[last][last-offset];
                // right -> bottom
                matrix[last][last-offset] = matrix[idx][last];
                // top -> right
                matrix[idx][last] = topElement;
            }
        }
        return true;
    }
 };

 #define CATCH_CONFIG_MAIN
 #include "catch.hpp"
 
 TEST_CASE("", "")
 {
     vector<vector<int> > from = {
         { 1, 2, 3 },
         { 4, 5, 6 },
         { 7, 8, 9 }
     };
     vector<vector<int> > to = {
         { 7, 4, 1 },
         { 8, 5, 2 },
         { 9, 6, 3 }
     };
     Matrix<int> matrix{ from };
     Solution solution;
     solution.rotateMatrix(matrix);
     REQUIRE(matrix == to);
 }
