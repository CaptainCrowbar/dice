#include "dice/dice.hpp"
#include "dice/rational.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {

    try {

        std::vector<std::string> args(argv + 1, argv + argc);

        // TODO

        return 0;

    }

    catch (const std::exception& ex) {
        std::cerr << "*** " << ex.what() << "\n";
        return 1;
    }

}
