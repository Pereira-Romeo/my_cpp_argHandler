/*
** EPITECH PROJECT, 2026
** argHandler
** File description:
** errors
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

Test(ArgHandler, missing_flags_exceptions)
{
    my::ArgHandler arg(avList1.size(), avList1.data());

    try {
        arg.find("-a"); //just looking for it twice to use the cache
        arg.find("-a", true);
        cr_assert_fail("Invalid find, successfully found missing flag '-a'.");
    } catch (my::ArgHandler::MissingFlag& e) {
        cr_assert(true);
    } catch (std::exception& e) {
        cr_assert_fail("bad exception caught, what(): '%s'.", e.what());
    }
    try {
        arg.find("--nuhuh", true);
        cr_assert_fail("Invalid find, successfully found missing flag '--nuhuh'.");
    } catch (my::ArgHandler::MissingFlag& e) {
        cr_assert(true);
    } catch (std::exception& e) {
        cr_assert_fail("bad exception caught, what(): '%s'.", e.what());
    }
    try {
        std::deque<std::string> __garbadge;
        arg.find("-b", true, -1, __garbadge);
        cr_assert_fail("Invalid find, successfully found missing flag '-b'.");
    } catch (my::ArgHandler::MissingFlag& e) {
        cr_assert(true);
    } catch (std::exception& e) {
        cr_assert_fail("bad exception caught, what(): '%s'.", e.what());
    }
    try {
        std::deque<std::string> __garbadge;
        arg.find("--bbbbb", true, -1, __garbadge);
        cr_assert_fail("Invalid find, successfully found missing flag '--bbbbb'.");
    } catch (my::ArgHandler::MissingFlag& e) {
        cr_assert(true);
    } catch (std::exception& e) {
        cr_assert_fail("bad exception caught, what(): '%s'.", e.what());
    }
}

static const
std::vector<const char *> avList2 = {
    "program name",
    "arg1",
    "-h",
    "arg2",
    "-pa",
    "8080"
};



Test(ArgHandler, bad_flags_exceptions1)
{
    my::ArgHandler arg(avList2.size(), avList2.data());

    try {
        std::deque<std::string> __garbadge;
        arg.find("-p", 1, __garbadge);
        cr_assert_fail("Invalid find, successfully found bad flag '-p' inside '-pa'.");
    } catch (my::ArgHandler::BadFlag& e) {
        cr_assert(true);
    } catch (std::exception& e) {
        cr_assert_fail("bad exception caught, what(): '%s'.", e.what());
    }
}

Test(ArgHandler, bad_flags_exceptions2)
{
    my::ArgHandler arg(avList1.size(), avList1.data());

    try {
        std::deque<std::string> __garbadge;
        arg.find("-p", 2, __garbadge);
        cr_assert_fail("Invalid find, successfully found bad flag '-p' that is missing an argument.");
    } catch (my::ArgHandler::BadFlag& e) {
        cr_assert(true);
    } catch (std::exception& e) {
        cr_assert_fail("bad exception caught, what(): '%s'.", e.what());
    }
}

Test(ArgHandler, unrecognizedFlag_exceptions)
{
    my::ArgHandler arg(avList1.size(), avList1.data());

    try {
        arg.find("-a");
        arg.tryThrowUnrecognized();
    } catch (my::ArgHandler::UnrecognizedFlag& e) {
        cr_assert(true);
    } catch (std::exception& e) {
        cr_assert_fail("bad exception caught, what(): '%s'.", e.what());
    }
}
