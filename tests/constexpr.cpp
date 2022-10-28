#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>

TEST_CASE("constexpr default initialisation") {
    constexpr Matrix<int, 3, 2> matrix;
    SUCCEED();
}

TEST_CASE("constexpr list initialisation") {
    constexpr Matrix<int, 4, 2> matrix = {
        {9, 1,},
        {7, 3,},
        {2, 1,},
        {8, 2,},
    };
    SUCCEED();
}

TEST_CASE("constexpr copy initialisation") {
    constexpr Matrix<int, 9, 7> a;
    constexpr Matrix<int, 9, 7> b = a;
    SUCCEED();
}

TEST_CASE("constexpr get dimensions") {
    constexpr Matrix<int, 4, 5> matrix;
    STATIC_REQUIRE(matrix.row_count() == 4);
    STATIC_REQUIRE(matrix.col_count() == 5);
}

TEST_CASE("constexpr equality comparison") {
    constexpr Matrix<int, 2, 3> a = {
        {1, 3, 7,},
        {5, 9, 4,},
    };
    constexpr Matrix<int, 2, 3> b = {
        {1, 3, 7,},
        {5, 9, 4,},
    };
    STATIC_REQUIRE(a == b);
    STATIC_REQUIRE(not (a != b));
}

TEST_CASE("constexpr get contents") {
    constexpr Matrix<int, 4, 2> matrix = {
        {9, 1,},
        {7, 3,},
        {2, 1,},
        {8, 2,},
    };
    // ??? --assigning the result to a constexpr is an error, but static check is fine?
    STATIC_REQUIRE(matrix.contents().size() == 8);
}

// MSVC has a bug where a virtual constexpr function-call operator (user here to
// access Matrix cell contents) is erroneously claimed to not be a constant
// expression.
// Bug report:
// https://developercommunity.visualstudio.com/t/MSVC-erroneously-claims-the-return-value/10184063
#ifndef _MSC_VER
TEST_CASE("constexpr cell access") {
    constexpr Matrix<int, 4, 2> matrix = {
        {9, 1,},
        {7, 3,},
        {2, 1,},
        {8, 2,},
    };
    constexpr int cell = matrix(1, 1);
    CHECK(cell == 3);
}

TEST_CASE("constexpr determinant") {
    constexpr Matrix<int, 3, 3> matrix = {
        {1, 3, 7,},
        {5, 9, 4,},
        {5, 6, 4,},
    };
    constexpr int determinant = matrix.determinant();
    CHECK(determinant == -93);
}
#endif
