#include "common.h"
#include "Matrix.h"

int main () {

    Matrix<int, 0> matrix;
    for(int idx = 0; idx < 10; ++idx) {
        matrix[idx][idx] = idx;
    }
    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }

    for(int idx = 9; idx > -1; --idx) {
        matrix[idx][idx] = idx;
    }
    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }

    for(int idx = 1; idx < 9; ++idx) {
        matrix[idx][idx] = idx;
    }

    std::cout << matrix.size() << std::endl;
    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }

    Matrix<int, 42> matrix2;
    assert(42 == matrix2[1000][1000]);
    matrix2[1][2] = 3; // здесь вставка в _arr
    matrix2[1][2] = 42; // а здесь удаление из _arr

   return 0;
}
