#include "MataMvidia.h"
#include<ostream>

using std::string;

MataMvidia::MataMvidia(const string &movie, const string &creator,
                       const Matrix *frames,
                       int length) : movieName(movie),
                                     creatorName(creator),
                                     length(length),
                                     frames(new Matrix[length]()) {
    for (int i = 0; i < length; i++) {
        this->frames[i] = frames[i];
    }
}

MataMvidia::MataMvidia(const MataMvidia &movie) : MataMvidia(movie.movieName, movie.creatorName,
                                                             movie.frames,
                                                             movie.length) {}

MataMvidia::~MataMvidia() {
    delete[] frames;
}

MataMvidia &MataMvidia::operator=(const MataMvidia &movie) {
    if (this == &movie) {
        return *this;
    }
    movieName = movie.movieName;
    creatorName = movie.creatorName;
    length = movie.length;
    delete[] frames;
    frames = new Matrix[length];

    for (int i = 0; i < length; i++) {
        frames[i] = movie.frames[i];
    }
    return *this;
}


Matrix &MataMvidia::operator[](int idx) {
    return (frames[idx]);
}

const Matrix &MataMvidia::operator[](int idx) const {
    return (frames[idx]);
}


MataMvidia &MataMvidia::operator+=(const MataMvidia &movie) {
    int org_length = length;
    Matrix *temp = new Matrix[length];
    for (int i = 0; i < length; i++) {
        temp[i] = frames[i];
    }
    delete[] frames;

    length += movie.length;
    frames = new Matrix[length];

    int i = 0;
    for (; i < org_length; i++) {
        frames[i] = temp[i];
    }

    for (; i < length; i++) {
        frames[i] = movie.frames[i - org_length];
    }

    delete[] temp;
    return *this;
}

MataMvidia MataMvidia::operator+(const MataMvidia &movie) const {
    MataMvidia temp(*this);
    return (temp += movie);
}


MataMvidia &MataMvidia::operator+=(const Matrix &matrix) {
    Matrix *temp = new Matrix[length + 1];
    for (int i = 0; i < length; i++) {
        temp[i] = frames[i];
    }
    temp[length] = matrix;
    delete[] frames;
    frames = temp;
    length++;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const MataMvidia &movie) {
    os << "Movie Name: " << movie.movieName << std::endl;
    os << "Author: " << movie.creatorName << std::endl;
    os << std::endl;
    for (int i = 0; i < movie.length; i++) {

        os << "Frame " << i << ":" << std::endl;
        os << movie.frames[i] << std::endl;
    }
    os << "-----End of Movie-----" << std::endl;
    return os;
}
