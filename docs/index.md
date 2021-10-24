# C++ Dice Roller #

* _© Ross Smith 2021_
* _Open source under the Boost License_

This contains:

* `dice` - a command line application for generating dice rolls
* [Dice](dice.html) - the C++ class that implements a dice roller
* [Rational](rational.html) - a simple rational number class

Usage of the `dice` command:

```
dice [<options>] <pattern> [<number>]
    <options> = One or more of:
        -g = Show grand total
        -d = Show non-integer results as decimal instead of fraction
        -r = Round fractions to the nearest integer
        -f = Round fractions down to an integer (floor)
        -c = Round fractions up to an integer (ceiling)
        -z = Force a non-negative result (results <0 reported as 0)
        -p = Force a positive result (results <1 reported as 1)
        -h, --help = Print usage information
    <pattern> = Dice to roll
    <number> = Number of times to roll (default 1)
```

A dice pattern consists of any number of dice groups or numeric modifiers. A
dice group can consist of an optional number of dice (1 by default), the
letter `D` (case insensitive), and an optional number of faces(6 by default).
For example, `3d6` means "roll three six-sided dice and add the results"
(and could also be written `3D`).

A group can be preceded or followed by an integer multiplier, delimited by
either a star or the letter `X` (case insensitive), and followed by a
divisor, delimited by a slash. For example, `3d6x2/3` means "roll `3d6` and
multiply the result by 2/3".

The pattern can also add or subtract constant integers or fractions. For
example, `3d6+10` means "roll `3d6` and add 10" (the modifier does not have
to be at the end; `10+3d6` is equally valid).

By default, fractions are kept in the output; optional flags can be used to
control rounding.

White space is not significant (but must be quoted). More complicated
arithmetic, such as anything that would require parentheses, is not
supported.
