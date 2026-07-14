/*
** EPITECH PROJECT, 2026
** argHandler_test
** File description:
** cache
*/

#include <criterion/criterion.h>
#include "ArgHandler.hpp"

static const
std::vector<const char *> avList1 = {
    "./program name",
    "arg1",
    "-h",
    "arg2",
    "-p",
    "8080"
};

Test(ArgHandler, caching1)
{
    my::ArgHandler arg(avList1.size(), avList1.data());
    std::deque<std::string> port;
    cr_assert_neq(arg.find("-p", 1, port), arg.npos, "Failure to find flag '-p'.");

    cr_assert_eq(port.size(), 1, "Invalid number of arguments for port, expected 1 but found %ld.", port.size());
    cr_assert_str_eq(port.at(0).c_str(), "8080", "Invalid argument for port, expected 8080 but found %s.", port.at(0).c_str());

    //try again to use cache
    std::deque<std::string> portCache;
    cr_assert_neq(arg.find("-p", 1, portCache), arg.npos, "Failure to find flag '-p' from cache.");

    cr_assert_eq(portCache.size(), 1, "Invalid number of arguments for port, expected 1 but found %ld.", portCache.size());
    cr_assert_str_eq(portCache.at(0).c_str(), "8080", "Invalid argument for port, expected 8080 but found %s.", portCache.at(0).c_str());
}
