/*
** MyLib PROJECT, 2026
** argHandler
** File description:
** ArgHandler
*/

#include "ArgHandler.hpp"
#include <iostream>

namespace my {
//================= Execptions ===================================//

ArgHandler::BadFlag::BadFlag(const std::string& flag, const int expected_ac, const int real_ac)
: Error("Bad flag '" + flag + "', expected " + std::to_string(expected_ac) + " arguments but found " + std::to_string(real_ac) + ".", false)
{}

ArgHandler::BadFlag::BadFlag(const std::string& flag)
: Error("Bad flag '" + flag + "'. Cannot group flags that take one or more arguments with other flags.", false)
{}

ArgHandler::UnrecognizedFlag::UnrecognizedFlag(const std::string& flag)
: Error("Unrecognized flag '" + flag + "'.", false)
{}

ArgHandler::MissingFlag::MissingFlag(const std::string& flag)
: Error("Missing necessary flag '" + flag + "'.", false)
{}

ArgHandler::DuplicateFlag::DuplicateFlag(const std::string& flag)
: Error("flag '" + flag + "' was found more than once but takes arguments.", false)
{}


//================= Constructor ==================================//

ArgHandler::ArgHandler(size_t argc, const char * const *argv, bool short_help)
{
    for (size_t i = 0; i < argc; i++) {
        _av.emplace_back(argv[i]);
    }

    _shortHelp = short_help;
    for (int i = (short_help) ? 0 : 1; i < 2; i++) {
        if (find(((i == 0) ? "-h" : "--help")) != npos) {
            break;
        } else {
        }
    }
}


//================= Basics =======================================//

size_t ArgHandler::ac() const noexcept
{
    return _av.size();
}

std::deque<std::string> ArgHandler::av() const noexcept
{
    return _av;
}

bool ArgHandler::help() const noexcept
{
    try {
        _cache.at("--help").ass.at(0);
        return true;
    } catch (std::out_of_range& e) {
    }
    if (_shortHelp) {
        try {
            _cache.at("-h").ass.at(0);
            return true;
        } catch (std::out_of_range& e) {
        }
    }
    return false;
}

std::string ArgHandler::progName() const noexcept
{
    return _av.at(0);
}


//================= Find =========================================//

size_t ArgHandler::find(const std::string& flag)
{
    //check cache first
    try {
        const _flag_t& entry = _cache.at(flag);
        if (entry.ass.size() > 0) {
            return entry.ass[0];
        } else {
            return npos;
        }
    } catch (std::out_of_range& e) {
    }

    //not in cache, looking through _av
    for (size_t i = 0; i < _av.size(); i++) {
        const std::string& arg = _av.at(i);
        if (arg[0] != '-' || arg.size() < 2)
            continue;
        if (arg == flag) {
            cache(flag, 0, i);
            return i;
        } else if (flag[1] != '-' && arg[1] != '-' && arg.find(flag[1]) != std::string::npos) {
            cache(flag, 0, i);
            return i;
        }
    }
    cache(flag, 0, npos);
    return npos;
}

size_t ArgHandler::find(const std::string& flag, bool necessary)
{
    size_t pos = find(flag);
    if (necessary && pos == npos)
        throw MissingFlag(flag);
    return pos;
}

//TODO: badflag, single with argument combined to other flag
size_t ArgHandler::find(const std::string& flag, int n, std::deque<std::string>& args)
{
    size_t pos = find(flag);
    if (pos != npos) {
        _flag_t& entry = _cache[flag];

        if (entry.ac == n && entry.ass.size() > 1) {
            //fetch from cache, skipping flag's position
            for (size_t i : entry.ass | std::views::drop(1)) {
                args.push_back(_av[i]);
            }

        } else {
            //update cache
            entry.ass.clear();
            entry.ass.push_back(pos);

            if (_av[entry.ass[0]].size() > 2) {
                throw BadFlag(flag);
            }
            size_t max = (n <= -1) ? _av.size(): n;
            entry.ac = n;

            for (size_t i = pos + 1; i < pos + 1 + max && i < _av.size(); i++) {
                if (_av[i][0] != '-') {
                    entry.ass.push_back(i);
                    args.push_back(_av[i]);
                } else {
                    _tryThrowDuplicates(flag, i);
                    break;
                }
            }

            if (n != -1 && entry.ass.size() - 1 != max)
                throw BadFlag(flag, n, entry.ass.size() - 1);
        }
    }
    return pos;
}

size_t ArgHandler::find(const std::string& flag, bool necessary, int n, std::deque<std::string>& args)
{
    size_t pos = find(flag, n, args);
    if (necessary && pos == npos)
        throw MissingFlag(flag);
    return pos;
}

//================= Misc =========================================//

std::deque<std::string> ArgHandler::getArgs() const
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

std::deque<size_t> ArgHandler::getAssigned() const
{
    std::deque<size_t> assignedArgs;

    for (auto entry : _cache) {
        assignedArgs.insert(assignedArgs.end(), entry.second.ass.begin(), entry.second.ass.end());
    }
    return assignedArgs;
}

std::deque<size_t> ArgHandler::getFlagsPos() const
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


void ArgHandler::tryThrowUnrecognized() const
{
    std::deque<size_t> flagsI = getFlagsPos();

    for (size_t i = 1; i < _av.size(); i++) {
        if (_av[i][0] == '-' && std::find(flagsI.begin(), flagsI.end(), i) == flagsI.end()) {
            throw UnrecognizedFlag(_av[i]);
        }
    }
}
void ArgHandler::_tryThrowDuplicates(const std::string& flag, size_t pos) const
{
    for (size_t i = pos; i < _av.size(); i++) {
        if (_av[i][0] != '-' || _av[i].size() < 2)
            continue;
        if (_av[i] == flag) {
            throw DuplicateFlag(flag);
        } else if (flag[1] != '-' && _av[i][1] != '-' && _av[i].find(flag[1]) != std::string::npos) {
            throw DuplicateFlag(flag);
        }
    }
}

//================= caching ======================================//

void ArgHandler::cache(const std::string& flag, int ac, size_t index)
{
    _flag_t& entry = _cache[flag];

    entry.ac = ac;
    if (index != npos) {
        if (entry.ass.size() == 0) {
            entry.ass.push_back(index);
        } else {
            entry.ass[0] = index;
        }
    } else {
        entry.ass.clear();
    }
}

} // namespace my
