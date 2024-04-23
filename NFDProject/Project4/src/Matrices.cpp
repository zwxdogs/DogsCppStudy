#include "Matrices.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

// ********************* Constructor **********************************//
template <typename T>
Matrices<T>::Matrices()
    : rows(0)
    , cols(0)
    , data(nullptr)
    , refCount(nullptr)
    , parent_mat(nullptr)
{
}

template <typename T>
Matrices<T>::Matrices(size_t rows, size_t cols)
    : rows(rows)
    , cols(cols)
    , data(nullptr)
    , refCount(nullptr)
    , parent_mat(nullptr)
{
    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("The cols or rows is zero!");
    }
}

template <typename T>
Matrices<T>::Matrices(T* data, size_t rows, size_t cols)
    : rows(rows)
    , cols(cols)
    , parent_mat(nullptr)
{
    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("The cols or rows is zero!");
    }
    if (data == nullptr) {
        throw std::invalid_argument("Input data is NULL!");
    }
    this->refCount = new int;
    (*(this->refCount)) = 1;
    this->data = data;
}

template <typename T>
Matrices<T>::Matrices(string path, size_t rows, size_t cols)
    : rows(rows)
    , cols(cols)
    , parent_mat(nullptr)
{
    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("The cols or rows is zero!");
    }
    if (path.empty()) {
        throw std::invalid_argument("path is empty!");
    }
    this->data = new T[rows * cols];
    this->refCount = new int;
    (*(this->refCount)) = 1;

    ifstream read_mat;
    read_mat.open(path, ios::in);
    if (!read_mat.is_open()) {
        throw std::invalid_argument("read fail, Can't Open this path!");
    }
    string temp_str;
    size_t index = 0;
    while (read_mat >> temp_str) {
        std::istringstream is(temp_str);
        T temp_number;
        is >> temp_number;
        this->data[index] = temp_number;
        index++;
    }
}

template <typename T>
Matrices<T>::Matrices(const Matrices<T>& mat)
    : rows(mat.rows)
    , cols(mat.cols)
{
    if (mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (mat.data == nullptr) {
        this->data = nullptr;
        this->refCount = nullptr;
        this->parent_mat = nullptr;
    } else {
        this->data = mat.data;
        this->parent_mat = mat.parent_mat;
        this->refCount = mat.refCount;
        (*(this->refCount))++;
    }
}

template <typename T>
Matrices<T>::Matrices(Matrices&& mat) noexcept
    : rows(mat.rows)
    , cols(mat.cols)
{
    this->data = mat.data;
    this->refCount = mat.refCount;
    this->parent_mat = mat.parent_mat;
    mat.data = nullptr;
    mat.refCount = nullptr;
}

// ********************* Destructor ***********************************//

template <typename T>
Matrices<T>::~Matrices()
{
    if (this->data) {
        if ((*(this->refCount)) == 1) {
            if (this->parent_mat) {
                delete[] this->parent_mat->data;
            } else {
                delete[] this->data;
                this->data = nullptr;
                delete this->refCount;
                this->refCount = nullptr;
            }
        } else {
            (*(this->refCount))--;
        }
    }
}

// ********************* Operator Reload ******************************//

template <typename T>
Matrices<T>& Matrices<T>::operator=(const Matrices<T>& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this == &mat) {
        return *this;
    }
    if (mat.data == nullptr) {
        throw std::invalid_argument("The mat right of = is null data!");
    }
    if (this->data) {
        if ((*(this->refCount)) == 1) {
            if (this->parent_mat) {
                delete[] this->parent_mat->data;
            } else {
                delete[] this->data;
                this->data = nullptr;
                delete this->refCount;
                this->refCount = nullptr;
            }
        } else {
            (*(this->refCount))--;
        }
    }
    this->rows = mat.rows;
    this->cols = mat.cols;
    this->data = mat.data;
    this->parent_mat = mat.parent_mat;
    this->refCount = mat.refCount;
    (*(this->refCount))++;

    return *this;
}

