#pragma once

#include "rational.hpp"
#include <cmath>
#include <ostream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

class Dice {
public:
    using integer_type = int64_t;
    using real_type = double;
    using result_type = Rational;
    Dice() = default;
    explicit Dice(integer_type n, integer_type faces = 6, const Rational& factor = 1) { insert(n, faces, factor); }
    explicit Dice(std::string_view str);
    template <typename RNG> Rational operator()(RNG& rng);
    Dice operator+() const { return *this; }
    Dice operator-() const;
    Dice& operator+=(const Dice& rhs);
    Dice& operator+=(const Rational& rhs) { modifier_ += rhs; return *this; }
    Dice& operator-=(const Dice& rhs);
    Dice& operator-=(const Rational& rhs) { modifier_ -= rhs; return *this; }
    Dice& operator*=(const Rational& rhs);
    Dice& operator/=(const Rational& rhs) { return *this *= Rational(rhs.den(), rhs.num()); }
    Rational mean() const noexcept;
    Rational variance() const noexcept;
    real_type sd() const noexcept { return std::sqrt(real_type(variance())); }
    Rational min() const noexcept;
    Rational max() const noexcept;
    std::string str() const;
private:
    using distribution_type = std::uniform_int_distribution<integer_type>;
    struct dice_group {
        distribution_type one_dice;
        integer_type n_dice;
        Rational factor;
    };
    std::vector<dice_group> groups_;
    Rational modifier_;
    void insert(integer_type n, integer_type faces, const Rational& factor);
};

template <typename RNG>
Rational Dice::operator()(RNG& rng) {
    Rational sum = modifier_;
    for (auto& g: groups_) {
        integer_type roll = 0;
        for (integer_type i = 0; i < g.n_dice; ++i)
            roll += g.one_dice(rng);
        sum += roll * g.factor;
    }
    return sum;
}

inline Dice operator+(const Dice& lhs, const Dice& rhs) { auto d = lhs; d += rhs; return d; }
inline Dice operator+(const Dice& lhs, const Rational& rhs) { auto d = lhs; d += rhs; return d; }
inline Dice operator+(const Rational& lhs, const Dice& rhs) { auto d = rhs; d += lhs; return d; }
inline Dice operator-(const Dice& lhs, const Dice& rhs) { auto d = lhs; d -= rhs; return d; }
inline Dice operator-(const Dice& lhs, const Rational& rhs) { auto d = lhs; d -= rhs; return d; }
inline Dice operator-(const Rational& lhs, const Dice& rhs) { auto d = - rhs; d += lhs; return d; }
inline Dice operator*(const Dice& lhs, const Rational& rhs) { auto d = lhs; d *= rhs; return d; }
inline Dice operator*(const Rational& lhs, const Dice& rhs) { auto d = rhs; d *= lhs; return d; }
inline Dice operator/(const Dice& lhs, const Rational& rhs) { auto d = lhs; d /= rhs; return d; }
inline std::ostream& operator<<(std::ostream& out, const Dice& d) { return out << d.str(); }
inline Dice operator""_d4(unsigned long long n) { return Dice(n, 4); }
inline Dice operator""_d6(unsigned long long n) { return Dice(n, 6); }
inline Dice operator""_d8(unsigned long long n) { return Dice(n, 8); }
inline Dice operator""_d10(unsigned long long n) { return Dice(n, 10); }
inline Dice operator""_d12(unsigned long long n) { return Dice(n, 12); }
inline Dice operator""_d20(unsigned long long n) { return Dice(n, 20); }
inline Dice operator""_d100(unsigned long long n) { return Dice(n, 100); }
