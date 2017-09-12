/**
 * Qestion: Robot in a Grid
 * Imagine a robot sitting on the upper left corner of grid with r rows andc columns.
 * The robot can only move in two directions, right and down, but certain cells are"off limits"such that
 * the robot cannot step on them. Design an algorithm to find a path for the robot from the top left to
 * the bottom right.
 */
#include "matrix.h"
#include <set>
#include <vector>
using namespace std;

using PathArray = vector<pair<int, int> >;

using PointSet = set<pair<int, int> >;

class Solution {
    bool getPath(Matrix<bool>& maze, int row, int col, PathArray& path, PointSet& failed_points)
    {
        if (row < 0 || col < 0
            || (maze.rows() - 1) < row || (maze.cols() - 1) < col
            || !maze[row][col]) {
            return false;
        }
        if (row == 0 && col == 0) {
            return true;
        }
        if (failed_points.find(make_pair(row, col)) == failed_points.end()) {
            return false;
        }
        if (getPath(maze, row, col - 1, path, failed_points)
            || getPath(maze, row - 1, col, path, failed_points)) {
            path.push_back(make_pair(row, col));
            return true;
        }
        failed_points.insert(make_pair(row, col));
        return false;
    }

public:
    PathArray getPath(Matrix<bool>& maze)
    {
        PathArray path;
        PointSet  failed_points;
        if (getPath(maze, maze.rows() - 1, maze.cols() - 1, path, failed_points)) {
            return path;
        } else {
            return PathArray();
        }
    }
};
