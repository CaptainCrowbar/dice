#include "test/unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // rational-test.cpp
    UNIT_TEST(rational_construction)
    UNIT_TEST(rational_partition)
    UNIT_TEST(rational_formatting)
    UNIT_TEST(rational_arithmetic)

    // dice-test.cpp
    UNIT_TEST(dice_arithmetic)
    UNIT_TEST(dice_statistics)
    UNIT_TEST(dice_parser)
    UNIT_TEST(dice_generation)
    UNIT_TEST(dice_literals)

    return RS::UnitTest::end_tests();

}
