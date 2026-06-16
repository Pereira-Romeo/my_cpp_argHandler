/*
** EPITECH PROJECT, 2026
** argHandler
** File description:
** ArgHandler
*/

#include "ArgHandler.hpp"
#include <iostream>


//================= Execptions ===================================//

my::ArgHandler::BadFlag::BadFlag(std::string flag, int expected_ac, int real_ac)
: Error("Bad flag '" + flag + "', expected " + std::to_string(expected_ac) + " arguments for this flag but found " + std::to_string(real_ac) + ".", false)
{}

my::ArgHandler::BadFlag::BadFlag(std::string flag)
: Error("Bad flag '" + flag + "'. Cannot group flags that take one or more arguments with other flags.", false)
{}

my::ArgHandler::UnrecognizedFlag::UnrecognizedFlag(std::string flag)
: Error("Unrecognized flag '" + flag + "'.", false)
{}


//================= Constructor ==================================//

my::ArgHandler::ArgHandler(int ac, const char **av)
{
    LOG_DEBUG("arghandler constructor, args are:");
    for (int i = 0; i < ac; i++) {
        _av.emplace_back(av[i]);
        LOG_DEBUG(std::to_string(i) + ": " + _av.back());
    }

    for (int i = 0; i < 2; i++) {
        if (find(((i == 0) ? "-h" : "--help")) != npos) {
            break;
        } else {
        }
    }
}


//================= Basics =======================================//

size_t my::ArgHandler::ac() const noexcept
{
    return _av.size();
}

std::deque<std::string> my::ArgHandler::av() const noexcept
{
    return _av;
}

bool my::ArgHandler::help() const noexcept
{
    try {
        _cache.at("--help").ass.at(0);
        return true;
    } catch (std::out_of_range& e) {
        return false;
    }
}

std::string my::ArgHandler::progName() const noexcept
{
    return _av.at(0);
}


//================= Find =========================================//

size_t my::ArgHandler::find(const std::string& flag)
{
    LOG_DEBUG("find looking for flag: " + flag);
    //check cache first
    try {
        const _flag_t& entry = _cache.at(flag);
        LOG_DEBUG("    found entry in cache about this flag");
        if (entry.ass.size() > 0) {
            LOG_DEBUG("    flag has index: " + std::to_string(entry.ass[0]));
            return entry.ass[0];
        } else {
            LOG_DEBUG("    flag is not present");
            return npos;
        }
    } catch (std::out_of_range& e) {
    }

    //not in cache, looking through _av
    for (size_t i = 0; i < _av.size(); i++) {
        const std::string& arg = _av.at(i);
        LOG_DEBUG("    checking arg");
        if (arg[0] != '-')
            continue;
        if (arg == flag) {
            LOG_DEBUG("    arg(" + std::to_string(i) + ") is perfect match");
            cache(flag, 0, i);
            return i;
        } else if (flag[1] != '-' && arg[1] != '-' && arg.find(flag[1]) != std::string::npos) {
            LOG_DEBUG("    arg(" + std::to_string(i) + ") contains flag at " + std::to_string(arg.find(flag[1])));
            cache(flag, 0, i);
            return i;
        }
    }
    cache(flag, 0, npos);
    LOG_DEBUG("    not found.");
    return npos;
}

size_t my::ArgHandler::find(const std::string& flag, bool& present)
{
    size_t pos = find(flag);
    present = pos != npos;
    return pos;
}

//TODO: badflag, single with argument combined to other flag
size_t my::ArgHandler::find(const std::string& flag, int n, std::deque<std::string>& args)
{
    size_t pos = find(flag);
    if (pos != npos) {
        _flag_t& entry = _cache[flag];

        if (entry.ac == n && entry.ass.size() > 1) {
            LOG_DEBUG("    loading args from cache");
            //fetch from cache, skipping flag's position
            for (size_t i : entry.ass | std::views::drop(1)) {
                args.push_back(_av[i]);
            }

        } else {
            //update cache
            entry.ass.clear();
            entry.ass.push_back(pos);

            size_t max = (n <= -1) ? _av.size(): n;
            entry.ac = n;

            LOG_DEBUG("    find looking for " + flag + " arg extension, max: " + std::to_string(n));
            LOG_DEBUG("    pos of the flag is reported as " + std::to_string(pos));
            for (size_t i = pos + 1; i < pos + 1 + max && i < _av.size(); i++) {
                if (_av[i][0] != '-') {
                    entry.ass.push_back(i);
                    args.push_back(_av[i]);
                    LOG_DEBUG("        Adding arg(" + std::to_string(i) + "): " + _av[i]);
                } else {
                    break;
                }
            }
            LOG_DEBUG("    Stopped gathering args");

            if (n != -1 && entry.ass.size() - 1 != max)
                throw BadFlag(flag, n, entry.ass.size() - 1);
        }
    }
    return pos;
}

size_t my::ArgHandler::find(const std::string& flag, bool& present, int n, std::deque<std::string>& args)
{
    size_t pos = find(flag, n, args);
    present = pos != npos;
    return pos;
}

//================= Misc =========================================//

std::deque<std::string> my::ArgHandler::getArgs() const
{
    std::deque<std::string> args;
    std::deque<size_t> assArg = getAssigned();

    args.push_front(_av[0]);
    for (size_t i = 1; i < _av.size(); i++) {
        if (std::find(assArg.begin(), assArg.end(), i) == assArg.end()) {
            args.push_back(_av[i]);
        }
    }
    return args;
}

std::deque<size_t> my::ArgHandler::getAssigned() const
{
    std::deque<size_t> assignedArgs;

    for (auto entry : _cache) {
        assignedArgs.insert(assignedArgs.end(), entry.second.ass.begin(), entry.second.ass.end());
    }
    LOG_DEBUG("getAssigned results:");
    for (size_t index : assignedArgs) {
        LOG_DEBUG("    - " + std::to_string(index));
    }
    return assignedArgs;
}

std::deque<size_t> my::ArgHandler::getFlagsPos() const
{
    std::deque<size_t> flagsI;

    for (auto entry : _cache) {
        try {
            flagsI.push_back(entry.second.ass.at(0));
        } catch (std::out_of_range& e) {
            continue;
        }
    }
    return flagsI;
}

void my::ArgHandler::tryThrowUnrecognized() const
{
    std::deque<size_t> flagsI = getFlagsPos();
    LOG_DEBUG("Trying to throw unrecognized flags");

    for (size_t i = 1; i < _av.size(); i++) {
        if (_av[i][0] == '-' && std::find(flagsI.begin(), flagsI.end(), i) == flagsI.end()) {
            throw UnrecognizedFlag(_av[i]);
        }
    }
}

//================= caching ======================================//

void my::ArgHandler::cache(const std::string& flag, int ac, size_t index)
{
    _flag_t& entry = _cache[flag];

    LOG_DEBUG("    caching flag " + flag);
    LOG_DEBUG("    flag index: " + std::to_string(index));
    LOG_DEBUG("    flag Present: " + ((index != npos) ? std::string("true"):std::string("false")));
    LOG_DEBUG("    flag arg count: " + std::to_string(ac));
    entry.ac = ac;
    if (index != npos) {
        LOG_DEBUG("        cache confirming flag is present.");
        if (entry.ass.size() == 0) {
            LOG_DEBUG("        assigned list was empty, pushing new index: " + std::to_string(index));
            entry.ass.push_back(index);
        } else {
            LOG_DEBUG("        assigned list not empty, updating from index: " + std::to_string(entry.ass[0]) + " to: " + std::to_string(index));
            entry.ass[0] = index;
        }
    } else {
        entry.ass.clear();
    }
}
