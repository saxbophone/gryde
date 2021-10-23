#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>


SCENARIO("Adding fixed-size Matrix to fixed-size Matrix") {
    GIVEN("A zero-sized fixed-size Matrx") {
        Matrix<int, 0, 0> a;
        GIVEN("Another zero-sized fixed-size Matrx") {
            Matrix<int, 0, 0> b;
            THEN("The two matrices can be added together, giving another empty Matrix") {
                Matrix<int, 0, 0> c = a + b;
                SUCCEED();
            }
        }
    }
    GIVEN("A non-zero-sized fixed-size Matrx with some contents") {
        Matrix<int, 3, 2> a = {
            {9, 3,},
            {1, 12,},
            {-3, 17,},
        };
        GIVEN("Another fixed-size Matrx with the same dimensions and some contents") {
            Matrix<int, 3, 2> b = {
                {3, 17,},
                {1, 2,},
                {9, 66,},
            };
            WHEN("The two matrices are added together") {
                Matrix<int, 3, 2> c = a + b;
                THEN("The result is the element-wise addition of the two matrices") {
                    Matrix<int, 3, 2> expected = {
                        {12, 20,},
                        {2, 14,},
                        {6, 83,},
                    };
                    CHECK(c == expected);
                }
            }
        }
    }
}

SCENARIO("Adding fixed-size Matrix to dynamic-size Matrix") {
    GIVEN("A zero-sized fixed-size Matrx") {
        Matrix<int, 0, 0> a;
        GIVEN("A zero-sized dynamic-size Matrx") {
            Matrix<int> b(0, 0);
            THEN("The two matrices can be added together, giving another empty Matrix of fixed-size") {
                Matrix<int, 0, 0> c = a + b;
                SUCCEED();
            }
        }
    }
    GIVEN("A non-zero-sized fixed-size Matrx with some contents") {
        Matrix<int, 3, 2> a = {
            {9, 3,},
            {1, 12,},
            {-3, 17,},
        };
        GIVEN("A dynamic-size Matrx with the same dimensions and some contents") {
            Matrix<int> b(
                3, 2,
                {
                    {3, 17,},
                    {1, 2,},
                    {9, 66,},
                }
            );
            WHEN("The two matrices are added together") {
                Matrix<int, 3, 2> c = a + b;
                THEN("The result is the element-wise addition of the two matrices, as a fixed-size Matrix") {
                    Matrix<int, 3, 2> expected = {
                        {12, 20,},
                        {2, 14,},
                        {6, 83,},
                    };
                    CHECK(c == expected);
                }
            }
        }
    }
}

SCENARIO("Adding dynamic-size Matrix to dynamic-size Matrix") {
    GIVEN("A zero-sized dynamic-size Matrx") {
        Matrix<int> a(0, 0);
        GIVEN("Another zero-sized dynamic-size Matrx") {
            Matrix<int> b(0, 0);
            THEN("The two matrices can be added together, giving another empty Matrix of dynamic-size") {
                Matrix<int> c = a + b;
                CHECK(c.row_count() == 0);
                CHECK(c.col_count() == 0);
            }
        }
    }
    GIVEN("A non-zero-sized dynamic-size Matrx with some contents") {
        Matrix<int> a(
            3, 2,
            {
                {9, 3,},
                {1, 12,},
                {-3, 17,},
            }
        );
        GIVEN("A dynamic-size Matrx with the same dimensions and some contents") {
            Matrix<int> b(
                3, 2,
                {
                    {3, 17,},
                    {1, 2,},
                    {9, 66,},
                }
            );
            WHEN("The two matrices are added together") {
                Matrix<int> c = a + b;
                THEN("The result is the element-wise addition of the two matrices, as a dynamic-size Matrix") {
                    Matrix<int> expected(
                        3, 2,
                        {
                            {12, 20,},
                            {2, 14,},
                            {6, 83,},
                        }
                    );
                    CHECK(c.row_count() == 3);
                    CHECK(c.col_count() == 2);
                    CHECK(c == expected);
                }
            }
        }
    }
}

SCENARIO("Adding dynamic-size Matrix to fixed-size Matrix") {
    GIVEN("A zero-sized dynamic-size Matrx") {
        Matrix<int> a(0, 0);
        GIVEN("A zero-sized fixed-size Matrx") {
            Matrix<int, 0, 0> b;
            THEN("The two matrices can be added together, giving another empty Matrix of fixed-size") {
                Matrix<int, 0, 0> c = a + b;
                SUCCEED();
            }
        }
    }
    GIVEN("A non-zero-sized dynamic-size Matrx with some contents") {
        Matrix<int> a(
            3, 2,
            {
                {9, 3,},
                {1, 12,},
                {-3, 17,},
            }
        );
        GIVEN("A fixed-size Matrx with the same dimensions and some contents") {
            Matrix<int, 3, 2> b = {
                {3, 17,},
                {1, 2,},
                {9, 66,},
            };
            WHEN("The two matrices are added together") {
                Matrix<int, 3, 2> c = a + b;
                THEN("The result is the element-wise addition of the two matrices, as a fixed-size Matrix") {
                    Matrix<int, 3, 2> expected = {
                        {12, 20,},
                        {2, 14,},
                        {6, 83,},
                    };
                    CHECK(c == expected);
                }
            }
        }
    }
}
