#include "rational.hpp"
#include "unit-test.hpp"
#include <stdexcept>
#include <string>

void test_rational_construction() {

    Rational r;

    /**/                    TEST_EQUAL(r.num(), 0);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(double(r), 0);
    TRY(r = 42);            TEST_EQUAL(r.num(), 42);   TEST_EQUAL(r.den(), 1);  TEST_EQUAL(double(r), 42);
    TRY(r = -42);           TEST_EQUAL(r.num(), -42);  TEST_EQUAL(r.den(), 1);  TEST_EQUAL(double(r), -42);
    TRY((r = {1, 4}));      TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 4);  TEST_EQUAL(double(r), 0.25);
    TRY((r = {4, 1}));      TEST_EQUAL(r.num(), 4);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(double(r), 4);
    TRY((r = {3, 4}));      TEST_EQUAL(r.num(), 3);    TEST_EQUAL(r.den(), 4);  TEST_EQUAL(double(r), 0.75);
    TRY((r = {5, 4}));      TEST_EQUAL(r.num(), 5);    TEST_EQUAL(r.den(), 4);  TEST_EQUAL(double(r), 1.25);
    TRY((r = {15, 20}));    TEST_EQUAL(r.num(), 3);    TEST_EQUAL(r.den(), 4);  TEST_EQUAL(double(r), 0.75);
    TRY((r = {25, 20}));    TEST_EQUAL(r.num(), 5);    TEST_EQUAL(r.den(), 4);  TEST_EQUAL(double(r), 1.25);
    TRY((r = {-25, 20}));   TEST_EQUAL(r.num(), -5);   TEST_EQUAL(r.den(), 4);  TEST_EQUAL(double(r), -1.25);
    TRY((r = {15, -20}));   TEST_EQUAL(r.num(), -3);   TEST_EQUAL(r.den(), 4);  TEST_EQUAL(double(r), -0.75);
    TRY((r = {-15, -20}));  TEST_EQUAL(r.num(), 3);    TEST_EQUAL(r.den(), 4);  TEST_EQUAL(double(r), 0.75);

    TEST_THROW(r = Rational(42, 0), std::invalid_argument);

}

void test_rational_partition() {

    Rational r;

    TRY((r = {6, 3}));   TEST_EQUAL(r.int_part(), 2);   TEST_EQUAL(r.frac_part(), 0);
    TRY((r = {5, 3}));   TEST_EQUAL(r.int_part(), 1);   TEST_EQUAL(r.frac_part(), Rational(2, 3));
    TRY((r = {4, 3}));   TEST_EQUAL(r.int_part(), 1);   TEST_EQUAL(r.frac_part(), Rational(1, 3));
    TRY((r = {3, 3}));   TEST_EQUAL(r.int_part(), 1);   TEST_EQUAL(r.frac_part(), 0);
    TRY((r = {2, 3}));   TEST_EQUAL(r.int_part(), 0);   TEST_EQUAL(r.frac_part(), Rational(2, 3));
    TRY((r = {1, 3}));   TEST_EQUAL(r.int_part(), 0);   TEST_EQUAL(r.frac_part(), Rational(1, 3));
    TRY((r = {0, 3}));   TEST_EQUAL(r.int_part(), 0);   TEST_EQUAL(r.frac_part(), 0);
    TRY((r = {-1, 3}));  TEST_EQUAL(r.int_part(), 0);   TEST_EQUAL(r.frac_part(), Rational(-1, 3));
    TRY((r = {-2, 3}));  TEST_EQUAL(r.int_part(), 0);   TEST_EQUAL(r.frac_part(), Rational(-2, 3));
    TRY((r = {-3, 3}));  TEST_EQUAL(r.int_part(), -1);  TEST_EQUAL(r.frac_part(), 0);
    TRY((r = {-4, 3}));  TEST_EQUAL(r.int_part(), -1);  TEST_EQUAL(r.frac_part(), Rational(-1, 3));
    TRY((r = {-5, 3}));  TEST_EQUAL(r.int_part(), -1);  TEST_EQUAL(r.frac_part(), Rational(-2, 3));
    TRY((r = {-6, 3}));  TEST_EQUAL(r.int_part(), -2);  TEST_EQUAL(r.frac_part(), 0);

}

