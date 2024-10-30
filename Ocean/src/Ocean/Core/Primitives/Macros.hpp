#pragma once

#include <signal.h>

// Macros

#define ArraySize(array) (sizeof(array) / sizeof((array)[0]))

#define OCEAN_INLINE                                inline
#define OCEAN_FINLINE                               always_inline

#define OCEAN_DEBUG_BREAK                           raise(SIGTRAP);

#define OCEAN_CONCAT_OPERATOR(x, y)                 x y

#define OCEAN_LOCATION                              __FILE_NAME__

#define OCEAN_STRINGIFY(L)                          #L
#define OCEAN_MAKESTRING(L)                         OCEAN_STRINGIFY(L)
#define OCEAN_CONCAT(x, y)                          OCEAN_CONCAT_OPERATOR(x, y)
#define OCEAN_LINE_STRING                           OCEAN_MAKESTRING(__line__)

#define OCEAN_FUNCTIONLINE(Function, Message)       (Function " (" OCEAN_LINE_STRING "): " Message)

#define OCEAN_UNIQUE_SUFFIX(Param)                  OCEAN_CONCAT(Param, __line__)

#define CONSOLE_TEXT_RED(Text)                      ("\x1b[31m" Text "\033[0m")
#define CONSOLE_TEXT_GREEN(Text)                    ("\x1b[32m" Text "\033[0m")
#define CONSOLE_TEXT_YELLOW(Text)                   ("\x1b[33m" Text "\033[0m")
#define CONSOLE_TEXT_BLUE(Text)                     ("\x1b[34m" Text "\033[0m")
#define CONSOLE_TEXT_MAGENTA(Text)                  ("\x1b[35m" Text "\033[0m")
#define CONSOLE_TEXT_CYAN(Text)                     ("\x1b[36m" Text "\033[0m")
#define CONSOLE_TEXT_WHITE(Text)                    ("\x1b[37m" Text "\033[0m")
