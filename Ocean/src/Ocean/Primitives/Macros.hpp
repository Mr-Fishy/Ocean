#pragma once

#include "Ocean/Platform/PlatformBase.hpp"

#include <signal.h>

/** @brief Marks a function parameter as possibly unused. Useful for silencing warnings between OC_DEBUG and OC_RELEASE builds. */
#define OC_UNUSED                                [[maybe_unused]]

/** @brief Marks a function to not be discarded by the compiler given that the compiler may attempt to optimize the function out. */
#define OC_NO_DISCARD                            [[nodiscard]]



/** @brief Marks a function as noexcept. */
#define OC_NO_EXCEPT                             noexcept
/** @brief Marks a function as noexcept with the given expression. */
#define OC_NO_EXCEPT_E(expression)               noexcept(expression)

/** @brief Marks a function as inline. */
#define OC_INLINE                                inline

#ifdef OC_PLATFORM_WINDOWS

    /** @brief Marks a function to force inline. (WIN32 implementation). */
    #define OC_FINLINE                               __forceinline

#elif defined(OC_PLATFORM_LINUX)

    /** @brief Marks a function to force inline. (Linux implementation). */
    #define OC_FINLINE                               __attribute__((always_inline))

#endif

/** @brief Marks a function as an inline constexpr (const expression). */
#define OC_INLINE_EXPR                           inline constexpr

/** @brief Marks a function as static. */
#define OC_STATIC                                static
/** @brief Marks a function as static and inline. */
#define OC_STATIC_INLINE                         static inline
/** @brief Marks a function as static and constexpr (const expression). */
#define OC_STATIC_EXPR                           static constexpr

/** @brief Marks a function as extern. E.g. an external definition. */
#define OC_EXTERN                                extern



#if defined(SIGTRAP)

    /** @brief SIGTRAP debug break. */
    #define OCEAN_DEBUG_BREAK                       raise(SIGTRAP)

#else

    /** @brief SIGABRT debug break fallback. */
    #define OCEAN_DEBUG_BREAK                       raise(SIGABRT)

#endif



/** @brief Macro to concatenate two strings */
#define OCEAN_CONCAT_OPERATOR(x, y)                 x y

/** @brief Macro to get the filename in the location. */
#define OCEAN_LOCATION                              __FILE_NAME__

/** @brief Macro to get the function name in the location. */
#define OCEAN_FUNCTION                              "__func__"

/** @brief Macro that stringify's the given constant. */
#define OCEAN_STRINGIFY(L)                          #L
/** @brief Macro protection of OCEAN_STRINGIFY. */
#define OCEAN_MAKESTRING(L)                         OCEAN_STRINGIFY(L)
/** @brief Macro to concatenate two strings. */
#define OCEAN_CONCAT(x, y)                          OCEAN_CONCAT_OPERATOR(x, y)
/** @brief Macro to get the line string at the location. */
#define OCEAN_LINE_STRING                           OCEAN_MAKESTRING(__LINE__)

/** @brief Macro to format a message with a function line. */
#define OCEAN_FUNCTIONLINE(Message)                 OCEAN_LOCATION " (" OCEAN_LINE_STRING "): " OCEAN_MAKESTRING(Message)

/** @brief Macro to make a unique suffix based on the given parameter and the call line. */
#define OCEAN_UNIQUE_SUFFIX(Param)                  OCEAN_CONCAT(Param, __LINE__)



/** @brief Macro to format console output as red text. */
#define CONSOLE_TEXT_RED(Text)                      ("\x1b[31m" Text "\033[0m")
/** @brief Macro to format console output as green text. */
#define CONSOLE_TEXT_GREEN(Text)                    ("\x1b[32m" Text "\033[0m")
/** @brief Macro to format console output as yellow text. */
#define CONSOLE_TEXT_YELLOW(Text)                   ("\x1b[33m" Text "\033[0m")
/** @brief Macro to format console output as blue text. */
#define CONSOLE_TEXT_BLUE(Text)                     ("\x1b[34m" Text "\033[0m")
/** @brief Macro to format console output as magent text. */
#define CONSOLE_TEXT_MAGENTA(Text)                  ("\x1b[35m" Text "\033[0m")
/** @brief Macro to format console output as cyan text. */
#define CONSOLE_TEXT_CYAN(Text)                     ("\x1b[36m" Text "\033[0m")
/** @brief Macro to format console output as white text. */
#define CONSOLE_TEXT_WHITE(Text)                    ("\x1b[37m" Text "\033[0m")



/** @brief Macro to define the copy constructor and assignment operator as delete. */
#define OC_NO_COPY(Type)                            Type(const Type&) = delete;\
                                                    Type& operator = (const Type&) = delete

/** @brief Macro to define the move constructor and move operator as delete. */
#define OC_NO_MOVE(Type)                            Type(const Type&&) = delete;\
                                                    Type& operator = (const Type&&) = delete

/** @brief Macro to define a given class or struct as a singleton using a thread-safe method. */
#define OC_SINGLETON(Type)                          OC_STATIC_INLINE Type& Get() {\
                                                        static Type* instance = nullptr;\
                                                        static std::once_flag initFlag;\
                                                    \
                                                        std::call_once(initFlag, []() {\
                                                            instance = new Type();\
                                                        });\
                                                    \
                                                        return *instance;\
                                                    }

/** @brief Get the size of an array. */
#define ArraySize(array) (sizeof(array) / sizeof((array)[0]))