void test_rational_formatting() {

    Rational r;

    /**/                    TEST_EQUAL(r.str(), "0");        TEST_EQUAL(r.mixed(), "0");
    TRY(r = 42);            TEST_EQUAL(r.str(), "42");       TEST_EQUAL(r.mixed(), "42");
    TRY(r = -42);           TEST_EQUAL(r.str(), "-42");      TEST_EQUAL(r.mixed(), "-42");
    TRY((r = {1, 4}));      TEST_EQUAL(r.str(), "1/4");      TEST_EQUAL(r.mixed(), "1/4");
    TRY((r = {4, 1}));      TEST_EQUAL(r.str(), "4");        TEST_EQUAL(r.mixed(), "4");
    TRY((r = {3, 4}));      TEST_EQUAL(r.str(), "3/4");      TEST_EQUAL(r.mixed(), "3/4");
    TRY((r = {5, 4}));      TEST_EQUAL(r.str(), "5/4");      TEST_EQUAL(r.mixed(), "1 1/4");
    TRY((r = {15, 20}));    TEST_EQUAL(r.str(), "3/4");      TEST_EQUAL(r.mixed(), "3/4");
    TRY((r = {25, 20}));    TEST_EQUAL(r.str(), "5/4");      TEST_EQUAL(r.mixed(), "1 1/4");
    TRY((r = {-25, 20}));   TEST_EQUAL(r.str(), "-5/4");     TEST_EQUAL(r.mixed(), "-1 1/4");
    TRY((r = {15, -20}));   TEST_EQUAL(r.str(), "-3/4");     TEST_EQUAL(r.mixed(), "-3/4");
    TRY((r = {-15, -20}));  TEST_EQUAL(r.str(), "3/4");      TEST_EQUAL(r.mixed(), "3/4");
    TRY((r = {6, 3}));      TEST_EQUAL(r.str(), "2");        TEST_EQUAL(r.mixed(), "2");
    TRY((r = {5, 3}));      TEST_EQUAL(r.str(), "5/3");      TEST_EQUAL(r.mixed(), "1 2/3");
    TRY((r = {4, 3}));      TEST_EQUAL(r.str(), "4/3");      TEST_EQUAL(r.mixed(), "1 1/3");
    TRY((r = {3, 3}));      TEST_EQUAL(r.str(), "1");        TEST_EQUAL(r.mixed(), "1");
    TRY((r = {2, 3}));      TEST_EQUAL(r.str(), "2/3");      TEST_EQUAL(r.mixed(), "2/3");
    TRY((r = {1, 3}));      TEST_EQUAL(r.str(), "1/3");      TEST_EQUAL(r.mixed(), "1/3");
    TRY((r = {0, 3}));      TEST_EQUAL(r.str(), "0");        TEST_EQUAL(r.mixed(), "0");
    TRY((r = {-1, 3}));     TEST_EQUAL(r.str(), "-1/3");     TEST_EQUAL(r.mixed(), "-1/3");
    TRY((r = {-2, 3}));     TEST_EQUAL(r.str(), "-2/3");     TEST_EQUAL(r.mixed(), "-2/3");
    TRY((r = {-3, 3}));     TEST_EQUAL(r.str(), "-1");       TEST_EQUAL(r.mixed(), "-1");
    TRY((r = {-4, 3}));     TEST_EQUAL(r.str(), "-4/3");     TEST_EQUAL(r.mixed(), "-1 1/3");
    TRY((r = {-5, 3}));     TEST_EQUAL(r.str(), "-5/3");     TEST_EQUAL(r.mixed(), "-1 2/3");
    TRY((r = {-6, 3}));     TEST_EQUAL(r.str(), "-2");       TEST_EQUAL(r.mixed(), "-2");
    TRY((r = {1000, 3}));   TEST_EQUAL(r.str(), "1000/3");   TEST_EQUAL(r.mixed(), "333 1/3");
    TRY((r = {100, 3}));    TEST_EQUAL(r.str(), "100/3");    TEST_EQUAL(r.mixed(), "33 1/3");
    TRY((r = {10, 3}));     TEST_EQUAL(r.str(), "10/3");     TEST_EQUAL(r.mixed(), "3 1/3");
    TRY((r = {-10, 3}));    TEST_EQUAL(r.str(), "-10/3");    TEST_EQUAL(r.mixed(), "-3 1/3");
    TRY((r = {-100, 3}));   TEST_EQUAL(r.str(), "-100/3");   TEST_EQUAL(r.mixed(), "-33 1/3");
    TRY((r = {-1000, 3}));  TEST_EQUAL(r.str(), "-1000/3");  TEST_EQUAL(r.mixed(), "-333 1/3");

}

void test_rational_arithmetic() {

    Rational x, y, z;

    TRY((x = {5, 4}));
    TRY((y = {25, 6}));

    TEST_EQUAL(x.sign(), 1);
    TEST_EQUAL((- x).sign(), -1);
    TEST_EQUAL(z.sign(), 0);

    TEST(x != y);
    TEST(x < y);
    TEST(x <= y);
    TEST(y > x);
    TEST(y >= x);

    TRY(z = + x);          TEST_EQUAL(z.num(), 5);    TEST_EQUAL(z.den(), 4);   TEST_EQUAL(z.str(), "5/4");
    TRY(z = - x);          TEST_EQUAL(z.num(), -5);   TEST_EQUAL(z.den(), 4);   TEST_EQUAL(z.str(), "-5/4");
    TRY(z = x + y);        TEST_EQUAL(z.num(), 65);   TEST_EQUAL(z.den(), 12);  TEST_EQUAL(z.str(), "65/12");
    TRY(z = x - y);        TEST_EQUAL(z.num(), -35);  TEST_EQUAL(z.den(), 12);  TEST_EQUAL(z.str(), "-35/12");
    TRY(z = y - x);        TEST_EQUAL(z.num(), 35);   TEST_EQUAL(z.den(), 12);  TEST_EQUAL(z.str(), "35/12");
    TRY(z = x * y);        TEST_EQUAL(z.num(), 125);  TEST_EQUAL(z.den(), 24);  TEST_EQUAL(z.str(), "125/24");
    TRY(z = x / y);        TEST_EQUAL(z.num(), 3);    TEST_EQUAL(z.den(), 10);  TEST_EQUAL(z.str(), "3/10");
    TRY(z = y / x);        TEST_EQUAL(z.num(), 10);   TEST_EQUAL(z.den(), 3);   TEST_EQUAL(z.str(), "10/3");
    TRY(z = x.abs());      TEST_EQUAL(z.num(), 5);    TEST_EQUAL(z.den(), 4);   TEST_EQUAL(z.str(), "5/4");
    TRY(z = (- x).abs());  TEST_EQUAL(z.num(), 5);    TEST_EQUAL(z.den(), 4);   TEST_EQUAL(z.str(), "5/4");

}
