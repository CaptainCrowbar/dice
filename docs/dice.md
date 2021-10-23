# Dice Roller Class

* _© Ross Smith 2021_
* _Open source under the Boost License_

The `Dice` class is a random number distribution that emulates rolling
multiple dice of different kinds, and applying some limited arithmetic to the
results. A set of dice can be specified either by building it up from
instances of the `Dice` class, or by parsing a string in a conventional
format.

Examples:

```c++
// These both produce the same distribution
auto dice1 = Dice(2, 10) * 5 + Dice(3, 6) + 10;
auto dice2 = Dice("2d10x5 + 3d6 + 10");
```

## Contents ##

* TOC
{:toc}

## Dice class ##

### Member types ###

```c++
using Dice::integer_type = int64_t;
using Dice::real_type = double;
using Dice::result_type = Rational;
```

Types used in the class. `Rational` is the output type of the generator.

### Life cycle functions ###

```c++
Dice::Dice() noexcept;
```

Creates a null dice roller, which always yields zero.

```c++
explicit Dice::Dice(integer_type n, integer_type faces = 6, const Rational& factor = 1);
```

Creates an object that rolls `n` dice, each numbered from 1 to `faces`,
optionally multiplying the result by `factor`. This will produce a null
(always zero) dice roller if any of the arguments is zero; it will throw
`std::invalid_argument` if `n` or `faces` is negative.

```c++
explicit Dice::Dice(std::string_view str);
```

Creates a dice roller by parsing the string representation.

A dice representation consists of any number of dice groups or numeric
modifiers. A dice group can consist of an optional number of dice (1 by
default), the letter `D` (case insensitive), and an optional number of faces
(6 by default). For example, `"3d6"` means "roll three six-sided dice and add
the results" (and could also be written `"3D"`).

A group can be preceded or followed by an integer multiplier, delimited by
either a star or the letter `X` (case insensitive), and followed by a divisor,
delimited by a slash. For example, `"3d6x2/3"` means "roll 3d6 and multiply
the result by 2/3". The returned value always keeps fractions intact, it does
not round to an integer.

The string can also add or subtract constant integers or fractions. For
example, `"3d6+10"` means "roll 3d6 and add 10" (the modifier does not have to
be at the end; `"10+3d6"` is equally valid).

White space is not significant. More complicated arithmetic, such as anything
that would require parentheses, is not supported. This constructor will throw
`std::invalid_argument` if the expression is not a valid dice specification
according to the above rules, or if it requires division by zero.

```c++
Dice::Dice(const Dice& d);
Dice::Dice(Dice&& d) noexcept;
Dice::~Dice() noexcept;
Dice& Dice::operator=(const Dice& d);
Dice& Dice::operator=(Dice&& d) noexcept;
```

Other life cycle functions.

### Generator function ###

```c++
template <typename RNG> Rational Dice::operator()(RNG& rng);
```

The main generator function. The `RNG` class can be any standard conforming
random number engine.

### Arithmetic functions ###

```c++
Dice Dice::operator+() const;
Dice Dice::operator-() const;
Dice& Dice::operator+=(const Dice& rhs);
Dice& Dice::operator+=(const Rational& rhs);
Dice& Dice::operator-=(const Dice& rhs);
Dice& Dice::operator-=(const Rational& rhs);
Dice& Dice::operator*=(const Rational& rhs);
Dice& Dice::operator/=(const Rational& rhs);
Dice operator+(const Dice& lhs, const Dice& rhs);
Dice operator+(const Dice& lhs, const Rational& rhs);
Dice operator+(const Rational& lhs, const Dice& rhs);
Dice operator-(const Dice& lhs, const Dice& rhs);
Dice operator-(const Dice& lhs, const Rational& rhs);
Dice operator-(const Rational& lhs, const Dice& rhs);
Dice operator*(const Dice& lhs, const Rational& rhs);
Dice operator*(const Rational& lhs, const Dice& rhs);
Dice operator/(const Dice& lhs, const Rational& rhs);
```

Operations that modify or combine two sets of dice, or a set of dice and a
numeric factor. Addition and subtraction simply combine groups of dice
together, in the same way as the plus and minus operators in the string
format. Multiplication or division by a rational number multiplies or divides
the result of future rolls by that number.

The division operators will throw `std::invalid_argument` if the RHS is zero.

### Statistical functions ###

```c++
Rational Dice::mean() const noexcept;
Rational Dice::variance() const noexcept;
real_type Dice::sd() const noexcept;
Rational Dice::min() const noexcept;
Rational Dice::max() const noexcept;
```

These return statistical properties of the dice roll results.

### Formatting functions ###

```c++
std::string Dice::str() const;
std::ostream& operator<<(std::ostream& out, const Dice& d);
```

These format the collection of dice stored in the object, in the same format
as described for the constructor. Because the string is being reconstructed
from the stored properties of the distribution, the result may not exactly
match the original string supplied to the constructor, but will be
functionally equivalent.

### Custom literals ###

```c++
Dice operator""_d4(unsigned long long n);
Dice operator""_d6(unsigned long long n);
Dice operator""_d8(unsigned long long n);
Dice operator""_d10(unsigned long long n);
Dice operator""_d12(unsigned long long n);
Dice operator""_d20(unsigned long long n);
Dice operator""_d100(unsigned long long n);
```

Literals for some commonly used dice. For example, `3_d6` is equivalent to
`Dice(3,6)` or `Dice("3d6")`.
