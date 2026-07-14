/*
** EPITECH PROJECT, 2026
** argHandler_test
** File description:
** basics
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

Test(ArgHandler, progName)
{
    my::ArgHandler arg(avList1.size(), avList1.data());
    cr_assert_str_eq(avList1[0], arg.progName().c_str(), "Invalid program name, expected '%s' but found '%s'.", avList1[0], arg.progName().c_str());
}

Test(ArgHandler, av_size_and_content)
{
    my::ArgHandler arg(avList1.size(), avList1.data());

    cr_assert_eq(avList1.size(), arg.ac(), "Invalid ac, expected %ld but found %ld.", avList1.size(), arg.ac());

    for (size_t i = 0; i < avList1.size(); i++) {
        cr_assert_str_eq(avList1[i], arg.av()[i].c_str(), "Invalid entry(i:%d) in av, expected '%s' but found '%s'.", i, avList1[i], arg.av()[i].c_str());
    }
}

Test(ArgHandler, getArgs)
{
    my::ArgHandler arg(avList1.size(), avList1.data());

    static const
    std::vector<const char *> expectedArgs = {
        "./program name",
        "arg1",
        "arg2"
    };

    std::deque<std::string> __port; //unused
    arg.find("-p", 1, __port); //need to find flags to figure out which args are free, which aren't
    std::deque<std::string> resArgs = arg.getArgs();

    cr_assert_eq(expectedArgs.size(), resArgs.size(), "Invalid number of args, expected %d but found %d.", expectedArgs.size(), resArgs.size());

    for (size_t i = 0; i < expectedArgs.size(); i++) {
        cr_assert_str_eq(expectedArgs[i], resArgs[i].c_str(), "Invalid arg(i:%d), expected '%s' but found '%s'.", i, expectedArgs[i], resArgs[i].c_str());
    }
}

static const
std::vector<const char *> avList2 = {
    "./program name",
    "arg1",
    "-h",
    "arg2",
    "-la",
};

Test(ArgHandler, flags_without_args)
{
    my::ArgHandler arg(avList2.size(), avList2.data());

    cr_assert_neq(arg.find("-l"), arg.npos, "Failure to find flag '-l'.");
    try {
        arg.find("-a", true);
        cr_assert(true);
    } catch (my::ArgHandler::MissingFlag& e) {
        cr_assert_fail("Failure to find flag '-a', what(): '%s'", e.what().c_str());
    }
}