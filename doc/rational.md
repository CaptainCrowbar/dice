# Rational Number Class #

* _[Ross Smith](mailto:rosssmith@me.com) 2018_
* _Open source under the MIT License_

`Rational` is a simple rational number class. Integers are represented by a
signed `int64_t`. All of the constructors and arithmetic operators will reduce
their result to the lowest possible terms; the denominator is always positive.
Behaviour is undefined if a numerator or denominator goes outside the
representable range of an `int64_t`.

### Member types ###

```c++
using Rational::integer_type = int64_t;
using Rational::real_type = double;
```

Types used in the class.

### Life cycle functions ###

```c++
Rational::Rational() noexcept;
```

The default constructor sets the value to zero.

```c++
Rational::Rational(integer_type n) noexcept;
```

Sets the value to `n/1`.

```c++
Rational::Rational(integer_type num, integer_type den);
```

Sets the value to `num/den`, reduced to the lowest possible terms, and with
the signs adjusted if necessary so the denominator is positive. This will
throw `std::invalid_argument` if the denominator is zero.

```c++
Rational::Rational(const Rational& r);
Rational::Rational(Rational&& r) noexcept;
Rational::~Rational() noexcept;
Rational& Rational::operator=(const Rational& r);
Rational& Rational::operator=(Rational&& r) noexcept;
```

Other life cycle functions.

### Query functions ###

```c++
explicit Rational::operator bool() const noexcept;
```

True if the value is non-zero.

```c++
explicit Rational::operator real_type() const noexcept;
```

Converts the rational number to a floating point number (with the usual
rounding behaviour for floating point division).

```c++
integer_type Rational::num() const noexcept;
integer_type Rational::den() const noexcept;
```

These return the numerator and denominator (after the adjustments described in
the constructor documentation).

```c++
integer_type Rational::int_part() const noexcept;
Rational Rational::frac_part() const noexcept;
```

These split a rational number into its integer part (truncated toward zero)
and fractional part (which will have the same sign as the original value if it
is not zero).

```c++
Rational Rational::abs() const noexcept;
```

Returns the absolute value of a rational number.

```c++
int Rational::sign() const noexcept;
```

Returns the sign of a rational number (-1 if negative, 0 if zero, +1 if
positive).

### Formatting functions ###

```c++
std::string Rational::str() const;
std::ostream& operator<<(std::ostream& out, const Rational& r);
```

These format a rational number as a fraction (e.g. `"22/7"`), or as a simple
integer if it has no fractional part.

```c++
std::string Rational::mixed() const;
```

Formats a rational number in mixed form (e.g. `"3 1/7"`).

### Arithmetic functions ###

```c++
Rational Rational::operator+() const noexcept;
Rational Rational::operator-() const noexcept;
Rational& Rational::operator+=(const Rational& rhs) noexcept;
Rational& Rational::operator-=(const Rational& rhs) noexcept;
Rational& Rational::operator*=(const Rational& rhs) noexcept;
Rational& Rational::operator/=(const Rational& rhs);
Rational operator+(const Rational& lhs, const Rational& rhs) noexcept;
Rational operator-(const Rational& lhs, const Rational& rhs) noexcept;
Rational operator*(const Rational& lhs, const Rational& rhs) noexcept;
Rational operator/(const Rational& lhs, const Rational& rhs);
```

These have their expected behaviour. The division operators will throw
`std::invalid_argument` if the RHS is zero.

### Comparison functions ###

```c++
bool operator==(const Rational& lhs, const Rational& rhs) noexcept;
bool operator!=(const Rational& lhs, const Rational& rhs) noexcept;
bool operator<(const Rational& lhs, const Rational& rhs) noexcept;
bool operator>(const Rational& lhs, const Rational& rhs) noexcept;
bool operator<=(const Rational& lhs, const Rational& rhs) noexcept;
bool operator>=(const Rational& lhs, const Rational& rhs) noexcept;
```

These have their expected behaviour.
