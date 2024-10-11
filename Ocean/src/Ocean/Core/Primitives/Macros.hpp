#pragma once

#if !defined(_MSC_VER)
	#include <signal.h>
#endif

// Macros

#define ArraySize(array) (sizeof(array) / sizeof((array)[0]))

#if defined (_MSC_VER)

	#define OCEAN_INLINE                            inline
	#define OCEAN_FINLINE                           __forceinline
	
	#define OCEAN_DEBUG_BREAK                       __debugbreak();
	
	#define OCEAN_DISABLE_WARNING(warning_number)   __pragma(warning(disable : warning_number))
	
	#define OCEAN_CONCAT_OPERATOR(x, y)             x##y

	#define OCEAN_LOCATION                          __FILE__

#else

	#define OCEAN_INLINE                            inline
	#define OCEAN_FINLINE                           always_inline
	
	#define OCEAN_DEBUG_BREAK                       raise(SIGTRAP);
	
	#define OCEAN_CONCAT_OPERATOR(x, y)             x y

	#define OCEAN_LOCATION                          (__FILE_NAME__)

#endif

#define OCEAN_STRINGIFY(L)                          #L
#define OCEAN_MAKESTRING(L)                         OCEAN_STRINGIFY(L)
#define OCEAN_CONCAT(x, y)                          OCEAN_CONCAT_OPERATOR(x, y)
#define OCEAN_LINE_STRING                           OCEAN_MAKESTRING(__LINE__)

#define OCEAN_FILELINE(Message)                     OCEAN_LOCATION "(" OCEAN_LINE_STRING ") : " Message

#define OCEAN_UNIQUE_SUFFIX(Param)                  OCEAN_CONCAT(Param, __LINE__)

#define CONSOLE_TEXT_RED(Text)                      "\x1b[31m" Text "\033[0m"
#define CONSOLE_TEXT_GREEN(Text)                    "\x1b[32m" Text "\033[0m"
#define CONSOLE_TEXT_YELLOW(Text)                   "\x1b[33m" Text "\033[0m"
#define CONSOLE_TEXT_BLUE(Text)                     "\x1b[34m" Text "\033[0m"
#define CONSOLE_TEXT_MAGENTA(Text)                  "\x1b[35m" Text "\033[0m"
#define CONSOLE_TEXT_CYAN(Text)                     "\x1b[36m" Text "\033[0m"
#define CONSOLE_TEXT_WHITE(Text)                    "\x1b[37m" Text "\033[0m"
