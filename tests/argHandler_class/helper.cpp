/*
** EPITECH PROJECT, 2026
** argHandler_test
** File description:
** helper
*/

#include <criterion/criterion.h>
#include "ArgHandler.hpp"

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
