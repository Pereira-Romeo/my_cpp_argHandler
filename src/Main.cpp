/*
** Mylib PROJECT, 2026
** argHandler
** File description:
** Main
*/

#include "ArgHandler.hpp"
#include <iostream>

static
int helper(int val)
{
    std::cout << "USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq" << std::endl;
    return val;
}

int main(int argc, const char **argv)
{
    try {
        my::ArgHandler args(argc, argv);
        if (args.help())
            return helper(0);
        std::deque<std::string> flag_args;
        args.find("-p", true, 1, flag_args);
        args.find("-x", true, 1, flag_args);
        args.find("-y", true, 1, flag_args);
        args.find("-c", true, 1, flag_args);
        args.find("-f", true, 1, flag_args);
        args.find("-n", true, -1, flag_args);
        std::cout << "port: " << flag_args.at(0) << std::endl;
        std::cout << "width: " << flag_args.at(1) << std::endl;
        std::cout << "height: " << flag_args.at(2) << std::endl;
        std::cout << "clientsNb: " << flag_args.at(3) << std::endl;
        std::cout << "freq: " << flag_args.at(4) << std::endl;

        if (flag_args.size() < 6) {
            throw my::Error("missing arguments for flag -n", false);
        }
        std::cout << "teams are:" << std::endl;
        for (size_t i = 5; i < flag_args.size(); i++) {
            std::cout << "- " + flag_args[i] << std::endl;
        }

        args.tryThrowUnrecognized();

        std::cout << args.progName() << ": Done executing :3" << std::endl;
        return 0;
    } catch (my::Error& e) {
        std::cerr << "\033[1;31mFatal error\033[0m: " + e.what() << std::endl;
        return e.value();
    } catch (std::exception& e) {
        std::cerr << "\033[1;31mFatal error\033[0m: " << e.what() << "\nLast registered errno(" << errno << "): " << ((errno != 0) ? std::strerror(errno) : "No registered errno.") << std::endl;
    } catch (...) {
        std::cerr << "\033[1;31mCaught error\033[0m: " << ((errno != 0) ? std::strerror(errno) : "No registered errno.") << std::endl;
    }
    return 84;
}
