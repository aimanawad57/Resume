
#pragma once

#include "Matrix.h"
#include <string>

using std::string;

class MataMvidia {

public:

    MataMvidia(const string &movie, const string &creator, const Matrix *frames,
               int length);
    MataMvidia(const MataMvidia &movie);

    Matrix &operator[](int idx);
    const Matrix &operator[](int idx) const;
    MataMvidia &operator=(const MataMvidia &movie);
    MataMvidia &operator+=(const MataMvidia &movie);
    MataMvidia &operator+=(const Matrix &matrix);
    MataMvidia operator+(const MataMvidia &matrix) const;

    friend std::ostream &operator<<(std::ostream &os, const MataMvidia &movie);

    ~MataMvidia();

private:
    string movieName;
    string creatorName;
    int length;
    Matrix *frames;
};



