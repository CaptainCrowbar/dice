#include "unit-test.hpp"

int main() {
    BEGIN_UNIT_TESTS();
    UNIT_TEST(test_rational_construction);
    UNIT_TEST(test_rational_partition);
    UNIT_TEST(test_rational_formatting);
    UNIT_TEST(test_rational_arithmetic);
    UNIT_TEST(test_dice_statistics);
    UNIT_TEST(test_dice_parser);
    UNIT_TEST(test_dice_generation);
    END_UNIT_TESTS();
}
