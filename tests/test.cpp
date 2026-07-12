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


//================= helper test ==================================//

static const
std::vector<const char *> avHelperList1 = {
    "program name",
    "arg1",
    "-h",
    "arg2",
    "-p",
    "8080"
};


static const
std::vector<const char *> avHelperList2 = {
    "program name",
    "arg1",
    "arg2",
    "-p",
    "8080"
};


static const
std::vector<const char *> avHelperList3 = {
    "program name",
    "arg1",
    "-h",
    "arg2",
    "-p",
    "8080"
};


static const
std::vector<const char *> avHelperList4 = {
    "program name",
    "arg1",
    "--help",
    "arg2",
    "-p",
    "8080"
};

static const
std::vector<const char *> avHelperList5 = {
    "program name",
    "arg1",
    "-h",
    "--help",
    "arg2",
    "-p",
    "8080"
};

Test(ArgHandler, short_help_present_and_recognized)
{
    my::ArgHandler arg(avHelperList1.size(), avHelperList1.data());
    cr_assert(arg.help());
}

Test(ArgHandler, short_help_not_present_and_recognized)
{
    my::ArgHandler arg(avHelperList2.size(), avHelperList2.data());
    cr_assert(!arg.help());
}

Test(ArgHandler, short_help_present_and_not_recognized)
{
    my::ArgHandler arg(avHelperList3.size(), avHelperList3.data(), false);
    cr_assert(!arg.help());
}

Test(ArgHandler, long_helper)
{
    my::ArgHandler arg(avHelperList4.size(), avHelperList4.data(), false);
    cr_assert(arg.help());
}

Test(ArgHandler, long_helper_and_short_helper_present_but_short_not_recognized)
{
    my::ArgHandler arg(avHelperList5.size(), avHelperList5.data(), false);
    cr_assert(arg.help());
}
