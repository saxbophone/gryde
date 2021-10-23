#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>


SCENARIO("Calculating determinant of fixed-size Matrix") {
    GIVEN("An empty fixed-size Matrix") {
        Matrix<int, 0, 0> empty;
        THEN("Matrix.determinant() is calculated as 1") {
            CHECK(empty.determinant() == 1);
        }
    }
    GIVEN("A fixed-size Matrix of one element") {
        Matrix<int, 1, 1> one = {{43,},};
        THEN("Matrix.determinant() is calculated as the value of its sole element") {
            CHECK(one.determinant() == 43);
        }
    }
    GIVEN("A square fixed-size Matrix with some contents") {
        Matrix<int, 3, 3> matrix = {
            {1, 3, 7,},
            {9, 8, 2,},
            {3, 4, 13,},
        };
        THEN("Matrix.determinant() returns the determinant of the Matrix") {
            CHECK(matrix.determinant() == -153);
        }
    }
}

SCENARIO("Calculating determinant of non-square dynamic-size Matrix") {
    GIVEN("A non-square dynamic-size Matrix") {
        Matrix<int> rectangular(43, 17);
        THEN("Matrix.determinant() throws an exception") {
            CHECK_THROWS(rectangular.determinant());
        }
    }
}

SCENARIO("Calculating determinant of square dynamic-size Matrix") {
    GIVEN("An empty dynamic-size Matrix") {
        Matrix<int> empty(0, 0);
        THEN("Matrix.determinant() is calculated as 1") {
            CHECK(empty.determinant() == 1);
        }
    }
    GIVEN("A dynamic-size Matrix of one element") {
        Matrix<int> one(1, 1, {{43,},});
        THEN("Matrix.determinant() is calculated as the value of its sole element") {
            CHECK(one.determinant() == 43);
        }
    }
    GIVEN("A square dynamic-size Matrix with some contents") {
        Matrix<int> matrix(
            3, 3,
            {
                {1, 3, 7,},
                {9, 8, 2,},
                {3, 4, 13,},
            }
        );
        THEN("Matrix.determinant() returns the determinant of the Matrix") {
            CHECK(matrix.determinant() == -153);
        }
    }
}