template <typename T>
bool Matrices<T>::operator==(const Matrices<T>& mat)
{
    if (this->rows != mat.rows || this->cols != mat.cols) {
        return false;
    }
    if (this->parent_mat != mat.parent_mat) {
        return false;
    }
    if (std::equal(this->data, (this->data + (this->rows * this->cols)), mat.data, (mat.data + (mat.rows * mat.cols)))) {
        return true;
    }
    return false;
}

template <typename T>
Matrices<T> Matrices<T>::operator+(const Matrices<T>& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->rows != mat.rows || this->cols != mat.cols) {
        throw std::invalid_argument("Two mat is not match!");
    }
    if ((this->data == nullptr) && (mat.data == nullptr)) {
        throw std::invalid_argument("Two mat is null data!");
    }
    if (mat.data == nullptr) {
        return *this;
    }
    if (this->data == nullptr) {
        return mat;
    }

    Matrices<T> sum_mat(mat.rows, mat.cols);
    sum_mat.data = new T[sum_mat.rows * sum_mat.cols];
    sum_mat.refCount = new int;
    *(sum_mat.refCount) = 1;

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            sum_mat.data[temp_row + c] = this->data[temp_row + c] + mat.data[temp_row + c];
        }
    }
    return sum_mat;
}

template <typename T>
Matrices<T> Matrices<T>::operator+(T number)
{
    if (this->rows == 0 || this->cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("matrice data is NULL!");
    }
    if (number == 0) {
        return *this;
    }

    Matrices<T> result_mat(this->rows, this->cols);
    result_mat.data = new T[result_mat.rows * result_mat.cols];
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            result_mat.data[temp_row + c] = this->data[temp_row + c] + number;
        }
    }

    return result_mat;
}

template <typename T>
Matrices<T>& Matrices<T>::operator+=(const Matrices<T>& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->rows != mat.rows || this->cols != mat.cols) {
        throw std::invalid_argument("Two mat is not match!");
    }
    if ((this->data == nullptr) && (mat.data == nullptr)) {
        throw std::invalid_argument("Two mat is null data!");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("null mat can not use +=!");
    }
    if (mat.data == nullptr) {
        return *this;
    }

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            this->data[temp_row + c] += mat.data[temp_row + c];
        }
    }
    return *this;
}

template <typename T>
Matrices<T>& Matrices<T>::operator+=(T number)
{
    if (this->rows == 0 || this->cols == 0) {
        throw std::invalid_argument("mat rows or cols  is zero");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("null mat can not use +=!");
    }
    if (number == 0) {
        return *this;
    }

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            this->data[temp_row + c] += number;
        }
    }

    return *this;
}

template <typename T>
Matrices<T> Matrices<T>::operator-(const Matrices<T>& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->rows != mat.rows || this->cols != mat.cols) {
        throw std::invalid_argument("Two mat is not match!");
    }
    if ((this->data == nullptr) && (mat.data == nullptr)) {
        throw std::invalid_argument("Two mat is null data!");
    }
    if (mat.data == nullptr) {
        return *this;
    }
    if (this->data == nullptr) {

        for (size_t r = 0; r < (this->rows); r++) {
            size_t temp_row = r * this->cols;
            for (size_t c = 0; c < (this->cols); c++) {
                mat.data[temp_row + c] = -mat.data[temp_row + c];
            }
        }

        return mat;
    }

    Matrices<T> result_mat(mat.rows, mat.cols);
    result_mat.data = new T[result_mat.rows * result_mat.cols];
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            result_mat.data[temp_row + c] = this->data[temp_row + c] - mat.data[temp_row + c];
        }
    }

    return result_mat;
}

