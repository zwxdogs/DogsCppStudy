#include "martix.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1;

const int PATH_LEN = 64;

void print_mat(const Martix* mat)
{
    if (mat == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return;
    } else if (mat->data == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): mat's data is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return;
    }

    printf("\n");
    for (int i = 0; i < (mat->rows); i++) {
        for (int j = 0; j < (mat->cols); j++) {
            printf("%f ", mat->data[i * mat->cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

Martix* create_no_data(size_t rows, size_t cols)
{
    // create martix
    int* refcount_address = (int*)malloc(sizeof(int));
    float* data = NULL;
    data = (float*)malloc(rows * cols * sizeof(float));
    Martix* mat = NULL;
    mat = (Martix*)malloc(sizeof(Martix));

    mat->rows = rows;
    mat->cols = cols;
    mat->data = data;
    mat->refcount = refcount_address;
    *(mat->refcount) = 1;

    return mat;
}

Martix* create(size_t rows, size_t cols, char path[PATH_LEN])
{
    if (rows == 0 || cols == 0) {
        fprintf(stderr, "File %s, Line %d, Function %s(): rows or cols is zero!\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }
    if (path == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): path is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    // read data
    float* data = NULL;
    data = (float*)malloc(rows * cols * sizeof(float));

    FILE* mat_read = NULL;
    mat_read = fopen(path, "r");
    if (mat_read == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): Open failed!\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    for (int i = 0; i < (rows * cols); i++) {
        fscanf(mat_read, "%f", &data[i]);
    }
    fclose(mat_read);

    // create martix
    int* refcount_address = (int*)malloc(sizeof(int));
    Martix* mat = NULL;
    mat = (Martix*)malloc(sizeof(Martix));

    mat->rows = rows;
    mat->cols = cols;
    mat->data = data;
    mat->refcount = refcount_address;
    *(mat->refcount) = 1;

    return mat;
}

bool release(Martix* mat)
{
    if (mat == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }
    if (mat->data) {
        if (*(mat->refcount) == 1) {
            free(mat->data);
            free(mat->refcount);
        } else {
            (*(mat->refcount))--;
        }
    }

    free(mat);
    return true;
}

bool hard_copy(const Martix* mat_ori, Martix* mat_tar)
{
    if (mat_ori == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): origin mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    } else if (mat_ori->data == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): origin mat's data is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }
    if (mat_tar == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): target mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }

    memcpy(mat_tar->data, mat_ori->data, mat_ori->rows * mat_ori->cols * sizeof(float));

    return true;
}

bool soft_copy(const Martix* mat_ori, Martix* mat_tar)
{
    if (mat_ori == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): origin mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    } else if (mat_ori->data == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): origin mat's data is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }
    if (mat_tar == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): target mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }

    mat_tar->data = mat_ori->data;
    mat_tar->refcount = mat_ori->refcount;
    (*(mat_tar->refcount))++;

    return true;
}

bool mul(const Martix* mat_A, const Martix* mat_B, Martix* result)
{
    if (mat_A == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): 1st mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    } else if (mat_A->data == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): 1st mat's data is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }
    if (mat_B == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): 2nd mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    } else if (mat_B->data == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): 2nd mat's data is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }
    if (result == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): result mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    } else if (result->data == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): result mat's data is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }

    if (mat_A->cols != mat_B->rows) {
        fprintf(stderr, "File %s, Line %d, Function %s(): cols and rows is not mapped!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    } else if (mat_A->rows == 0 || mat_A->cols == 0 || mat_B->rows == 0) {
        fprintf(stderr, "File %s, Line %d, Function %s(): mat rows or cols is zero!\n", __FILE__, __LINE__, __FUNCTION__);
        return false;
    }

    for (int i = 0; i < (result->rows); i++) {
        size_t rows_now = i * result->cols;
        size_t A_rows_now = i * mat_A->cols;
        for (int j = 0; j < (result->cols); j++) {
            size_t cal_result_now = rows_now + j;
            for (int k = 0; k < (mat_A->cols); k++) {
                result->data[cal_result_now] += mat_A->data[A_rows_now + k] * mat_B->data[j + k * mat_B->cols];
            }
        }
    }

    return true;
}

void write(const Martix* mat, char path[64])
{
    if (mat == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): mat is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return;
    } else if (mat->data == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): mat's data is NULL!\n", __FILE__, __LINE__, __FUNCTION__);
        return;
    }

    FILE* mat_write = NULL;
    mat_write = fopen(path, "w");
    if (mat_write == NULL) {
        fprintf(stderr, "File %s, Line %d, Function %s(): open file failed!\n", __FILE__, __LINE__, __FUNCTION__);
        return;
    }
    for (int i = 0; i < (mat->rows * mat->cols); i++) {
        if (i % mat->cols == 0) {
            fprintf(mat_write, "\n");
        }
        fprintf(mat_write, "%f ", mat->data[i]);
    }

    fclose(mat_write);
}