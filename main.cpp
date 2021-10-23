#include <iostream>

#include <gryde/Matrix.hpp>


int main() {
    // constexpr isn't working on Windows right now
    // needs more investigation into why this is
    // constexpr Matrix<int, 3, 3> a = {
    //     {1, 9, 6,},
    //     {3, 2, 7,},
    //     {8, 1, 0,},
    // };
    // constexpr auto b = a.submatrix(2, 2);
    // for (auto item : b.contents()) {
    //     std::cout << item << ", ";
    // }
    // std::cout << std::endl;
    // constexpr int determinant = a.determinant();
    // constexpr int b_determinant = b.determinant();
    // std::cout << determinant << " " << b_determinant << std::endl;
}
