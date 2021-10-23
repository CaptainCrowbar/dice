#include "dice/dice.hpp"
#include "dice/rational.hpp"
#include "unit-test.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>
#include <stdexcept>

namespace {

    class Statistics {
    public:
        void add(double x) noexcept {
            if (++num_ == 1) {
                min_ = max_ = x;
            } else {
                min_ = std::min(min_, x);
                max_ = std::max(max_, x);
            }
            sum_ += x;
            sum2_ += x * x;
        }
        int num() const noexcept { return num_; }
        double min() const noexcept { return min_; }
        double max() const noexcept { return max_; }
        double mean() const noexcept { return num_ < 1 ? 0 : sum_ / double(num_); }
        double sd() const noexcept { return num_ < 2 ? 0 : std::sqrt((sum2_ - sum_ * sum_ / num_) / (num_ - 1)); }
    private:
        double min_ = 0;
        double max_ = 0;
        double sum_ = 0;
        double sum2_ = 0;
        int num_ = 0;
    };

}

void test_dice_arithmetic() {

    Dice a, b, c;
    Rational r(2, 3);

    TRY(a = Dice(2, 6));
    TRY(b = Dice(3, 10));

    TRY(c = a * 3);  TEST_EQUAL(c.str(), "2d6*3");
    TRY(c = a / 3);  TEST_EQUAL(c.str(), "2d6/3");
    TRY(c = a * r);  TEST_EQUAL(c.str(), "2d6*2/3");
    TRY(c = a / r);  TEST_EQUAL(c.str(), "2d6*3/2");
    TRY(c = a + b);  TEST_EQUAL(c.str(), "3d10+2d6");
    TRY(c = a - b);  TEST_EQUAL(c.str(), "-3d10+2d6");
    TRY(c = b - a);  TEST_EQUAL(c.str(), "3d10-2d6");

    TEST_THROW(c = a / 0, std::invalid_argument);

}

void test_dice_statistics() {

    Dice dice;

    TEST_EQUAL(dice.min(), 0);
    TEST_EQUAL(dice.max(), 0);
    TEST_EQUAL(dice.mean(), 0);
    TEST_EQUAL(dice.sd(), 0);
    TEST_EQUAL(dice.str(), "0");

    TRY(dice += 5);
    TEST_EQUAL(dice.min(), 5);
    TEST_EQUAL(dice.max(), 5);
    TEST_EQUAL(dice.mean(), 5);
    TEST_EQUAL(dice.sd(), 0);
    TEST_EQUAL(dice.str(), "5");

    TRY(dice += Dice(2, 6));
    TEST_EQUAL(dice.min(), 7);
    TEST_EQUAL(dice.max(), 17);
    TEST_EQUAL(dice.mean(), 12);
    TEST_NEAR(dice.sd(), 2.415229, 1e-6);
    TEST_EQUAL(dice.str(), "2d6+5");

    TRY(dice -= 10);
    TEST_EQUAL(dice.min(), -3);
    TEST_EQUAL(dice.max(), 7);
    TEST_EQUAL(dice.mean(), 2);
    TEST_NEAR(dice.sd(), 2.415229, 1e-6);
    TEST_EQUAL(dice.str(), "2d6-5");

    TRY(dice -= Dice(2, 10));
    TEST_EQUAL(dice.min(), -23);
    TEST_EQUAL(dice.max(), 5);
    TEST_EQUAL(dice.mean(), -9);
    TEST_NEAR(dice.sd(), 4.725816, 1e-6);
    TEST_EQUAL(dice.str(), "-2d10+2d6-5");

}

