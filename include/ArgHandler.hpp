/*
** MyLib PROJECT, 2026
** argHandler
** File description:
** ArgHandler
*/

#pragma once
#include "Logger.hpp"
#include "Error.hpp"
#include <map>
#include <deque>
#include <ranges>
#include <algorithm>

namespace my {
    /** ArgHandler class
     * made to more easily handle your flags :3
     * do note this class is meant for use with the argc and argv of your main
     * and is therefore implemented with this kind of info in mind.
     * repository: https://github.com/Pereira-Romeo/my_cpp_argHandler
     * @note In the processing of flags, there are 2 categories:
     * @note     short (-) (e.g.: -h), which can be combined behind the same '-' (e.g: -la) unless they take arguments
     * @note     long(--) (e.g.: --help), which cannot be combined with any other flag
     * @note This class has an auto managed cache, any flag you find() will be added to cache, wether they are present or not, this allows for multiple side functions and cutting time/compute power cost.
     */
    class ArgHandler {
        public:
            /** exception class that can indicate 2 things:
             * a flag was expecting a certain amount of arguments but found less than that.
             * it can never be more than the expected amount because arguments that would lead to "too many" are simply marked unused and you can get them with getArgs().
             * OR
             * a short(-) flag that takes arguments but was found in a group of short flags (e.g: "-hp").
             */
            class BadFlag: public Error {
                public:
                    BadFlag(const std::string& flag, const int expected_ac, const int real_ac);
                    BadFlag(const std::string& flag);
            };

            /** exception class that can indicate:
             * you used tryThrowUnrecognized() and there was an flag for which you didn't call find() before.
             * @note you shouldn't be able to recieve this error unless you asked for it.
             */
            class UnrecognizedFlag: public Error {
                public:
                    UnrecognizedFlag(const std::string& flag);
            };

            /** exception class that indicates a flag you marked as necessary is missing from the arguments list */
            class MissingFlag: public Error {
                public:
                    MissingFlag(const std::string& flag);
            };

            /** exception class that indicates a flag was found more than once
             * this class can only be caught if you used find() with a flag that has arguments
             */
            class DuplicateFlag: public Error {
                public:
                    DuplicateFlag(const std::string& flag);
            };

            /** Value returned by various member functions when they fail.
             * Also is identical to std::string::npos
             */
            static const size_t npos = std::string::npos;

            ArgHandler() = delete;
            /** ArgHandler class constructor
             * @param ac argument count
             * @param av list og argument
             * @param short_help leave at true if you want "-h" to also be recognized as help flag, set to false if you only want "--help"
             */
            // ArgHandler(int argc, const char **argv, bool short_help = true);
            /** ArgHandler class constructor
             * @param ac argument count
             * @param av list og argument
             * @param short_help leave at true if you want "-h" to also be recognized as help flag, set to false if you only want "--help"
             */
            ArgHandler(size_t argc, const char * const * argv, bool short_help = true);


            //===== Utils ========================================//

            /* get the number of arguments (flags & arguments) */
            size_t ac() const noexcept;
            /* get the list of arguments (flags & arguments) */
            std::deque<std::string> av() const noexcept;

            /** check if the user asked for help
             * this checks for the flags"--help" and "-h" (if -h was kept in constructor)
             * @returns true if user asked for help
             */
            bool help() const noexcept;

            /** get the program's name (argv[0])
             * @returns string
             */
            std::string progName() const noexcept;

            /** get the arguments that aren't used by any flags
             * @note the returned deque will ALWAYS contain the program's name
             */
            std::deque<std::string> getArgs() const;


            //===== find =========================================//

            /** find flag and get it's index
             * @param flag the flag to find
             * @returns index of the flag if found.
             * returns npos if it wasn't found.
             */
            size_t find(const std::string& flag);
            /** find flag and get it's index
             * @param flag the flag to find
             * @param necessary set to true if flag is present, false if not.
             * @returns index of the flag if found.
             * returns npos if it wasn't found.
             * @exception MissingFlag
             */
            size_t find(const std::string& flag, bool necessary);
            /** find flag and get it's index
             * @param flag the flag to find
             * @param n number of arguments this flag should have. -1 to get all args up to the next flag or the end of the list.
             * @param args deque to fill with the arguments of that flag
             * @returns index of the flag if found.
             * @exception BadFlag, see class for information
             * returns npos if it wasn't found.
             * @note if using n = -1, you should check the size of args before using it as it might have a size of 0.
             */
            size_t find(const std::string& flag, int n, std::deque<std::string>& args);
            /** find flag and get it's index
             * @param flag the flag to find
             * @param necessary set to true if flag is present, false if not.
             * @param n number of arguments this flag should have. -1 to get all args up to the next flag or the end of the list.
             * @param args deque to fill with the arguments of that flag
             * @returns index of the flag if found.
             * returns npos if it wasn't found.
             * @exception BadFlag, see class for information
             * @exception MissingFlag, see class for information
             * @note if using n = -1, you should check the size of args before using it as it might have a size of 0.
             */
            size_t find(const std::string& flag, bool necesary, int n, std::deque<std::string>& args);


            //===== Misc =========================================//

            /** cause a throw if there is a flag in the list of arguments that isn't in the cache
             * else does nothing
             * @note a flag is in the cache if you called find() on it.
             * @exception UnrecognizedFlag
             */
            void tryThrowUnrecognized() const;

        private:
            /* list of arguments */
            std::deque<std::string> _av;

            /** function used to cache entries
             * @param flag the flag
             * @param ac argument count of the flag
             * @param index position of the flag inside of av
             * @note trying to cache an already cached flag will update the entry
             */
            void cache(const std::string& flag, int ac, size_t index);

            /** get deque of indexes of arguments that are assigned to flags
             * @note those indexes include the index of the flags* themselves.
             * @note *flags that were added to cache, so you called find() on first.
             */
            std::deque<size_t> getAssigned() const;

            /** like getAssigned but only flag positions
             */
            std::deque<size_t> getFlagsPos() const;

            /** _flag_t struct, used for caching
             * @param ac number of arguments to assign to this flag
             * @param ass index of assigned arguments
             */
            typedef struct _flag_s {
                int ac;
                /* [0] is the position of the flag itself inside _av. others are the pos of corresponding arguments. */
                std::deque<size_t> ass;
            } _flag_t;

            /* cache */
            std::map<std::string, _flag_t> _cache;

            /** cause a throw if there is flag (that takes arguments) which was found more than once
             * @param flag flag to hunt duplicates of
             * @param pos position to start hunt from inside av
             * @exception DuplicateFlag, see class for information
             */
            void _tryThrowDuplicates(const std::string& flag, size_t pos) const;

            /** wether "-h" is recognized as "--help" */
            bool _shortHelp;
    };
} // namespace my
