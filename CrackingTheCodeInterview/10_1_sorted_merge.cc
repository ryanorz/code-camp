/**
 * Sorted Merge: You are given two sorted arrays, A and B, 
 * where A has a large enough buffer at the end to hold B.
 *  Write a method to merge B into A in sorted order
 */
class Solution {
public:
    void sortedMerge(int a[], int b[], int lenA, int lenB)
    {
        int indexAB = lenA + lenB - 1;
        int indexA  = lenA - 1;
        int indexB  = lenB - 1;
        while (indexB >= 0) {
            if (a[indexA] > b[indexB]) {
                a[indexAB] = a[indexA];
                indexA--;
            } else {
                a[indexAB] = a[indexB];
                indexB--;
            }
            indexAB--;
        }
    }
};