template <typename T>
Matrices<T> Matrices<T>::operator-(T number)
{
    if (this->rows == 0 || this->cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("matrice data is NULL!");
    }
    if (number == 0) {
        return *this;
    }

    Matrices<T> result_mat(this->rows, this->cols);
    result_mat.data = new T[result_mat.rows * result_mat.cols];
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            result_mat.data[temp_row + c] = this->data[temp_row + c] - number;
        }
    }

    return result_mat;
}

template <typename T>
Matrices<T>& Matrices<T>::operator-=(const Matrices<T>& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols  is zero");
    }
    if (this->rows != mat.rows || this->cols != mat.cols) {
        throw std::invalid_argument("Two mat is not match!");
    }
    if ((this->data == nullptr) && (mat.data == nullptr)) {
        throw std::invalid_argument("Two mat is null data!");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("null mat can not use +=!");
    }
    if (mat.data == nullptr) {
        return *this;
    }

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            this->data[temp_row + c] -= mat.data[temp_row + c];
        }
    }

    return *this;
}

template <typename T>
Matrices<T>& Matrices<T>::operator-=(T number)
{
    if (this->rows == 0 || this->cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("null mat can not use +=!");
    }
    if (number == 0) {
        return *this;
    }

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            this->data[temp_row + c] -= number;
        }
    }

    return *this;
}

template <typename T>
Matrices<T> Matrices<T>::operator*(const Matrices<T>& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->rows != mat.rows || this->cols != mat.cols) {
        throw std::invalid_argument("Two mat is not match!");
    }
    if ((this->data == nullptr) && (mat.data == nullptr)) {
        throw std::invalid_argument("Two mat is null data!");
    }
    if (mat.data == nullptr) {
        throw std::invalid_argument("right mat is null data!");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("left mat is null data!");
    }

    Matrices<T> result_mat(mat.rows, mat.cols);
    result_mat.data = new T[result_mat.rows * result_mat.cols];
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            result_mat.data[temp_row + c] = this->data[temp_row + c] * mat.data[temp_row + c];
        }
    }

    return result_mat;
}

template <typename T>
Matrices<T> Matrices<T>::operator*(T number)
{
    if (this->rows == 0 || this->cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("matrice data is NULL!");
    }

    Matrices<T> result_mat(this->rows, this->cols);
    result_mat.data = new T[result_mat.rows * result_mat.cols];
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            result_mat.data[temp_row + c] = this->data[temp_row + c] * number;
        }
    }

    return result_mat;
}

template <typename T>
Matrices<T>& Matrices<T>::operator*=(const Matrices<T>& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->rows != mat.rows || this->cols != mat.cols) {
        throw std::invalid_argument("Two mat is not match!");
    }
    if ((this->data == nullptr) && (mat.data == nullptr)) {
        throw std::invalid_argument("Two mat is null data!");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("null mat can not use +=!");
    }
    if (mat.data == nullptr) {
        throw std::invalid_argument("right mat data is NULL!");
    }

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            this->data[temp_row + c] *= mat.data[temp_row + c];
        }
    }

    return *this;
}

template <typename T>
Matrices<T>& Matrices<T>::operator*=(T number)
{
    if (this->rows == 0 || this->cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("null mat can not use +=!");
    }

    for (size_t r = 0; r < (this->rows); r++) {
        size_t temp_row = r * this->cols;
        for (size_t c = 0; c < (this->cols); c++) {
            this->data[temp_row + c] *= number;
        }
    }

    return *this;
}

template <typename T>
Matrices<T> Matrices<T>::operator^(const Matrices& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("left mat data is NULL!");
    }
    if (mat.data == nullptr) {
        throw std::invalid_argument("right mat data is NULL!");
    }

    if (this->cols != mat.rows) {
        throw std::invalid_argument("two mat is not match!");
    }

    Matrices<T> result_mat(this->rows, mat.cols);
    result_mat.data = new T[result_mat.rows * result_mat.cols] { 0 };
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t rr = 0; rr < result_mat.rows; rr++) {
        size_t temp_rows_this = rr * this->cols;
        size_t temp_rows_result = rr * result_mat.cols;
        for (size_t rc = 0; rc < result_mat.cols; rc++) {
            size_t temp_index_result = temp_rows_result + rc;
            for (size_t mid = 0; mid < this->cols; mid++) {
                result_mat.data[temp_index_result] += (this->data[temp_rows_this + mid] * mat.data[rc + mid * mat.cols]);
            }
        }
    }

    return result_mat;
}

