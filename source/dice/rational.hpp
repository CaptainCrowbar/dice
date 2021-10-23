#pragma once

#include <cstdint>
#include <ostream>
#include <string>

class Rational {
public:
    using integer_type = int64_t;
    using real_type = double;
    Rational() = default;
    Rational(integer_type n) noexcept: num_(n), den_(1) {}
    Rational(integer_type num, integer_type den);
    integer_type num() const noexcept { return num_; }
    integer_type den() const noexcept { return den_; }
    integer_type int_part() const noexcept { return num_ / den_; }
    Rational frac_part() const noexcept;
    Rational abs() const noexcept;
    int sign() const noexcept { return num_ < 0 ? -1 : num_ > 0 ? 1 : 0; }
    std::string str() const;
    std::string mixed() const;
    integer_type round() const noexcept;
    integer_type floor() const noexcept;
    integer_type ceil() const noexcept;
    explicit operator bool() const noexcept { return num_ != 0; }
    explicit operator real_type() const noexcept { return real_type(num_) / real_type(den_); }
    Rational operator+() const noexcept { return *this; }
    Rational operator-() const noexcept;
    Rational& operator++() noexcept { return *this += 1; }
    Rational operator++(int) noexcept { auto r = *this; ++*this; return r; }
    Rational& operator--() noexcept { return *this -= 1; }
    Rational operator--(int) noexcept { auto r = *this; --*this; return r; }
    Rational& operator+=(const Rational& rhs) noexcept;
    Rational& operator-=(const Rational& rhs) noexcept { return *this += - rhs; }
    Rational& operator*=(const Rational& rhs) noexcept;
    Rational& operator/=(const Rational& rhs);
private:
    integer_type num_ = 0;
    integer_type den_ = 1;
    void normalize() noexcept;
};

Rational operator+(const Rational& lhs, const Rational& rhs) noexcept;
Rational operator-(const Rational& lhs, const Rational& rhs) noexcept;
Rational operator*(const Rational& lhs, const Rational& rhs) noexcept;
Rational operator/(const Rational& lhs, const Rational& rhs);
bool operator==(const Rational& lhs, const Rational& rhs) noexcept;
inline bool operator!=(const Rational& lhs, const Rational& rhs) noexcept { return ! (lhs == rhs); }
bool operator<(const Rational& lhs, const Rational& rhs) noexcept;
inline bool operator>(const Rational& lhs, const Rational& rhs) noexcept { return rhs < lhs; }
inline bool operator<=(const Rational& lhs, const Rational& rhs) noexcept { return ! (rhs < lhs); }
inline bool operator>=(const Rational& lhs, const Rational& rhs) noexcept { return ! (lhs < rhs); }
inline std::ostream& operator<<(std::ostream& out, const Rational& r) { return out << r.str(); }
