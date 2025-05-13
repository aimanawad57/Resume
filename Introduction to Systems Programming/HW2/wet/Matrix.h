#pragma once
#include <ostream>



class Matrix {
public:
    Matrix(const int &rows, const int &columns);

    Matrix();

    Matrix(const Matrix &matrix);

    Matrix &operator=(const Matrix &matrix);

    /**
     * transpose - takes a matrix and returns its transpose
     *
     * @param matrix - the matrix to transpose
     * @return - the transpose of the matrix
     */
    Matrix transpose();

    /**
     * rotateClockwise - rotate the matrix in the direction of Clockwise
     *
     * @param matrix - the matrix to rotate
     * @return - the matrix rotated Clockwise
     */
    Matrix rotateClockwise();

    /**
    * rotateCounterClockwise - rotate the matrix in the direction of Clockwise
    *
    * @param matrix - the matrix to rotate
    * @return - the matrix rotated counterClockwise
    */
    Matrix rotateCounterClockwise();


    int &operator()(int row, int col);
    const int &operator()(int row, int col) const;
    Matrix operator-() const;

    friend Matrix operator+(const Matrix &matrix1, const Matrix &matrix2);
    friend Matrix operator-(const Matrix &matrix1, const Matrix &matrix2);
    friend Matrix operator*(const Matrix &matrix1, const Matrix &matrix2);

    Matrix &operator+=(const Matrix &matrix);
    Matrix &operator-=(const Matrix &matrix);
    Matrix &operator*=(const Matrix &matrix);
    Matrix &operator*=(int num);

    friend bool operator==(const Matrix &matrix1, const Matrix &matrix2);

    /**
     * operator << - take ostream and and print the matrix details into
     *
     * @param os - the ostream we printing on
     * @param matrix - the matrix we are printing
     * @return the value we entered to the stream
     */
    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

    /**
     * Destructor called to free the array.
     */
    ~Matrix();
private:
    int rows;
    int columns;
    int *array;

};

bool operator!=(const Matrix &matrix1, const Matrix &matrix2);


Matrix operator*(int num, const Matrix &matrix); //multiplication with a number at left
Matrix operator*(const Matrix &matrix, int num); //multiplication with a number at right