void test_dice_parser() {

    Dice dice;

    TRY(dice = Dice(""));
    TEST_EQUAL(dice.str(), "0");
    TEST_EQUAL(dice.min(), 0);
    TEST_EQUAL(dice.max(), 0);
    TEST_EQUAL(dice.mean(), 0);
    TEST_EQUAL(dice.sd(), 0);

    TRY(dice = Dice("5"));
    TEST_EQUAL(dice.str(), "5");
    TEST_EQUAL(dice.min(), 5);
    TEST_EQUAL(dice.max(), 5);
    TEST_EQUAL(dice.mean(), 5);
    TEST_EQUAL(dice.sd(), 0);

    TRY(dice = Dice("2d6"));
    TEST_EQUAL(dice.str(), "2d6");
    TEST_EQUAL(dice.min(), 2);
    TEST_EQUAL(dice.max(), 12);
    TEST_EQUAL(dice.mean(), 7);
    TEST_NEAR(dice.sd(), 2.415229, 1e-6);

    TRY(dice = Dice("2d10+2d6+10"));
    TEST_EQUAL(dice.str(), "2d10+2d6+10");
    TEST_EQUAL(dice.min(), 14);
    TEST_EQUAL(dice.max(), 42);
    TEST_EQUAL(dice.mean(), 28);
    TEST_NEAR(dice.sd(), 4.725816, 1e-6);

    TRY(dice = Dice("2d10-2d6+10"));
    TEST_EQUAL(dice.str(), "2d10-2d6+10");
    TEST_EQUAL(dice.min(), 0);
    TEST_EQUAL(dice.max(), 28);
    TEST_EQUAL(dice.mean(), 14);
    TEST_NEAR(dice.sd(), 4.725816, 1e-6);

    TRY(dice = Dice(" 3*2d10 - 2d6/4 + d8*6/8 + 10\n"));
    TEST_EQUAL(dice.str(), "2d10*3+d8*3/4-2d6/4+10");
    TEST_EQUAL(dice.min(), Rational(55, 4));
    TEST_EQUAL(dice.max(), Rational(151, 2));
    TEST_EQUAL(dice.mean(), Rational(357, 8));
    TEST_NEAR(dice.sd(), 12.321433, 1e-6);

}

void test_dice_generation() {

    static constexpr int iterations = 1'000'000;
    static constexpr double tolerance = 0.005;

    Dice dice;
    std::minstd_rand rng;
    Statistics stats;
    Rational x;

    stats = {};
    TRY(dice = Dice(2, 6));
    for (int i = 0; i < iterations; ++i) {
        TRY(x = dice(rng));
        TRY(stats.add(double(x)));
    }
    TEST_EQUAL(stats.min(), double(dice.min()));
    TEST_EQUAL(stats.max(), double(dice.max()));
    TEST_NEAR(stats.mean(), double(dice.mean()), tolerance);
    TEST_NEAR(stats.sd(), dice.sd(), tolerance);

    stats = {};
    TRY(dice = Dice("2d6"));
    for (int i = 0; i < iterations; ++i) {
        TRY(x = dice(rng));
        TRY(stats.add(double(x)));
    }
    TEST_EQUAL(stats.min(), double(dice.min()));
    TEST_EQUAL(stats.max(), double(dice.max()));
    TEST_NEAR(stats.mean(), double(dice.mean()), tolerance);
    TEST_NEAR(stats.sd(), dice.sd(), tolerance);

    stats = {};
    TRY(dice = Dice("2d10-2d6+10"));
    for (int i = 0; i < iterations; ++i) {
        TRY(x = dice(rng));
        TRY(stats.add(double(x)));
    }
    TEST_EQUAL(stats.min(), double(dice.min()));
    TEST_EQUAL(stats.max(), double(dice.max()));
    TEST_NEAR(stats.mean(), double(dice.mean()), tolerance);
    TEST_NEAR(stats.sd(), dice.sd(), tolerance);

}

void test_dice_literals() {

    Dice dice;

    TRY(dice = 5_d4);    TEST_EQUAL(dice.str(), "5d4");
    TRY(dice = 5_d6);    TEST_EQUAL(dice.str(), "5d6");
    TRY(dice = 5_d8);    TEST_EQUAL(dice.str(), "5d8");
    TRY(dice = 5_d10);   TEST_EQUAL(dice.str(), "5d10");
    TRY(dice = 5_d12);   TEST_EQUAL(dice.str(), "5d12");
    TRY(dice = 5_d20);   TEST_EQUAL(dice.str(), "5d20");
    TRY(dice = 5_d100);  TEST_EQUAL(dice.str(), "5d100");

}
