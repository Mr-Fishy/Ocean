#pragma once

#include <signal.h>

// Macros

#define ArraySize(array) (sizeof(array) / sizeof((array)[0]))

/** @brief Inline descriptor. */
#define OCEAN_INLINE                                inline
/** @brief Force inline descriptor. */
#define OCEAN_FINLINE                               always_inline

#define OCEAN_EXP                                   extern

#if defined(SIGTRAP)

    #define OCEAN_DEBUG_BREAK                       raise(SIGTRAP)

#else

    #define OCEAN_DEBUG_BREAK                       raise(SIGABRT)

#endif

/** @brief Concatenate two strings */
#define OCEAN_CONCAT_OPERATOR(x, y)                 x y

/** @brief Get's the filename in the location. */
#define OCEAN_LOCATION                              __FILE_NAME__

/** @brief Get's the function name in the location. */
#define OCEAN_FUNCTION                              "__func__"

/** @brief Stringify's the given constant. */
#define OCEAN_STRINGIFY(L)                          #L
#define OCEAN_MAKESTRING(L)                         OCEAN_STRINGIFY(L)
#define OCEAN_CONCAT(x, y)                          OCEAN_CONCAT_OPERATOR(x, y)
#define OCEAN_LINE_STRING                           OCEAN_MAKESTRING(__LINE__)

#define OCEAN_FUNCTIONLINE(Message)                 OCEAN_LOCATION " (" OCEAN_LINE_STRING "): " OCEAN_MAKESTRING(Message)

#define OCEAN_UNIQUE_SUFFIX(Param)                  OCEAN_CONCAT(Param, __LINE__)

#define CONSOLE_TEXT_RED(Text)                      ("\x1b[31m" Text "\033[0m")
#define CONSOLE_TEXT_GREEN(Text)                    ("\x1b[32m" Text "\033[0m")
#define CONSOLE_TEXT_YELLOW(Text)                   ("\x1b[33m" Text "\033[0m")
#define CONSOLE_TEXT_BLUE(Text)                     ("\x1b[34m" Text "\033[0m")
#define CONSOLE_TEXT_MAGENTA(Text)                  ("\x1b[35m" Text "\033[0m")
#define CONSOLE_TEXT_CYAN(Text)                     ("\x1b[36m" Text "\033[0m")
#define CONSOLE_TEXT_WHITE(Text)                    ("\x1b[37m" Text "\033[0m")

#define OC_UNUSED [[maybe_unused]]
