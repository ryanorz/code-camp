/**
 * Qestion: Magic Index
 * A magic index in an array A[1 .•.n-1] is defined to be an index such that A[i] = i. 
 * Given a sorted array of distinct integers, write a method to find a magic index, if one exists, in array A.
 * FOLLOW UP
 * What if the values are not distinct?
 * 
 * 0    1   2   3   4   5   6   7   8
 * -1   0   2   4   5   5   5   6   8
 */

#include <vector>
using namespace std;

class Solution {
public:
    int magicIndexBruteForce(vector<int> &array)
    {
        size_t size = array.size();
        for (size_t i = 0; i < size; i++) {
            if (i == array[i]) {
                return i;
            }
        }
        return -1;
    }

    int magicIndexBinarySearch(vector<int> &array, int start, int end)
    {
        if (start > end) {
            return -1;
        }
        int mid = (start + end) / 2;
        if (mid == array[mid]) {
            return mid;
        } else if (mid < array[mid]) {
            return magicIndexBinarySearch(array, start, mid - 1);
        } else {
            return magicIndexBinarySearch(array, mid + 1, end);
        }
    }

    int magicIndexFastLimitDup(vector<int> &array, int start, int end)
    {
        if (start > end) {
            return -1;
        }
        int mid = (start + end) / 2;
        if (mid == array[mid]) {
            return mid;
        }
        // search left
        int left = magicIndexFastLimitDup(array, start, std::min(array[mid], mid - 1));
        if (left >= 0) {
            return left;
        }
        // search right
        int right = magicIndexFastLimitDup(array, std::max(array[mid], mid + 1), end);
        if (right >= 0) {
            return right;
        }
        return -1;
    }
};