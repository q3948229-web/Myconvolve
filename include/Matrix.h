#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include <string>
#include <iostream>

using namespace std;

// Abstract Base Class
class MatrixBase {
protected:
    Vector<Vector<double> > data;
    int rows;
    int cols;

public:
    MatrixBase(int r = 0, int c = 0) : rows(r), cols(c) {
        if (r > 0) {
            data.resize(r);
            for (int i = 0; i < r; ++i) {
                data[i].resize(c);
            }
        }
    }

    // Pure virtual function
    virtual void printInfo() const = 0; 

    virtual ~MatrixBase() {}

    void resize(int r, int c) {
        if (r <= 0 || c <= 0) return;
        data.resize(r);
        for (int i = 0; i < r; ++i) {
            data[i].resize(c);
        }
        rows = r;
        cols = c;
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }

    double getElement(int r, int c) const throw(int) {
        if (r < 0 || r >= rows || c < 0 || c >= cols) throw -1;
        return data[r][c];
    }

    void setElement(int r, int c, double val) throw(int) {
        if (r < 0 || r >= rows || c < 0 || c >= cols) throw -1;
        data[r][c] = val;
    }

    void Output(ostream& out) const {
        for (int i = 0; i < rows; ++i) {
            out << data[i] << endl;
        }
    }
};

inline ostream& operator<<(ostream& out, const MatrixBase& m) {
    m.Output(out);
    return out;
}

// Concrete Matrix Class
class Matrix : public MatrixBase {
public:
    Matrix(int r = 0, int c = 0) : MatrixBase(r, c) {}

    // Implement pure virtual function
    virtual void printInfo() const {
        cout << "Matrix (" << rows << "x" << cols << ")" << endl;
    }

    // Operators return Matrix (Concrete)
    Matrix operator+(const Matrix& other) const throw(double) {
        if (rows != other.rows || cols != other.cols) throw -1.0;
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const throw(double) {
        if (rows != other.rows || cols != other.cols) throw -1.0;
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }

    Matrix operator*(double scalar) const {
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }

    friend Matrix operator*(double scalar, const Matrix& m) {
        return m * scalar;
    }

    Matrix operator*(const Matrix& other) const throw(double) {
        if (cols != other.rows) throw -1.0;
        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                double sum = 0;
                for (int k = 0; k < cols; ++k) {
                    sum += data[i][k] * other.data[k][j];
                }
                result.setElement(i, j, sum);
            }
        }
        return result;
    }

    Matrix transpose() const {
        Matrix result(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.setElement(j, i, data[i][j]);
            }
        }
        return result;
    }
};

#endif
