#include <catch2/catch.hpp>

#include <gryde/Matrix.hpp>


SCENARIO("Comparing zero-size Matrices for equality") {
    GIVEN("A fixed-size Matrix of zero-size") {
        Matrix<int, 0, 0> a;
        GIVEN("Another fixed-size Matrix of zero-size") {
            Matrix<int, 0, 0> b;
            THEN("The matrices compare equal") {
                CHECK(a == b);
                CHECK_FALSE(a != b);
            }
        }
        GIVEN("A dynamic-size Matrix of zero-size") {
            Matrix<int> b(0, 0);
            THEN("The matrices compare equal") {
                CHECK(a == b);
                CHECK_FALSE(a != b);
            }
        }
    }
    GIVEN("A dynamic-size Matrix of zero-size") {
        Matrix<int> a(0, 0);
        GIVEN("Another dynamic-size Matrix of zero-size") {
            Matrix<int> b(0, 0);
            THEN("The matrices compare equal") {
                CHECK(a == b);
                CHECK_FALSE(a != b);
            }
        }
        GIVEN("A fixed-size Matrix of zero-size") {
            Matrix<int, 0, 0> b;
            THEN("The matrices compare equal") {
                CHECK(a == b);
                CHECK_FALSE(a != b);
            }
        }
    }
}

// can't compare fixed with fixed for wrong-size tests as it's a compile time error

SCENARIO("Comparing different-sized Matrices for equality") {
    GIVEN("A fixed-size Matrix of some non-zero size") {
        Matrix<int, 3, 2> a;
        GIVEN("A dynamic-size Matrix of some different non-zero size") {
            Matrix<int> b(9, 7);
            THEN("Attempting to compare them for equality or inequality throws an exception") {
                CHECK_THROWS(a == b);
                CHECK_THROWS(a != b);
            }
        }
    }
    GIVEN("A dynamic-size Matrix of some non-zero size") {
        Matrix<int> a(3, 2);
        GIVEN("Another dynamic-size Matrix of some different non-zero size") {
            Matrix<int> b(9, 7);
            THEN("Attempting to compare them for equality or inequality throws an exception") {
                CHECK_THROWS(a == b);
                CHECK_THROWS(a != b);
            }
        }
        GIVEN("A fixed-size Matrix of some different non-zero size") {
            Matrix<int, 9, 7> b;
            THEN("Attempting to compare them for equality or inequality throws an exception") {
                CHECK_THROWS(a == b);
                CHECK_THROWS(a != b);
            }
        }
    }
}

SCENARIO("Comparing same-sized Matrices for equality") {
    GIVEN("A fixed-size Matrix of some non-zero size and with some contents") {
        Matrix<int, 3, 2> a = {
            {1, 2,},
            {3, 4,},
            {5, 6,},
        };
        GIVEN("Another fixed-size Matrix of same size and contents") {
            Matrix<int, 3, 2> b = {
                {1, 2,},
                {3, 4,},
                {5, 6,},
            };
            THEN("The matrices compare equal") {
                CHECK(a == b);
                CHECK_FALSE(a != b);
            }
        }
        GIVEN("Another fixed-size Matrix of same size and different contents") {
            Matrix<int, 3, 2> b = {
                {9, 1,},
                {7, 3,},
                {5, 6,},
            };
            THEN("The matrices compare inequal") {
                CHECK_FALSE(a == b);
                CHECK(a != b);
            }
        }
        GIVEN("A dynamic-size Matrix of same size and contents") {
            Matrix<int> b(
                3, 2,
                {
                    {1, 2,},
                    {3, 4,},
                    {5, 6,},
                }
            );
            THEN("The matrices compare equal") {
                CHECK(a == b);
                CHECK_FALSE(a != b);
            }
        }
        GIVEN("A dynamic-size Matrix of same size and different contents") {
            Matrix<int> b(
                3, 2,
                {
                    {1, 1,},
                    {2, 3,},
                    {5, 4,},
                }
            );
            THEN("The matrices compare inequal") {
                CHECK_FALSE(a == b);
                CHECK(a != b);
            }
        }
    }
    GIVEN("A dynamic-size Matrix of some non-zero size and with some contents") {
        Matrix<int> a(
            3, 2,
            {
                {1, 1,},
                {4, 3,},
                {9, 7,},
            }
        );
        GIVEN("Another dynamic-size Matrix of same size and contents") {
            Matrix<int> b(
                3, 2,
                {
                    {1, 1,},
                    {4, 3,},
                    {9, 7,},
                }
            );
            THEN("The matrices compare equal") {
                CHECK(a == b);
                CHECK_FALSE(a != b);
            }
        }
        GIVEN("Another dynamic-size Matrix of same size and different contents") {
            Matrix<int> b(
                3, 2,
                {
                    {9, 1,},
                    {4, 2,},
                    {3, 7,},
                }
            );
            THEN("The matrices compare inequal") {
                CHECK_FALSE(a == b);
                CHECK(a != b);
            }
        }
        GIVEN("A fixed-size Matrix of same size and contents") {
            Matrix<int, 3, 2> b = {
                {1, 1,},
                {4, 3,},
                {9, 7,},
            };
            THEN("The matrices compare equal") {
                CHECK(a == b);
                CHECK_FALSE(a != b);
            }
        }
        GIVEN("A fixed-size Matrix of same size and different contents") {
            Matrix<int, 3, 2> b = {
                {2, 4,},
                {1, 3,},
                {9, 17,},
            };
            THEN("The matrices compare inequal") {
                CHECK_FALSE(a == b);
                CHECK(a != b);
            }
        }
    }
}
