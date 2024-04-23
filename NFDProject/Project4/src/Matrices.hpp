#pragma once

#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

template <typename T>
class Matrices {
private:
    size_t rows;
    size_t cols;
    int* refCount;
    T* data;
    const Matrices<T>* parent_mat;

public:
    // Constructor
    Matrices();

    Matrices(
        size_t rows,
        size_t cols);

    Matrices(
        T* data,
        size_t rows,
        size_t cols);

    Matrices(
        string path,
        size_t rows,
        size_t cols);
    // Copy constructor
    Matrices(const Matrices<T>& mat);
    // move constructor
    Matrices(Matrices&& mat) noexcept;
    // Destructor
    ~Matrices();
    // operator reload
    Matrices& operator=(const Matrices& mat);
    bool operator==(const Matrices& mat);

    Matrices operator+(const Matrices& mat); // plus a mat.
    Matrices operator+(T number); // plus one number.
    Matrices& operator+=(const Matrices& mat);
    Matrices& operator+=(T number);

    Matrices operator-(const Matrices& mat);
    Matrices operator-(T number);
    Matrices& operator-=(const Matrices& mat);
    Matrices& operator-=(T number);

    Matrices operator*(const Matrices& mat);
    Matrices operator*(T number);
    Matrices& operator*=(const Matrices& mat);
    Matrices& operator*=(T number);

    Matrices operator^(const Matrices& mat);

    T& operator()(int row, int col);

    // Friend Function
    template <typename Y>
    friend Matrices<Y> operator+(Y number, const Matrices<Y>& mat); // number plus a mat;

    template <typename Y>
    friend Matrices<Y> operator-(Y number, const Matrices<Y>& mat); // number plus a mat;

    template <typename Y>
    friend Matrices<Y> operator*(Y number, const Matrices<Y>& mat); // number plus a mat;

    template <typename Y>
    friend std::ostream& operator<<(std::ostream& os, const Matrices<Y>& mat);

    // Member function
    void print_mat() const; // print this mat.
    void print_shape() const;
    size_t get_rows();
    size_t get_cols();
    int get_refCount();
    void transpose();
    Matrices clone(const Matrices& mat);
    Matrices ROI(size_t row_start, size_t row_end, size_t col_start, size_t col_end);
    void ROI_adjust(size_t row_start, size_t row_end, size_t col_start, size_t col_end);

    void determinant();
};
