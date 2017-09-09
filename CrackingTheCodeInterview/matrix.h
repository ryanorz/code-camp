#ifndef matrix_h
#define matrix_h

#include <iostream>
#include <vector>
using std::vector;

template <typename T>
class Matrix {
private:
    vector<vector<T> > array;

public:
    Matrix(int rows, int cols)
        : array(rows)
    {
        for (auto& theRow : array) {
            theRow.resize(cols);
        }
    }
    Matrix(vector<vector<T> > v)
        : array{ v }
    {
    }

    Matrix(vector<vector<T> >&& v)
        : array{ std::move(v) }
    {
    }

    const vector<T>& operator[](int row) const
    {
        return array[row];
    }

    vector<T>& operator[](int row)
    {
        return array[row];
    }

    int rows() const
    {
        return array.size();
    }

    int cols() const
    {
        return rows() ? array[0].size() : 0;
    }

    bool operator==(const Matrix& rhs) const
    {
        return array == rhs.array;
    }

    void display() const
    {
        std::cout << ">> " << rows() << " x " << cols() << "\n";
        for (int i = 0; i < rows(); i++) {
            std::cout << "[ ";
            for (int j = 0; j < cols(); j++) {
                std::cout << array[i][j] << " ";
            }
            std::cout << "]\n";
        }
    }
};

#endif