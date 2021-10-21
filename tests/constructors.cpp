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

SCENARIO("Initializer-list construction of Fixed-size Matrx") {
    WHEN("A Matrix of compile-time size is constructed from an Initializer-list") {
        Matrix<int, 3, 4> matrix = {
            {1, 2, 3, 4,},
            {-6, -7, 1, 0,},
            {2, 3, 2, 2,},
        };
        THEN("The Matrix contents are the same as those of the Initializer-list") {
            std::vector<int> expected = {
                1, 2, 3, 4,
                -6, -7, 1, 0,
                2, 3, 2, 2,
            };
            // collect Matrix contents into a vector
            auto mc = matrix.contents();
            std::vector<int> contents(mc.begin(), mc.end());
            CHECK(contents == expected);
        }
    }
}

SCENARIO("Construction of Dynamic-size Matrix with dimensions") {
    WHEN("A Matrix of run-time size is constructed with dimensions") {
        Matrix<int> matrix(3, 4);
        THEN("The Matrix has those dimensions") {
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

SCENARIO("Initializer-list construction of Dynamic-size Matrx") {
    WHEN("A Matrix of run-time size is constructed with dimensions and initializer-list") {
        Matrix<int> matrix(
            3, 4,
            {
                {1, 2, 3, 4,},
                {-6, -7, 1, 0,},
                {2, 3, 2, 2,},
            }
        );
        THEN("The Matrix has those dimensions") {
            CHECK(matrix.row_count() == 3);
            CHECK(matrix.col_count() == 4);
        }
        AND_THEN("The Matrix contents are the same as those of the Initializer-list") {
            std::vector<int> expected = {
                1, 2, 3, 4,
                -6, -7, 1, 0,
                2, 3, 2, 2,
            };
            // collect Matrix contents into a vector
            auto mc = matrix.contents();
            std::vector<int> contents(mc.begin(), mc.end());
            CHECK(contents == expected);
        }
    }
}
