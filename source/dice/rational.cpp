#include "dice/rational.hpp"
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <stdexcept>

Rational::Rational(integer_type num, integer_type den):
num_(num), den_(den) {
    if (den_ == 0)
        throw std::invalid_argument("Division by zero");
    normalize();
}

Rational Rational::frac_part() const noexcept {
    Rational r = *this;
    r.num_ %= den_;
    return r;
}

Rational Rational::abs() const noexcept {
    auto r = *this;
    r.num_ = std::abs(r.num_);
    return r;
}

std::string Rational::str() const {
    auto s = std::to_string(num_);
    if (den_ != 1)
        s += '/' + std::to_string(den_);
    return s;
}

std::string Rational::mixed() const {
    if (num_ == 0)
        return "0";
    if (std::abs(num_) < den_)
        return str();
    auto s = std::to_string(int_part());
    auto f = frac_part();
    if (f)
        s += ' ' + f.abs().str();
    return s;
}

Rational::integer_type Rational::round() const noexcept {
    auto i = int_part();
    auto f2 = 2 * frac_part();
    if (f2 >= 1)
        ++i;
    else if (f2 < -1)
        --i;
    return i;
}

Rational::integer_type Rational::floor() const noexcept {
    auto i = int_part();
    auto f = frac_part();
    if (f < 0)
        --i;
    return i;
}

Rational::integer_type Rational::ceil() const noexcept {
    auto i = int_part();
    auto f = frac_part();
    if (f > 0)
        ++i;
    return i;
}

void Rational::normalize() noexcept {
    if (den_ < 0) {
        num_ = - num_;
        den_ = - den_;
    }
    auto gcd = std::gcd(num_, den_);
    num_ /= gcd;
    den_ /= gcd;
}

Rational Rational::operator-() const noexcept {
    auto r = *this;
    r.num_ = - r.num_;
    return r;
}

Rational& Rational::operator+=(const Rational& rhs) noexcept {
    auto lcm = std::lcm(den_, rhs.den_);
    num_ = num_ * (lcm / den_) + rhs.num_ * (lcm / rhs.den_);
    den_ = lcm;
    normalize();
    return *this;
}

Rational& Rational::operator*=(const Rational& rhs) noexcept {
    auto n = num_ * rhs.num_;
    den_ *= rhs.den_;
    num_ = n;
    normalize();
    return *this;
}

Rational& Rational::operator/=(const Rational& rhs) {
    *this = {num_ * rhs.den_, den_ * rhs.num_};
    return *this;
}

Rational operator+(const Rational& lhs, const Rational& rhs) noexcept {
    Rational r = lhs;
    r += rhs;
    return r;
}

Rational operator-(const Rational& lhs, const Rational& rhs) noexcept {
    Rational r = lhs;
    r -= rhs;
    return r;
}

Rational operator*(const Rational& lhs, const Rational& rhs) noexcept {
    Rational r = lhs;
    r *= rhs;
    return r;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational r = lhs;
    r /= rhs;
    return r;
}

bool operator==(const Rational& lhs, const Rational& rhs) noexcept {
    return lhs.num() == rhs.num() && lhs.den() == rhs.den();
}

bool operator<(const Rational& lhs, const Rational& rhs) noexcept {
    auto lcm = std::lcm(lhs.den(), rhs.den());
    return lhs.num() * (lcm / lhs.den()) < rhs.num() * (lcm / rhs.den());
}
