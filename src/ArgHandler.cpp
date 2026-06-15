/*
** EPITECH PROJECT, 2026
** argHandler
** File description:
** ArgHandler
*/

#include "../include/ArgHandler.hpp"
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
    std::cout << "argHandler constructor, args are:" << std::endl;
    for (int i = 0; i < ac; i++) {
        _av.emplace_back(av[i]);
        std::cout << i << ": " + _av.back() << std::endl;
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
        if (_cache.at("--help").present) {
            return true;
        } else {
            return false;
        }
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
    std::cout << "find looking for flag " << flag << std::endl;
    //check cache first
    try {
        const _flag_t& entry = _cache.at(flag);
        std::cout << "    flag found in cache." << std::endl;
        if (entry.present) {
            std::cout << "    flag is at index: " << entry.ass[0] << std::endl;
            return entry.ass[0];
        } else {
            std::cout << "    flag is not present: " << entry.ass[0] << std::endl;
            return npos;
        }
    } catch (std::out_of_range& e) {
    }

    //not in cache, looking through _av
    for (size_t i = 0; i < _av.size(); i++) {
        const std::string& arg = _av.at(i);
        std::cout << "    checking arg " << arg << std::endl;
        if (arg[0] != '-')
            continue;
        if (arg == flag) {
            std::cout << "    arg(" << i << ") is perfect match" << std::endl;
            cache(flag, true, 0, i);
            return i;
        } else if (flag[1] != '-' && arg[1] != '-' && arg.find(flag[1]) != std::string::npos) {
            std::cout << "    arg(" << i << ") contains flag at " << arg.find(flag[1]) << std::endl;
            cache(flag, true, 0, i);
            return i;
        }
    }
    cache(flag, false, 0, 0);
    std::cout << "    not found." << std::endl;
    return npos;
}

size_t my::ArgHandler::find(const std::string& flag, bool& present)
{
    size_t pos = find(flag);
    present = pos != npos;
    return pos;
}

//TODO: add retrieving args from cache if ass.size() > 1
size_t my::ArgHandler::find(const std::string& flag, int n, std::deque<std::string>& args)
{
    size_t pos = find(flag);
    if (pos != npos) {
        std::cout << "pos of the flag is reported as " << pos << std::endl;
        _flag_t& entry = _cache[flag];
        entry.ac = n;
        size_t max = (n <= -1) ? _av.size(): n;

        std::cout << "find looking for " + flag + " arg extension, max: " << n << std::endl;
        std::cout << "pos of the flag is reported as " << pos << std::endl;
        for (size_t i = pos; i < pos + max && i < _av.size(); i++) {
            std::cout << "    checking arg n°" << i << std::endl;
            if (_av[i][0] != '-') {
                entry.ass.push_back(i);
                args.push_back(_av[i]);
                std::cout << "    adding to list" << std::endl;
            } else {
                break;
            }
        }
        std::cout << "stopped gathering args" << std::endl;
        if (n != -1 && entry.ass.size() - 1 != max)
            throw BadFlag(flag, n, entry.ass.size() - 1);
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

std::deque<std::string>& my::ArgHandler::getArgs() const
{

}

void my::ArgHandler::tryThrowUnrecognized() const
{
    // std::cout << "try throw unrecognized flags" << std::endl;
    // for (std::string arg : _av) {
    //     try {
    //         std::cout << "checking arg: " << arg << std::endl;
    //         if (arg[0] != '-') {
    //             std::cout << "    not a flag, skipping." << std::endl;
    //             continue;
    //         } else if (arg[1] == '-') {
    //             std::cout << "    long flag" << std::endl;
    //             _flags.at(arg);
    //         } else {
    //             std::cout << "    short flag" << std::endl;
    //             for (size_t i = 1; i < arg.size(); i++) {
    //                 std::cout << "        trying " << "-" + arg[i] << std::endl;
    //                 _flags.at("-" + arg[i]);
    //             }
    //         }
    //     } catch (std::out_of_range& e) {
    //         throw UnrecognizedFlag(arg);
    //     }
    // }
}

//================= caching ======================================//

void my::ArgHandler::cache(const std::string& flag, bool present, int ac, size_t index)
{
    _flag_t& entry = _cache[flag];

    std::cout << "    caching flag " + flag << std::endl;
    std::cout << "        flag present: " << present << std::endl;
    std::cout << "        flag arg count: " << ac << std::endl;
    std::cout << "        flag index: " << index << std::endl;
    entry.present = present;
    entry.ac = ac;
    if (present) {
        std::cout << "        cache confirming flag is present." << std::endl;
        if (entry.ass.size() == 0) {
            std::cout << "        assigned list was empty, pushing new index: " << index << std::endl;
            entry.ass.push_back(index);
        } else {
            std::cout << "        assigned list not empty, updating from index: " << entry.ass[0] << " to: "<< index << std::endl;
            entry.ass[0] = index;
        }
    } else {
        entry.ass.clear();
    }
}