template <typename T>
T& Matrices<T>::operator()(int row, int col)
{
    if (row >= this->rows || col >= this->cols) {
        throw std::invalid_argument("row or col out of range!");
    }
    if (this->data == nullptr) {
        throw std::invalid_argument("this mat data is NULL!");
    }
    return this->data[row * this->cols + col];
}

// ********************* Friend Function ******************************//
template <typename Y>
Matrices<Y> operator+(Y number, const Matrices<Y>& mat)
{
    if (mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (number == 0) {
        return mat;
    }
    if (mat.data == nullptr) {
        throw std::invalid_argument("matrice data is NULL!");
    }

    Matrices<Y> result_mat(mat.rows, mat.cols);
    result_mat.data = new Y[result_mat.rows * result_mat.cols];
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t r = 0; r < (mat.rows); r++) {
        size_t temp_row = r * mat.cols;
        for (size_t c = 0; c < (mat.cols); c++) {
            result_mat.data[temp_row + c] = number + mat.data[temp_row + c];
        }
    }

    return result_mat;
}

template <typename Y>
Matrices<Y> operator-(Y number, const Matrices<Y>& mat)
{
    if (mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (mat.data == nullptr) {
        throw std::invalid_argument("matrice data is NULL!");
    }

    Matrices<Y> result_mat(mat.rows, mat.cols);
    result_mat.data = new Y[result_mat.rows * result_mat.cols];
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t r = 0; r < (mat.rows); r++) {
        size_t temp_row = r * mat.cols;
        for (size_t c = 0; c < (mat.cols); c++) {
            result_mat.data[temp_row + c] = number - mat.data[temp_row + c];
        }
    }

    return result_mat;
}

template <typename Y>
Matrices<Y> operator*(Y number, const Matrices<Y>& mat)
{
    if (mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols  is zero");
    }
    if (mat.data == nullptr) {
        throw std::invalid_argument("matrice data is NULL!");
    }

    Matrices<Y> result_mat(mat.rows, mat.cols);
    result_mat.data = new Y[result_mat.rows * result_mat.cols];
    result_mat.refCount = new int;
    *(result_mat.refCount) = 1;

    for (size_t r = 0; r < (mat.rows); r++) {
        size_t temp_row = r * mat.cols;
        for (size_t c = 0; c < (mat.cols); c++) {
            result_mat.data[temp_row + c] = number * mat.data[temp_row + c];
        }
    }

    return result_mat;
}

template <typename Y>
std::ostream& operator<<(std::ostream& os, const Matrices<Y>& mat)
{
    if (mat.data == nullptr) {
        throw std::runtime_error("print matrice or matrice data is NULL!");
    }

    os << "[" << '\n';
    if (mat.parent_mat == nullptr) {
        for (size_t r = 0; r < (mat.rows); r++) {
            size_t temp_row = r * mat.cols;
            for (size_t c = 0; c < (mat.cols); c++) {
                os << setprecision(8) << mat.data[temp_row + c] << " ";
            }
            if (r == (mat.rows - 1)) {
                os << '\n'
                   << "]" << '\n';

            } else {
                os << '\n';
            }
        }
    } else {
        for (size_t r = 0; r < (mat.rows); r++) {
            size_t temp_row = r * mat.parent_mat->cols;
            for (size_t c = 0; c < (mat.cols); c++) {
                os << setprecision(8) << mat.data[temp_row + c] << " ";
            }
            if (r == (mat.rows - 1)) {
                os << '\n'
                   << "]" << '\n';

            } else {
                os << '\n';
            }
        }
    }

    os << '\n';
    return os;
}

