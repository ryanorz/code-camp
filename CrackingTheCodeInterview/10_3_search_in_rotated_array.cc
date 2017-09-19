#include <vector>
using namespace std;
class Solution {
public:
    int search(int needle, const vector<int>& array, int start, int end)
    {
        if (start > end) {
            return -1;
        }
        int mid = (start + end) / 2;
        if (array[mid] == needle) {
            return mid;
        }
        if (array[start] < array[mid]) { // left is normal order
            if (needle >= array[start] && needle < array[mid]) {
                return search(needle, array, start, mid - 1);
            } else {
                return search(needle, array, mid + 1, end);
            }
        } else if (array[start] > array[mid]) { // right is normal order
            if (array[mid] < needle && needle > array[end]) {
                return search(needle, array, mid + 1, end);
            } else {
                return search(needle, array, start, mid - 1);
            }
        } else { // array[start] == array[mid]
            if (array[mid] != array[end]) { // right is diff
                return search(needle, array, mid + 1, end);
            } else {
                int index = search(needle, array, start, mid - 1);
                if (index == -1) {
                    index = search(needle, array, mid + 1, end);
                }
                return index;
            }
        }
        return -1;
    }
};
