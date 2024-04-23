#include "Matrices.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

int main(int argc, char** argv)
{
    int* data = nullptr;
    data = new int[3 * 3] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int* data2 = nullptr;
    data2 = new int[3 * 3] { 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    float* data3 = nullptr;
    data3 = new float[3 * 3] {
        11.F, 12.F, 13.F,
        14.F, 15.F, 16.F,
        17.F, 18.F, 19.F
    };

    try {
        // Matrices<int> mat1(data, 3, 3);
        // Matrices<int> mat2(data2, 3, 3);
        // Matrices<float> mat3(data3, 3, 3);
        Matrices<float> mat4("/home/zwxdogs/Cpp/Project4/data/matA_32.txt", 32, 32);

        Matrices<float> mat5 = mat4.ROI(5, 7, 5, 8);
        Matrices<float> mat6 = mat5.ROI(1, 2, 1, 2);
        Matrices<float> mat7(mat5);
        mat6 = mat6.clone(mat7);

    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

    return 0;
}
