#ifndef matrix_h
#define matrix_h

#include <vector>
using std::vector;

template <typename T>
class Matrix {
private:
    vector<vector<T> > matrix;

public:
    Matrix(int rows, int cols)
        : matrix(rows)
    {
        for (auto& thisRow : matrix) {
            thisRow.resize(cols);
        }
    }

    Matrix(vector<vector<T> > rhs)
        : matrix{ rhs }
    {
    }
    Matrix(vector<vector<T> >&& rhs)
        : matrix{ std::move(rhs) }
    {
    }

    const vector<T>& operator[](int row) const
    {
        return matrix[row];
    }

    vector<T>& operator[](int row)
    {
        return matrix[row];
    }

    int rows() const
    {
        return matrix.size();
    }
    int cols() const
    {
        return rows() ? matrix[0].size() : 0;
    }
};

#endif