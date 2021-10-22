#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>


SCENARIO("Make submatrix of zero-size dynamic-size Matrix") {
    GIVEN("A dynamic-size Matrix of zero-size") {
        Matrix<int> matrix(0, 0);
        THEN("Matrix.submatrix() throws an exception") {
            CHECK_THROWS(matrix.submatrix(0, 0));
        }
    }
}

SCENARIO("Make submatrix of fixed-size Matrix with default values") {
    GIVEN("A fixed-size Matrix with default-initialised contents") {
        Matrix<int, 10, 9> matrix;
        THEN("Matrix.submatrix() returns a matrix with one less row and column with default content") {
            // with a zero-matrix, doesn't actually matter which args are passed to submatrix()
            Matrix<int, 9, 8> submatrix = matrix.submatrix(0, 0);
            std::vector<int> expected(9 * 8); // expected values
            // collect all values into a vector
            auto smc = submatrix.contents();
            std::vector<int> contents(smc.begin(), smc.end());
            CHECK(contents == expected);
        }
        THEN("Matrix.submatrix() throws an exception when the row or col indices are out of bounds") {
            CHECK_THROWS(matrix.submatrix(10, 8)); // row
            CHECK_THROWS(matrix.submatrix(9, 9)); // col
            CHECK_THROWS(matrix.submatrix(20, 20)); // both
        }
    }
}

SCENARIO("Make submatrix of dynamic-size Matrix with default values") {
    GIVEN("A dynamic-size Matrix with default-initialised contents") {
        Matrix<int> matrix(10, 9);
        THEN("Matrix.submatrix() returns a matrix with one less row and column with default content") {
            // with a zero-matrix, doesn't actually matter which args are passed to submatrix()
            Matrix<int> submatrix = matrix.submatrix(0, 0);
            // verify dimensions
            REQUIRE(submatrix.row_count() == 9);
            REQUIRE(submatrix.col_count() == 8);
            std::vector<int> expected(9 * 8); // expected values
            // collect all values into a vector
            auto smc = submatrix.contents();
            std::vector<int> contents(smc.begin(), smc.end());
            CHECK(contents == expected);
        }
        THEN("Matrix.submatrix() throws an exception when the row or col indices are out of bounds") {
            CHECK_THROWS(matrix.submatrix(10, 8)); // row
            CHECK_THROWS(matrix.submatrix(9, 9)); // col
            CHECK_THROWS(matrix.submatrix(20, 20)); // both
        }
    }
}

SCENARIO("Make submatrix of fixed-size Matrix") {
    GIVEN("A fixed-size Matrix with some contents") {
        Matrix<int, 3, 4> matrix = {
            {9, 4, 5, 5,},
            {7, 3, 5, 6,},
            {2, 8, 61, 3,},
        };
        WHEN("A submatrix of it is made") {
            Matrix<int, 2, 3> submatrix = matrix.submatrix(1, 2);
            THEN("The submatrix includes all of its contents except the excluded row and column") {
                std::vector<int> expected = {
                    9, 4, 5,
                    2, 8, 3,
                };
                // collect all values into a vector
                auto smc = submatrix.contents();
                std::vector<int> contents(smc.begin(), smc.end());
                CHECK(contents == expected);
            }
        }
    }
}

SCENARIO("Make submatrix of dynamic-size Matrix") {
    GIVEN("A dynamic-size Matrix with some contents") {
        Matrix<int> matrix(
            3, 4,
            {
                {9, 4, 5, 5,},
                {7, 3, 5, 6,},
                {2, 8, 61, 3,},
            }
        );
        WHEN("A submatrix of it is made") {
            Matrix<int> submatrix = matrix.submatrix(1, 2);
            THEN("The submatrix has one less row and column than it") {
                REQUIRE(submatrix.row_count() == 2);
                REQUIRE(submatrix.col_count() == 3);
            }
            AND_THEN("The submatrix includes all of its contents except the excluded row and column") {
                std::vector<int> expected = {
                    9, 4, 5,
                    2, 8, 3,
                };
                // collect all values into a vector
                auto smc = submatrix.contents();
                std::vector<int> contents(smc.begin(), smc.end());
                CHECK(contents == expected);
            }
        }
    }
}
