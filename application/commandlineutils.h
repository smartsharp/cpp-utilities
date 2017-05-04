#ifndef APPLICATIONUTILITIES_COMMANDLINEUTILS_H
#define APPLICATIONUTILITIES_COMMANDLINEUTILS_H

#include "../global.h"

#include <ostream>

#ifdef PLATFORM_WINDOWS
#include <memory>
#include <vector>
#endif

namespace ApplicationUtilities {

/*!
 * \brief The Response enum is used to specify the default response for the confirmPrompt() method.
 */
enum class Response { None, Yes, No };

bool CPP_UTILITIES_EXPORT confirmPrompt(const char *message, Response defaultResponse = Response::None);

#ifdef PLATFORM_WINDOWS
void CPP_UTILITIES_EXPORT startConsole();
std::pair<std::vector<std::unique_ptr<char[]>>, std::vector<char *>> CPP_UTILITIES_EXPORT convertArgsToUtf8();
#define CMD_UTILS_START_CONSOLE ::ApplicationUtilities::startConsole();
#define CMD_UTILS_CONVERT_ARGS_TO_UTF8                                                                                                               \
    auto utf8Args = ::ApplicationUtilities::convertArgsToUtf8();                                                                                     \
    argv = utf8Args.second.data();                                                                                                                   \
    argc = static_cast<int>(utf8Args.second.size());
#else
#define CMD_UTILS_START_CONSOLE
#define CMD_UTILS_CONVERT_ARGS_TO_UTF8
#endif

/*!
 * \brief The Indentation class allows printing indentation conveniently, eg. cout << Indentation(4) << ...
 */
class CPP_UTILITIES_EXPORT Indentation {
public:
    Indentation(unsigned char level = 4, char character = ' ')
        : level(level)
        , character(character)
    {
    }

    Indentation operator+(unsigned char level)
    {
        return Indentation(this->level + level, character);
    }

    unsigned char level;
    char character;
};

inline CPP_UTILITIES_EXPORT std::ostream &operator<<(std::ostream &out, Indentation indentation)
{
    for (unsigned char i = 0; i < indentation.level; ++i) {
        out << indentation.character;
    }
    return out;
}

} // namespace ApplicationUtilities

#endif // APPLICATIONUTILITIES_COMMANDLINEUTILS_H
