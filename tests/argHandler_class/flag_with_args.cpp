/*
** EPITECH PROJECT, 2026
** argHandler_test
** File description:
** flag_with_args
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

Test(ArgHandler, flag_with_arguments)
{
    my::ArgHandler arg(avList1.size(), avList1.data());
    std::deque<std::string> port;
    cr_assert_neq(arg.find("-p", true, 1, port), arg.npos, "Failure to find flag '-p'.");

    cr_assert_eq(port.size(), 1, "Invalid number of arguments for port, expected 1 but found %ld.", port.size());
    cr_assert_str_eq(port.at(0).c_str(), "8080", "Invalid argument for port, expected 8080 but found %s.", port.at(0).c_str());
}

static const
std::vector<const char *> avList2 = {
    "program name",
    "arg1",
    "-h",
    "arg2",
    "-p",
    "8080",
    "-t",
    "team1",
    "team2",
    "team3"
};

Test(ArgHandler, two_flags_with_args)
{
    my::ArgHandler arg(avList2.size(), avList2.data());

    std::deque<std::string> port;
    cr_assert_neq(arg.find("-p", 1, port), arg.npos, "Failure to find flag '-p'.");
    cr_assert_eq(port.size(), 1, "Invalid number of arguments for port, expected 1 but found %ld.", port.size());
    cr_assert_str_eq(port.at(0).c_str(), "8080", "Invalid argument for port, expected 8080 but found %s.", port.at(0).c_str());

    static const
    std::vector<const char *> expectedTeams = {
        "team1",
        "team2",
        "team3"
    };

    std::deque<std::string> teams;
    cr_assert_neq(arg.find("-t", -1, teams), arg.npos, "Failure to find flag '-p'.");
    cr_assert_eq(teams.size(), expectedTeams.size(), "Invalid number of arguments for teams, expected %ld but found %ld.", expectedTeams.size(), teams.size());
    for (size_t i = 0; i < expectedTeams.size(); i++) {
        cr_assert_str_eq(expectedTeams[i], teams[i].c_str(), "Invalid team(i:%d), expected '%s' but found '%s'.", i, expectedTeams[i], teams[i].c_str());
    }
}
