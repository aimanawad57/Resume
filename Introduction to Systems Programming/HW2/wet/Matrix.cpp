#include<iostream>
#include "Matrix.h"
#include "Utilities.h"

Matrix::Matrix(const int &rows, const int &columns) : rows(rows),
                                                      columns(columns) {
    if (rows == 0 || columns == 0) {
        array = nullptr;
    }
    else {
        array = new int[rows * columns]();
    }
}


Matrix::Matrix() : rows(0), columns(0), array(nullptr) {}


Matrix::Matrix(const Matrix &matrix) : rows(matrix.rows),
                                       columns(matrix.columns) {
    if (rows == 0 || columns == 0) {
        array = nullptr;
    }
    else {
        array = new int[rows * columns];
        for (int i = 0; i < rows * columns; i++) {
            array[i] = matrix.array[i];
        }
    }
}

Matrix &Matrix::operator=(const Matrix &matrix) {
    if (this == &matrix) {
        return *this;
    }
    rows = matrix.rows;
    columns = matrix.columns;

    int *new_array = new int[rows * columns];
    for (int i = 0; i < rows * columns; i++) {
        new_array[i] = matrix.array[i];
    }
    //delete the old array and assign the new one
    delete[] array;
    array = new_array;
    return *this;
}


Matrix &Matrix::operator+=(const Matrix &matrix) {
    if (rows != matrix.rows || columns != matrix.columns) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
    for (int i = 0; i < rows * columns; i++) {
        array[i] += matrix.array[i];
    }
    return *this;
}


Matrix &Matrix::operator-=(const Matrix &matrix) {
    *this += (-1 * matrix);
    return *this;
}

Matrix operator+(const Matrix &matrix1, const Matrix &matrix2) {
    Matrix temp(matrix1);
    return (temp += matrix2);
}


Matrix operator-(const Matrix &matrix1, const Matrix &matrix2) {
    Matrix temp(matrix1);
    return (temp -= matrix2);
}


Matrix &Matrix::operator*=(const Matrix &matrix) {
    if (columns != matrix.rows) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
    Matrix temp(rows, matrix.columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < matrix.columns; j++) {
            int sum = 0;
            for (int k = 0; k < columns; k++) {
                sum += (*this)(i, k) * matrix(k, j);
            }
            temp(i, j) = sum;
        }
    }
    *this = temp;
    return *this;
}

Matrix operator*(const Matrix &matrix1, const Matrix &matrix2) {
    Matrix temp(matrix1);
    return temp *= matrix2;
}

int &Matrix::operator()(int row, int col) {
    if (row >= rows || row < 0 || col >= columns || col < 0) {
        exitWithError(MatamErrorType::OutOfBounds);
    }
    return array[row * columns + col];


}


const int &Matrix::operator()(int row, int col) const {
    if (row >= rows || row < 0 || col >= columns || col < 0) {
        exitWithError(MatamErrorType::OutOfBounds);
    }
    return array[row * columns + col];
}


Matrix::~Matrix() {
    delete[] array;
}

Matrix Matrix::operator-() const {
    Matrix result(rows, columns);
    for (int i = 0; i < rows * columns; i++) {
        result.array[i] = -array[i];

    }
    return result;
}


Matrix operator*(const Matrix &matrix, int num) {
    Matrix result(matrix);
    result *= num;
    return result;
}


Matrix operator*(int num, const Matrix &matrix) {
    return matrix * num;
}


Matrix &Matrix::operator*=(int num) {
    if (array == nullptr) {
        return *this;
    }
    for (int i = 0; i < rows * columns; i++) {
        array[i] = array[i] * num;

    }
    return *this;

}


bool operator==(const Matrix &matrix1, const Matrix &matrix2) {
    if (matrix1.rows != matrix2.rows || matrix1.columns != matrix2.columns) {
        return false;
    }
    for (int i = 0; i < matrix1.rows * matrix1.columns; i++) {
        if (matrix1.array[i] != matrix2.array[i]) {
            return false;
        }
    }
    return true;
}


bool operator!=(const Matrix &matrix1, const Matrix &matrix2) {
    if (matrix1 == matrix2) {
        return false;
    }
    else {
        return true;
    }
}


Matrix Matrix::rotateClockwise() {
    Matrix rotated(columns, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            rotated(j, rows - 1 - i) = (*this)(i, j);
        }
    }
    return rotated;
}

Matrix Matrix::rotateCounterClockwise() {
    return (*this).rotateClockwise().rotateClockwise().rotateClockwise();
}

Matrix Matrix::transpose() {
    if (array == nullptr) {
        return *this;
    }
    Matrix transposed(columns, rows);
    for (int i = 0; i < transposed.rows; i++) {
        for (int j = 0; j < transposed.columns; j++) {
            transposed(i, j) = (*this)(j, i);
        }
    }
    return transposed;
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
    if (matrix.array == nullptr) {
        return os;
    }
    for (int i = 0; i < matrix.rows; i++) {
        os << "|";
        for (int j = 0; j < matrix.columns; j++) {
            os << matrix.array[(i * matrix.columns) + j] << "|";
        }
        os << "\n";
    }
    return os;
}
