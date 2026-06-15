/*
** EPITECH PROJECT, 2026
** argHandler
** File description:
** Main
*/

#include "../include/ArgHandler.hpp"
#include <iostream>

static
int helper(int val)
{
    std::cout << "This is a main to test the ArgHandler class" << std::endl;
    return val;
}

int main(int argc, const char **argv)
{
    try {
        my::ArgHandler ah(argc, argv);
        if (ah.help())
            return helper(0);
        std::cout << "prog: " + ah.progName() << std::endl;
        std::cout << "searching flag -b, " << ((ah.find("-b") == ah.npos) ? "not found": "found at " + std::to_string(ah.find("-b"))) << std::endl;
        std::deque<std::string> args_p;
        ah.find("-p", 1, args_p);
        for (auto arg : args_p) {
            std::cout << "- " + arg << std::endl;
        }
        return 0;
    } catch (my::Error& e) {
        std::cerr << "\033[1;31mFatal error\033[0m: " + e.what() << std::endl;
        return e.value();
    }
    // catch (std::exception& e) {
    //     std::cerr << "\033[1;31mFatal error\033[0m: " << e.what() << "\nLast registered errno(" << errno << "): " << ((errno != 0) ? std::strerror(errno) : "No registered errno.") << std::endl;
    // } catch (...) {
    //     std::cerr << "\033[1;31mCaught error\033[0m: " << ((errno != 0) ? std::strerror(errno) : "No registered errno.") << std::endl;
    // }
    return 84;
}
