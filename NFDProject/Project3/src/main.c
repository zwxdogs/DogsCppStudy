#include "martix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    clock_t start, end;

    char path_1024_A[64] = "/home/zwxdogs/Cpp/Project3/data/matA_1024.txt";
    char path_1024_B[64] = "/home/zwxdogs/Cpp/Project3/data/matB_1024.txt";
    Martix* mat_1024_A = create(1024, 1024, path_1024_A);
    Martix* mat_1024_B = create(1024, 1024, path_1024_B);
    Martix* mat_mul_result = create_no_data(1024, 1024);

    start = clock();
    mul(mat_1024_A, mat_1024_B, mat_mul_result);
    end = clock();
    printf("mul use time: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    release(mat_1024_A);
    release(mat_1024_B);
    release(mat_mul_result);

    return 0;
}
