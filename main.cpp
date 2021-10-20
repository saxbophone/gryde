#include <iostream>

int main() {
    constexpr Matrix<int, 3, 3> a = {
        {1, 9, 6,},
        {3, 2, 7,},
        {8, 1, 0,},
    };
    constexpr auto b = a.submatrix(2, 2);
    for (auto item : b.contents()) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
}
