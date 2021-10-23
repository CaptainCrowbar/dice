#include "dice/dice.hpp"
#include "dice/rational.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char** argv) {

    try {

        std::vector<std::string> args(argv + 1, argv + argc);

        if (args.empty() || args[0] == "-h" || args[0] == "--help") {
            std::cout <<
                "dice [<options>] <pattern> [<number>]\n"
                "    <options> = One or more of:\n"
                "        -d = Show non-integer results in decimal form instead of fractions\n"
                "        -r = Round fractions to the nearest integer\n"
                "        -f = Round fractions down to an integer (floor)\n"
                "        -c = Round fractions up to an integer (ceiling)\n"
                "        -z = Force a non-negative result (results less than 0 are reported as 0)\n"
                "        -p = Force a positive result (results less than 1 are reported as 1)\n"
                "        -h, --help = Print usage information\n"
                "    <pattern> = Dice to roll\n"
                "    <number> = Number of times to roll (default 1)\n";
            return 0;
        }

        bool use_decimal = false;
        bool use_round = false;
        bool use_floor = false;
        bool use_ceil = false;
        bool use_zero = false;
        bool use_positive = false;

        while (! args.empty() && args[0][0] == '-') {
            for (char c: args[0].substr(1)) {
                switch (c) {
                    case 'd':  use_decimal = true; break;
                    case 'r':  use_round = true; break;
                    case 'f':  use_floor = true; break;
                    case 'c':  use_ceil = true; break;
                    case 'z':  use_zero = true; break;
                    case 'p':  use_positive = true; break;
                    default:   throw std::invalid_argument("Invalid flags: " + args[0]);
                }
            }
            args.erase(args.begin());
        }

        if (int(use_decimal) + int(use_round) + int(use_floor) + int(use_ceil) > 1)
            throw std::invalid_argument("Only one of the -d, -r, -f, and -c flags can be used");
        if (int(use_zero) + int(use_positive) > 1)
            throw std::invalid_argument("Only one of the -z and -p flags can be used");
        if (args.empty())
            throw std::invalid_argument("No dice pattern was supplied");
        if (args.size() > 2)
            throw std::invalid_argument("Too many arguments");
        if (args.size() == 2 && args[1].find_first_not_of("0123456789") != std::string::npos)
            throw std::invalid_argument("Invalid number of rolls: " + args[1]);

        Dice dice(args[0]);
        long number = 1;
        if (args.size() == 2)
            number = std::strtol(args[1].data(), nullptr, 10);
        auto seed = std::random_device()();
        std::mt19937 rng(seed);

        for (long i = 1; i <= number; ++i) {
            auto result = dice(rng);
            if (use_round)
                result = result.round();
            else if (use_floor)
                result = result.floor();
            else if (use_ceil)
                result = result.ceil();
            if (use_zero && result < 0)
                result = 0;
            else if (use_positive && result < 1)
                result = 1;
            if (number > 1)
                std::cout << i << ": ";
            if (use_decimal)
                std::cout << double(result) << "\n";
            else
                std::cout << result << "\n";
        }

        return 0;

    }

    catch (const std::exception& ex) {
        std::cerr << "*** " << ex.what() << "\n";
        return 1;
    }

}
