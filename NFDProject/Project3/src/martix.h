#ifndef MARTIX
#define MARTIX

#include <stdbool.h>
#include <stdio.h>

typedef struct MARTIX_ {
    size_t rows;
    size_t cols;
    float* data;
    int* refcount;
} Martix;

void write(const Martix* mat, char path[64]);

Martix* create_no_data(size_t rows, size_t cols);

Martix* create(size_t rows, size_t cols, char path[64]);

bool release(Martix* mat);

bool hard_copy(const Martix* mat_ori, Martix* mat_tar);

bool soft_copy(const Martix* mat_ori, Martix* mat_tar);

bool mul(const Martix* mat_A, const Martix* mat_B, Martix* result);

void print_mat(const Martix* mat);

#endif