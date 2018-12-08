#include "dice.hpp"
#include <algorithm>
#include <cstdlib>
#include <regex>
#include <stdexcept>
#include <utility>

Dice::Dice(std::string_view str) {
    static const auto char_is_whitespace = [] (char c) noexcept {
        static constexpr std::string_view whitespace = "\t\n\f\r ";
        return whitespace.find(c) != std::string::npos;
    };
    static const auto parse_integer = [] (const std::string& str, Dice::integer_type def) noexcept {
        return str.empty() ? def : Dice::integer_type(std::strtoll(str.data(), nullptr, 10));
    };
    static const std::regex pattern(
        "([+-])"               // [1] sign
        "(?:"
            "(?:(\\d+)[*x])?"  // [2] left multiplier
            "(\\d*)"           // [3] number of dice
            "d(\\d*)"          // [4] number of faces
            "(?:[*x](\\d+))?"  // [5] right multiplier
        "|"
            "(\\d+)"           // [6] fixed modifier
        ")"
        "(?:/(\\d+))?",        // [7] divisor
        std::regex_constants::icase | std::regex_constants::optimize);
    std::string text(str);
    text.erase(std::remove_if(text.begin(), text.end(), char_is_whitespace), text.end());
    if (text.empty())
        return;
    if (text[0] != '+' && text[0] != '-')
        text.insert(0, 1, '+');
    std::smatch match;
    auto begin = text.cbegin(), end = text.cend();
    while (begin != end) {
        if (! std::regex_search(begin, end, match, pattern, std::regex_constants::match_continuous))
            throw std::invalid_argument("Invalid dice");
        auto sign = *match[0].first == '-' ? -1 : 1;
        auto divisor = parse_integer(match[7], 1);
        if (match[6].matched) {
            auto factor1 = parse_integer(match[6], 1);
            modifier_ += Rational(sign * factor1, divisor);
        } else {
            auto factor1 = parse_integer(match[2], 1);
            auto n_dice = parse_integer(match[3], 1);
            auto n_faces = parse_integer(match[4], 6);
            auto factor2 = parse_integer(match[5], 1);
            insert(n_dice, n_faces, Rational(sign * factor1 * factor2, divisor));
        }
        begin = match[0].second;
    }
}

Dice Dice::operator-() const {
    Dice d = *this;
    for (auto& g: d.groups_)
        g.factor = - g.factor;
    d.modifier_ = - d.modifier_;
    return d;
}

Dice& Dice::operator+=(const Dice& rhs) {
    Dice d = *this;
    for (auto& g: rhs.groups_)
        d.insert(g.n_dice, g.one_dice.b(), g.factor);
    d.modifier_ += rhs.modifier_;
    *this = std::move(d);
    return *this;
}

Dice& Dice::operator-=(const Dice& rhs) {
    Dice d = *this;
    for (auto& g: rhs.groups_)
        d.insert(g.n_dice, g.one_dice.b(), - g.factor);
    d.modifier_ -= rhs.modifier_;
    *this = std::move(d);
    return *this;
}

Dice& Dice::operator*=(const Rational& rhs) {
    if (rhs) {
        for (auto& g: groups_)
            g.factor *= rhs;
        modifier_ *= rhs;
    } else {
        groups_.clear();
        modifier_ = 0;
    }
    return *this;
}

Rational Dice::mean() const noexcept {
    Rational sum = modifier_;
    for (auto& g: groups_)
        sum += Rational(g.n_dice * (g.one_dice.b() + 1)) * g.factor / Rational(2);
    return sum;
}

Rational Dice::variance() const noexcept {
    Rational sum;
    for (auto& g: groups_)
        sum += Rational(g.n_dice * (g.one_dice.b() * g.one_dice.b() - 1)) * g.factor * g.factor / Rational(12);
    return sum;
}

Rational Dice::min() const noexcept {
    Rational sum = modifier_;
    for (auto& g: groups_) {
        if (g.factor > 0)
            sum += Rational(g.n_dice) * g.factor;
        else
            sum += Rational(g.n_dice * g.one_dice.b()) * g.factor;
    }
    return sum;
}

Rational Dice::max() const noexcept {
    Rational sum = modifier_;
    for (auto& g: groups_) {
        if (g.factor > 0)
            sum += Rational(g.n_dice * g.one_dice.b()) * g.factor;
        else
            sum += Rational(g.n_dice) * g.factor;
    }
    return sum;
}

std::string Dice::str() const {
    std::string text;
    for (auto& g: groups_) {
        text += g.factor.sign() == -1 ? '-' : '+';
        if (g.n_dice > 1)
            text += std::to_string(g.n_dice);
        text += 'd' + std::to_string(g.one_dice.b());
        auto n = std::abs(g.factor.num());
        if (n > 1)
            text += '*' + std::to_string(n);
        auto d = g.factor.den();
        if (d > 1)
            text += '/' + std::to_string(d);
    }
    if (modifier_ > 0)
        text += '+';
    if (modifier_)
        text += modifier_.str();
    if (text[0] == '+')
        text.erase(0, 1);
    if (text.empty())
        text = "0";
    return text;
}

void Dice::insert(integer_type n, integer_type faces, const Rational& factor) {
    static const auto match_terms = [] (const dice_group& g1, const dice_group& g2) noexcept {
        return g1.one_dice.b() == g2.one_dice.b() && g1.factor == g2.factor;
    };
    static const auto sort_terms = [] (const dice_group& g1, const dice_group& g2) noexcept {
        return g1.one_dice.b() == g2.one_dice.b() ? g1.factor < g2.factor : g1.one_dice.b() > g2.one_dice.b();
    };
    if (n < 0 || faces < 0)
        throw std::invalid_argument("Invalid dice");
    if (n > 0 && faces > 0 && factor != 0) {
        dice_group g;
        g.one_dice = distribution_type(1, faces);
        g.n_dice = n;
        g.factor = factor;
        auto it = std::lower_bound(groups_.begin(), groups_.end(), g, sort_terms);
        if (it != groups_.end() && match_terms(*it, g))
            it->n_dice += g.n_dice;
        else
            groups_.insert(it, g);
    }
}