// ********************* Member Function ******************************//

template <typename T>
void Matrices<T>::print_mat() const
{
    if (this->data == nullptr) {
        throw std::runtime_error("print matrice or matrice data is NULL!");
    }

    cout << "[" << '\n';
    if (this->parent_mat == nullptr) {
        for (size_t r = 0; r < (this->rows); r++) {
            size_t temp_row = r * this->cols;
            for (size_t c = 0; c < (this->cols); c++) {
                cout << setprecision(8) << this->data[temp_row + c] << " ";
            }
            if (r == (this->rows - 1)) {
                cout << '\n'
                     << "]" << '\n';

            } else {
                cout << '\n';
            }
        }
    } else {
        for (size_t r = 0; r < (this->rows); r++) {
            size_t temp_row = r * this->parent_mat->cols;
            for (size_t c = 0; c < (this->cols); c++) {
                cout << setprecision(8) << this->data[temp_row + c] << " ";
            }
            if (r == (this->rows - 1)) {
                cout << '\n'
                     << "]" << '\n';

            } else {
                cout << '\n';
            }
        }
    }

    cout << '\n';
}

template <typename T>
void Matrices<T>::print_shape() const
{
    cout << "rows: " << this->rows
         << ", cols: " << this->cols << '\n';
}

template <typename T>
size_t Matrices<T>::get_rows()
{
    return this->rows;
}
template <typename T>
size_t Matrices<T>::get_cols()
{
    return this->cols;
}
template <typename T>
int Matrices<T>::get_refCount()
{
    return *(this->refCount);
}

template <typename T>
void Matrices<T>::transpose()
{
    if (this->data == nullptr) {
        throw invalid_argument("mat data is NULL!");
    }

    Matrices<T>* temp_mat = new Matrices<T>(this->cols, this->rows);
    temp_mat->data = new T[temp_mat->rows * temp_mat->cols];
    temp_mat->refCount = new int;
    *(temp_mat->refCount) = 2;

    for (int row = 0; row < this->rows; row++) {
        size_t temp_rows = row * this->cols;
        for (int col = 0; col < this->cols; col++) {
            temp_mat->data[col * temp_mat->cols + row] = this->data[temp_rows + col];
        }
    }

    if (*(this->refCount) == 1) {
        delete[] this->data;
        this->data = nullptr;
        delete this->refCount;
        this->refCount = nullptr;
    } else {
        (*(this->refCount))--;
    }
    this->rows = temp_mat->rows;
    this->cols = temp_mat->cols;
    this->data = temp_mat->data;
    this->refCount = temp_mat->refCount;
    delete temp_mat;
}

template <typename T>
Matrices<T> Matrices<T>::clone(const Matrices<T>& mat)
{
    if (this->rows == 0 || this->cols == 0 || mat.rows == 0 || mat.cols == 0) {
        throw std::invalid_argument("mat rows or cols is zero");
    }
    if (mat.data == nullptr) {
        throw std::invalid_argument("The mat right of = is null data!");
    }
    if (this == &mat) {
        return *this;
    }
    if (this->data) {
        if ((*(this->refCount)) == 1) {
            if (this->parent_mat) {
                delete[] this->parent_mat->data;
            } else {
                delete[] this->data;
                this->data = nullptr;
                delete this->refCount;
                this->refCount = nullptr;
            }
        } else {
            (*(this->refCount))--;
        }
    }

    this->parent_mat = nullptr;
    this->rows = mat.rows;
    this->cols = mat.cols;
    this->data = new T[mat.rows * mat.cols];
    std::copy(mat.data, (mat.data + (mat.rows * mat.cols)), this->data);
    this->refCount = new int;
    (*(this->refCount)) = 1;

    return *this;
}

