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

SCENARIO("Span construction of Fixed-size Matrx") {
    WHEN("A Matrix of compile-time size is constructed from a span") {
        std::vector<int> source = {1, 2, 3, 4, 5, 6,};
        std::span<int> view(source);
        Matrix<int, 2, 3> matrix(view);
        THEN("The Matrix contents are the same as those of the span") {
            // collect Matrix contents into a vector
            auto mc = matrix.contents();
            std::vector<int> contents(mc.begin(), mc.end());
            CHECK(contents == source);
        }
    }
}

SCENARIO("Span construction of Dynamic-size Matrx") {
    WHEN("A Matrix of run-time size is constructed from dimensions and a span") {
        std::vector<int> source = {1, 2, 3, 4, 5, 6,};
        std::span<int> view(source);
        Matrix<int> matrix(2, 3, view);
        THEN("The Matrix has those dimensions") {
            CHECK(matrix.row_count() == 2);
            CHECK(matrix.col_count() == 3);
        }
        AND_THEN("The Matrix contents are the same as those of the span") {
            // collect Matrix contents into a vector
            auto mc = matrix.contents();
            std::vector<int> contents(mc.begin(), mc.end());
            CHECK(contents == source);
        }
    }
}

SCENARIO("Construct fixed-size Matrix from dynamic-size Matrix") {
    GIVEN("A Matrix of dynamic size, with non-zero dimensions and contents") {
        Matrix<int> dynamic(3, 2, {{1, 2,}, {3, 4,}, {1, 9,},});
        WHEN("A Matrix of fixed size is constructed from the dynamic one") {
            Matrix<int, 3, 2> fixed(dynamic);
            THEN("The contents of both matrices are identical") {
                // collect contents of both for comparison
                auto dc = dynamic.contents();
                auto fc = fixed.contents();
                std::vector<int> dynamic_contents(dc.begin(), dc.end());
                std::vector<int> fixed_contents(fc.begin(), fc.end());
                CHECK(fixed_contents == dynamic_contents);
            }
        }
    }
}

SCENARIO("Construct dynamic-size Matrix from fixed-size Matrix") {
    GIVEN("A Matrix of fixed size, with non-zero dimensions and contents") {
        Matrix<int, 3, 2> fixed = {
            {1, 2,},
            {3, 4,},
            {1, 9,},
        };
        WHEN("A Matrix of dynamic size is constructed from the fixed one") {
            Matrix<int> dynamic(fixed);
            THEN("The dimensions of both matrices match") {
                CHECK(dynamic.row_count() == fixed.row_count());
                CHECK(dynamic.col_count() == fixed.col_count());
            }
            AND_THEN("The contents of both matrices are identical") {
                // collect contents of both for comparison
                auto fc = fixed.contents();
                auto dc = dynamic.contents();
                std::vector<int> fixed_contents(fc.begin(), fc.end());
                std::vector<int> dynamic_contents(dc.begin(), dc.end());
                CHECK(dynamic_contents == fixed_contents);
            }
        }
    }
}
