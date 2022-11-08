#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>


using namespace com::saxbophone::gryde;

SCENARIO("Get cell of zero-size fixed Matrix") {
    GIVEN("A fixed Matrix of zero-size") {
        Matrix<int, 0, 0> matrix;
        THEN("Using function-call operator to get any cell throws an exception") {
            CHECK_THROWS(matrix(0, 0));
        }
    }
}

SCENARIO("Get cell of zero-size dynamic Matrix") {
    GIVEN("A dynamic Matrix of zero-size") {
        Matrix<int> matrix(0, 0);
        THEN("Using function-call operator to get any cell throws an exception") {
            CHECK_THROWS(matrix(0, 0));
        }
    }
}

SCENARIO("Get cell of non-zero-size fixed Matrix") {
    GIVEN("A fixed Matrix of non-zero-size with some contents") {
        Matrix<int, 3, 2> matrix = {
            {1, 2,},
            {3, 4,},
            {6, 3,},
        };
        THEN("A single cell of the Matrix can be retrieved using the function-call operator") {
            int cell = matrix(1, 1);
            CHECK(cell == 4);
        }
        THEN("Using the function-call operator with out-of-bounds indices throws an exception") {
            CHECK_THROWS(matrix(2, 4)); // 4 out of bounds
            CHECK_THROWS(matrix(9, 1)); // 9 out of bounds
            CHECK_THROWS(matrix(2000, 1000)); // both out of bounds
        }
    }
}

SCENARIO("Get cell of non-zero-size dynamic Matrix") {
    GIVEN("A dynamic Matrix of non-zero-size with some contents") {
        Matrix<int> matrix(
            3, 2,
            {
                {1, 2,},
                {3, 4,},
                {6, 3,},
            }
        );
        THEN("A single cell of the Matrix can be retrieved using the function-call operator") {
            int cell = matrix(1, 1);
            CHECK(cell == 4);
        }
        THEN("Using the function-call operator with out-of-bounds indices throws an exception") {
            CHECK_THROWS(matrix(2, 4)); // 4 out of bounds
            CHECK_THROWS(matrix(9, 1)); // 9 out of bounds
            CHECK_THROWS(matrix(2000, 1000)); // both out of bounds
        }
    }
}

SCENARIO("Set cell of zero-size fixed Matrix") {
    GIVEN("A fixed Matrix of zero-size") {
        Matrix<int, 0, 0> matrix;
        THEN("Using function-call operator to set any cell throws an exception") {
            CHECK_THROWS(matrix(0, 0) = 9);
        }
    }
}

SCENARIO("Set cell of zero-size dynamic Matrix") {
    GIVEN("A dynamic Matrix of zero-size") {
        Matrix<int> matrix(0, 0);
        THEN("Using function-call operator to set any cell throws an exception") {
            CHECK_THROWS(matrix(0, 0) = 9);
        }
    }
}

SCENARIO("Set cell of non-zero-size fixed Matrix") {
    GIVEN("A fixed Matrix of non-zero-size with default-initialised content") {
        Matrix<int, 2, 3> matrix;
        // verify content in cell we're going to change is default-initialised
        REQUIRE(matrix(1, 2) == int{});
        WHEN("A single cell is set to a value using the function-call operator") {
            matrix(1, 2) = 467;
            THEN("That cell now has that value") {
                CHECK(matrix(1, 2) == 467);
            }
        }
        THEN("Trying to set an out of bounds cell throws an exception") {
            CHECK_THROWS(matrix(2, 4) = 23); // 4 out of bounds
            CHECK_THROWS(matrix(9, 1) = 19); // 9 out of bounds
            CHECK_THROWS(matrix(2000, 1000) = 7); // both out of bounds
        }
    }
}

SCENARIO("Set cells of non-zero-size dynamic Matrix") {
    GIVEN("A dynamic Matrix of non-zero-size") {
        Matrix<int> matrix(2, 3);
        // verify content in cell we're going to change is default-initialised
        REQUIRE(matrix(1, 2) == int{});
        WHEN("A single cell is set to a value using the function-call operator") {
            matrix(1, 2) = 467;
            THEN("That cell now has that value") {
                CHECK(matrix(1, 2) == 467);
            }
        }
        THEN("Trying to set an out of bounds cell throws an exception") {
            CHECK_THROWS(matrix(2, 4) = 23); // 4 out of bounds
            CHECK_THROWS(matrix(9, 1) = 19); // 9 out of bounds
            CHECK_THROWS(matrix(2000, 1000) = 7); // both out of bounds
        }
    }
}