template <typename T>
Matrices<T> Matrices<T>::ROI(size_t row_start, size_t row_end, size_t col_start, size_t col_end)
{
    if (this->data == nullptr) {
        throw std::invalid_argument("You should enter the row and column ranges from small to large!");
    }
    if (row_start > row_end || col_start > col_end) {
        throw std::invalid_argument("end of row/col should be larger than start of row/col");
    }
    if (row_end < 0 || col_end < 0) {
        throw std::invalid_argument("row or coll must be positive!");
    }
    if (row_end >= this->rows || col_end >= this->cols) {
        throw std::invalid_argument("row or col is out of range!");
    }

    size_t rows = row_end - row_start + 1;
    size_t cols = col_end - col_start + 1;
    Matrices<T> ROI_mat(rows, cols);

    if (this->parent_mat == nullptr) {
        ROI_mat.data = this->data + row_start * this->cols + col_start;
        ROI_mat.parent_mat = this;
    } else {
        ROI_mat.data = this->data + row_start * this->parent_mat->cols + col_start;
        ROI_mat.parent_mat = this->parent_mat;
    }

    ROI_mat.refCount = this->refCount;
    (*(this->refCount))++;

    return ROI_mat;
}

template <typename T>
void Matrices<T>::ROI_adjust(size_t row_start, size_t row_end, size_t col_start, size_t col_end)
{
    if (this->data == nullptr) {
        throw std::invalid_argument("You should enter the row and column ranges from small to large!");
    }
    if (this->parent_mat == nullptr) {
        throw std::invalid_argument("This is not a ROI mat, please use the function ROI()");
    }
    if (row_start > row_end || col_start > col_end) {
        throw std::invalid_argument("You should ");
    }
    if (row_end < 0 || col_end < 0) {
        throw std::invalid_argument("row or coll must be positive!");
    }
    if (row_end >= this->parent_mat->rows || col_end >= this->parent_mat->cols) {
        throw std::invalid_argument("row or col is out of range!");
    }

    size_t rows = row_end - row_start + 1;
    size_t cols = col_end - col_start + 1;

    this->rows = rows;
    this->cols = cols;
    this->data = this->parent_mat->data + row_start * this->parent_mat->cols + col_start;
}

// friend function instantiation
template Matrices<int> operator+(int number, const Matrices<int>& mat);
template Matrices<unsigned char> operator+(unsigned char number, const Matrices<unsigned char>& mat);
template Matrices<short> operator+(short number, const Matrices<short>& mat);
template Matrices<float> operator+(float number, const Matrices<float>& mat);
template Matrices<double> operator+(double number, const Matrices<double>& mat);

template Matrices<int> operator-(int number, const Matrices<int>& mat);
template Matrices<unsigned char> operator-(unsigned char number, const Matrices<unsigned char>& mat);
template Matrices<short> operator-(short number, const Matrices<short>& mat);
template Matrices<float> operator-(float number, const Matrices<float>& mat);
template Matrices<double> operator-(double number, const Matrices<double>& mat);

template Matrices<int> operator*(int number, const Matrices<int>& mat);
template Matrices<unsigned char> operator*(unsigned char number, const Matrices<unsigned char>& mat);
template Matrices<short> operator*(short number, const Matrices<short>& mat);
template Matrices<float> operator*(float number, const Matrices<float>& mat);
template Matrices<double> operator*(double number, const Matrices<double>& mat);

template std::ostream& operator<<(std::ostream& os, const Matrices<int>& mat);
template std::ostream& operator<<(std::ostream& os, const Matrices<unsigned char>& mat);
template std::ostream& operator<<(std::ostream& os, const Matrices<short>& mat);
template std::ostream& operator<<(std::ostream& os, const Matrices<float>& mat);
template std::ostream& operator<<(std::ostream& os, const Matrices<double>& mat);

// Matrices instantiation
template class Matrices<unsigned char>;
template class Matrices<short>;
template class Matrices<int>;
template class Matrices<float>;
template class Matrices<double>;