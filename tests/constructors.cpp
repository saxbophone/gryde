#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>


using namespace com::saxbophone::gryde;

SCENARIO("Default-construction of Fixed-size Matrix") {
    WHEN("A Matrix of compile-time size is default-constructed") {
        constexpr Matrix<int, 3, 4> matrix;
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
        constexpr Matrix<int, 3, 4> matrix = {
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
    WHEN("A Matrix of compile-time size is constructed from an Initializer-list of incorrect size") {
        THEN("An exception is raised") {
            CHECK_THROWS(Matrix<int, 9, 7>({{}}));
        }
    }
    WHEN("A Matrix of compile-time size is constructed from an Initializer-list with an oversized row") {
        THEN("An exception is raised") {
            CHECK_THROWS(Matrix<int, 3, 3>({{1, 2, 3, 4,}, {}, {},}));
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
    WHEN("A Matrix of run-time size is constructed from an Initializer-list of incorrect size") {
        THEN("An exception is raised") {
            CHECK_THROWS(Matrix<int>(9, 7, {{}}));
        }
    }
    WHEN("A Matrix of run-time size is constructed from an Initializer-list with an oversized row") {
        THEN("An exception is raised") {
            CHECK_THROWS(Matrix<int>(3, 3, {{1, 2, 3, 4,}, {}, {},}));
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
    WHEN("A Matrix of compile-time size is constructed from a span of incorrect size") {
        THEN("An exception is raised") {
            CHECK_THROWS(Matrix<int, 3, 3>(std::span<int>()));
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
    WHEN("A Matrix of run-time size is constructed from a span of incorrect size") {
        THEN("An exception is raised") {
            CHECK_THROWS(Matrix<int>(3, 3, std::span<int>()));
        }
    }
}

SCENARIO("Copy-construction of fixed-size Matrix") {
    GIVEN("A Matrix of fixed-size") {
        Matrix<int, 3, 2> a = {
            {1, 2,},
            {4, -7,},
            {6, 3,},
        };
        WHEN("Another Matrix of fixed-size is constructed from it") {
            Matrix<int, 3, 2> b = a;
            THEN("The contents of both matrices are identical") {
                // collect contents of both for comparison
                auto ac = a.contents();
                auto bc = b.contents();
                std::vector<int> a_contents(ac.begin(), ac.end());
                std::vector<int> b_contents(bc.begin(), bc.end());
                CHECK(a_contents == b_contents);
            }
        }
    }
}

SCENARIO("Copy-construction of dynamic-size Matrix") {
    GIVEN("A Matrix of dynamic-size") {
        Matrix<int> a(
            3, 2,
            {
                {1, 2,},
                {4, -7,},
                {6, 3,},
            }
        );
        WHEN("Another Matrix of dynamic-size is constructed from it") {
            Matrix<int> b = a;
            THEN("The dimensions of both matrices are identical") {
                CHECK(a.row_count() == b.row_count());
                CHECK(a.col_count() == b.col_count());
            }
            AND_THEN("The contents of both matrices are identical") {
                // collect contents of both for comparison
                auto ac = a.contents();
                auto bc = b.contents();
                std::vector<int> a_contents(ac.begin(), ac.end());
                std::vector<int> b_contents(bc.begin(), bc.end());
                CHECK(a_contents == b_contents);
            }
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
        WHEN("A Matrix of fixed size and different dimensions is constructed from the dynamic one") {
            THEN("An exception is raised") {
                CHECK_THROWS(Matrix<int, 1, 1>(dynamic));
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

SCENARIO("Copy-assignment of fixed-size Matrix") {
    GIVEN("A fixed-size Matrix") {
        Matrix<int, 3, 2> matrix;
        WHEN("The Matrix is assigned to with another fixed-size Matrix") {
            std::vector<int> source = {1, 2, 3, 4, 5, 6,};
            matrix = Matrix<int, 3, 2>(std::span<int>(source));
            THEN("The Matrix's contents are the same as those of the Matrix assigned to it") {
                // collect Matrix contents into a vector
                auto mc = matrix.contents();
                std::vector<int> contents(mc.begin(), mc.end());
                CHECK(contents == source);
            }
        }
    }
}

SCENARIO("Copy-assignment of dynamic-size Matrix") {
    GIVEN("A dynamic-size Matrix") {
        Matrix<int> matrix(3, 2);
        WHEN("The Matrix is assigned to with another dynamic-size Matrix") {
            std::vector<int> source = {1, 2, 3, 4, 5, 6,};
            matrix = Matrix<int>(3, 2, std::span<int>(source));
            THEN("The dimensions of the Matrix are correct") {
                CHECK(matrix.row_count() == 3);
                CHECK(matrix.col_count() == 2);
            }
            THEN("The Matrix's contents are the same as those of the Matrix assigned to it") {
                // collect Matrix contents into a vector
                auto mc = matrix.contents();
                std::vector<int> contents(mc.begin(), mc.end());
                CHECK(contents == source);
            }
        }
        WHEN("The Matrix is assigned to with another dynamic-size Matrix of different dimensions") {
            std::vector<int> source = {1, 2, 3, 4,};
            matrix = Matrix<int>(2, 2, std::span<int>(source));
            THEN("The dimensions of the Matrix are correct") {
                CHECK(matrix.row_count() == 2);
                CHECK(matrix.col_count() == 2);
            }
            THEN("The Matrix's contents are the same as those of the Matrix assigned to it") {
                // collect Matrix contents into a vector
                auto mc = matrix.contents();
                std::vector<int> contents(mc.begin(), mc.end());
                CHECK(contents == source);
            }
        }
    }
}
