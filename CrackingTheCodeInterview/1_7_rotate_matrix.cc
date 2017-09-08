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
