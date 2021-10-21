#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>


SCENARIO("Get contents of zero-size fixed Matrix") {
    GIVEN("A fixed Matrix of zero-size") {
        Matrix<int, 0, 0> matrix;
        THEN("Matrix.contents() returns a span of zero-size") {
            CHECK(matrix.contents().size() == 0);
        }
    }
}

SCENARIO("Get contents of zero-size dynamic Matrix") {
    GIVEN("A dynamic Matrix of zero-size") {
        Matrix<int> matrix(0, 0);
        THEN("Matrix.contents() returns a span of zero-size") {
            CHECK(matrix.contents().size() == 0);
        }
    }
}

SCENARIO("Get contents of non-zero-size fixed Matrix") {
    GIVEN("A fixed Matrix of non-zero-size with some contents") {
        Matrix<int, 3, 2> matrix = {
            {1, 2,},
            {3, 4,},
            {6, 3,},
        };
        WHEN("A span is retrieved from Matrix.contents()") {
            auto mc = matrix.contents();
            THEN("That span's size is the same as the Matrix's") {
                CHECK(mc.size() == 3 * 2);
            }
            AND_THEN("That span's contents are the same as the Matrix's") {
                std::vector<int> expected = {
                    1, 2,
                    3, 4,
                    6, 3,
                };
                // collect Matrix contents into a vector
                std::vector<int> contents(mc.begin(), mc.end());
                CHECK(contents == expected);
            }
        }
    }
}

SCENARIO("Get contents of non-zero-size dynamic Matrix") {
    GIVEN("A dynamic Matrix of non-zero-size with some contents") {
        Matrix<int> matrix(
            3, 2,
            {
                {1, 2,},
                {3, 4,},
                {6, 3,},
            }
        );
        WHEN("A span is retrieved from Matrix.contents()") {
            auto mc = matrix.contents();
            THEN("That span's size is the same as the Matrix's") {
                CHECK(mc.size() == 3 * 2);
            }
            AND_THEN("That span's contents are the same as the Matrix's") {
                std::vector<int> expected = {
                    1, 2,
                    3, 4,
                    6, 3,
                };
                // collect Matrix contents into a vector
                std::vector<int> contents(mc.begin(), mc.end());
                CHECK(contents == expected);
            }
        }
    }

}

SCENARIO("Set contents of non-zero-size fixed Matrix") {
    GIVEN("A fixed Matrix of non-zero-size") {
        Matrix<int, 2, 3> matrix;
        WHEN("Some of its contents are set through Matrix.contents()") {
            auto mc = matrix.contents();
            mc[3] = 467;
            mc[5] = -123;
            THEN("The Matrix contents are changed accordingly") {
                std::vector<int> expected = {
                    {}, {},
                    {}, 467,
                    {}, -123,
                };
                auto mc = matrix.contents();
                // collect Matrix contents into a vector
                std::vector<int> contents(mc.begin(), mc.end());
                CHECK(contents == expected);
            }
        }
    }
}

SCENARIO("Set contents of non-zero-size dynamic Matrix") {
    GIVEN("A dynamic Matrix of non-zero-size") {
        Matrix<int> matrix(2, 3);
        WHEN("Some of its contents are set through Matrix.contents()") {
            auto mc = matrix.contents();
            mc[3] = 467;
            mc[5] = -123;
            THEN("The Matrix contents are changed accordingly") {
                std::vector<int> expected = {
                    {}, {},
                    {}, 467,
                    {}, -123,
                };
                auto mc = matrix.contents();
                // collect Matrix contents into a vector
                std::vector<int> contents(mc.begin(), mc.end());
                CHECK(contents == expected);
            }
        }
    }
}
