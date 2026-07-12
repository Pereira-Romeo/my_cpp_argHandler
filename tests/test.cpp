/*
** EPITECH PROJECT, 2026
** argHandler
** File description:
** test
*/

#include <criterion/criterion.h>
#include "ArgHandler.hpp"

static const
std::vector<const char *> avList1 = {
    "program name",
    "arg1",
    "-h",
    "arg2",
    "-p",
    "8080"
};

Test(ArgHandler, flags_with_arguments)
{
    my::ArgHandler arg(avList1.size(), avList1.data());
    std::deque<std::string> port;
    cr_assert_neq(arg.find("-p", 1, port), arg.npos);

    cr_assert_eq(port.size(), 1);
    cr_assert_str_eq(port.at(0).c_str(), "8080");
}
