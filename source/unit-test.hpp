#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>
#include <string>

inline std::atomic<int>& test_failures() noexcept {
    static std::atomic<int> count(0);
    return count;
}

#define CATCH_TEST() \
    catch (const std::exception& ex) { FAIL_TEST("Unexpected exception: " << ex.what()); } \
    catch (...) { FAIL_TEST("Unexpected exception"); }
#define FAIL_TEST(message) { \
    ++test_failures(); \
    std::cout << "*** [" << __FILE__ << ":" << __LINE__ << "] " << message << std::endl; \
}

#define TRY(expr) \
    try { expr; } \
    CATCH_TEST()
#define TEST(expr) \
    try { if (! (expr)) FAIL_TEST("Expression is false: " << #expr); } \
    CATCH_TEST()
#define TEST_EQUAL(lhs, rhs) \
    try { auto a = lhs; auto b = rhs; if (a != b) FAIL_TEST("Expressions are not equal: lhs = " << a << ", rhs = " << b); } \
    CATCH_TEST()
#define TEST_NEAR(lhs, rhs, epsilon) \
    try { auto a = double(lhs); auto b = double(rhs); if (std::abs(a - b) > epsilon) FAIL_TEST("Expressions are not close enough: lhs = " << a << ", rhs = " << b); } \
    CATCH_TEST()
#define TEST_THROW(expr, except) \
    try { expr; FAIL_TEST("Expression failed to throw: " << #expr); } \
    catch (const except&) {} \
    CATCH_TEST()

#define UNIT_TEST(function) { \
    extern void function(); \
    std::string title = # function; \
    std::replace(title.begin(), title.end(), '_', ' '); \
    title[0] = char(std::toupper(title[0])); \
    std::cout << title << std::endl; \
    function(); \
}

#define BEGIN_UNIT_TESTS() \
    using namespace std::chrono; \
    static const auto rule = std::string(30, '=') + '\n'; \
    auto start = system_clock::now(); \
    std::cout << rule;

#define END_UNIT_TESTS() \
    std::cout << rule; \
    double time = 1e-6 * duration_cast<microseconds>(system_clock::now() - start).count(); \
    if (test_failures() == 0) \
        std::cout << "OK: all tests passed\n"; \
    else \
        std::cout << "*** Tests failed: " << test_failures() << "\n"; \
    std::cout << "Time: " << time << " s\n"; \
    return int(test_failures() > 0);
