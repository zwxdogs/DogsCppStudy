#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

float* read_mat(string& path, int row, int col)
{
    float* mat = new float[row * col] { 0.f };
    ifstream mat_data(path, ios::in);

    for (int i = 0; i < (row * col); i++) {
        mat_data >> mat[i];
    }
    mat_data.close();
    return mat;
}

float* mat_mul(const float* mat_A, const float* mat_B, int row_A, int col_A, int row_B, int col_B)
{
    int result_dimention = row_A * col_B;
    float* result = new float[result_dimention] { 0.f };
    for (int i = 0; i < row_A; i++) {
        for (int j = 0; j < col_B; j++) {
            for (int k = 0; k < col_A; k++) {
                result[i * col_B + j] += mat_A[i * col_A + k] * mat_B[j + col_B * k];
            }
        }
    }
    return result;
}

void print_mat(const float* mat, int row, int col)
{
    cout << "[";
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << mat[i * col + j] << " ";
        }
        if (i < (row - 1)) {
            cout << '\n';
        }
    }
    cout << "]" << '\n';
}

void out_result_txt(const float* mat, int row, int col, string path)
{
    ofstream out_result_txt;
    out_result_txt.open(path, ios::out);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            out_result_txt << mat[i * col + j] << " ";
        }
        out_result_txt << '\n';
    }
    out_result_txt.close();
}

int main(int argc, char** argv)
{
    int test_row = 1024;
    int test_col = 1024;

    string mat_path_A = "/home/zwxdogs/Cpp/Project2/data/matA_1024.txt";
    string mat_path_B = "/home/zwxdogs/Cpp/Project2/data/matB_1024.txt";
    float* mat_A = read_mat(mat_path_A, test_row, test_col);
    float* mat_B = read_mat(mat_path_B, test_row, test_col);

    auto start_time = chrono::system_clock::now();
    float* result = mat_mul(mat_A, mat_B, test_row, test_col, test_row, test_col);
    chrono::duration<double> pass_time = chrono::system_clock::now() - start_time;
    cout << "Time used: " << pass_time.count() << "s" << '\n';

    // print_mat(mat_A, test_row, test_col);

    string out_path = "/home/zwxdogs/Cpp/Project2/result/result_1024.txt";
    out_result_txt(result, test_row, test_col, out_path);
    delete[] mat_A;
    delete[] mat_B;
    delete[] result;

    return 0;
}
