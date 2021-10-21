#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>


SCENARIO("Default-construction of Fixed-size Matrix") {
    WHEN("A Matrix of compile-time size is default-constructed") {
        Matrix<int, 3, 4> matrix;
        THEN("The Matrix dimensions are equal to its compile-time size") {
            CHECK(matrix.row_count() == 3);
            CHECK(matrix.col_count() == 4);
        }
        AND_THEN("The Matrix contents are default-initialised") {
            std::vector<int> expected(3 * 4); // default values expected
            // collect Matrix contents into another vector
            auto mc = matrix.contents();
            std::vector<int> contents(mc.begin(), mc.end());
            CHECK(contents == expected);
        }
    }
}

SCENARIO("Default-construction of Dynamic-size Matrix") {
    WHEN("A Matrix of run-time size is default-constructed") {
        Matrix<int> matrix;
        THEN("The Matrix dimensions are 0x0") {
            CHECK(matrix.row_count() == 0);
            CHECK(matrix.col_count() == 0);
        }
        AND_THEN("The Matrix contents are empty") {
            CHECK(matrix.contents().size() == 0);
        }
    }
}
