# C++ Dice Roller Class #

* _[Ross Smith](mailto:rosssmith@me.com) 2018_
* _Open source under the MIT License_

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

Also included is a basic rational number class, to allow dice rolls to yield
fractional results without truncation.

### Documentation ###

* [Dice Class](doc/dice.md)
* [Rational Class](doc/rational.md)
