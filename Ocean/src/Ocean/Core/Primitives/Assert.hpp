#pragma once

#include "Ocean/Core/Primitives/Macros.hpp"
#include "Ocean/Core/Primitives/Log.hpp"

namespace Ocean {

	// Assert given that the condition is NOT true. 
	#define OASSERT(condition) if (!(condition)) { oprint(OCEAN_FILELINE("FALSE\n")); OCEAN_DEBUG_BREAK }

#if defined (_MSC_VER)

	// Assert given that the condition is NOT true. Also can be given a message like oprint.
	#define OASSERTM(condition, message, ...) if (!(condition)) { oprint(OCEAN_FILELINE(OCEAN_CONCAT(message, "\n")), __VA_ARGS__); OCEAN_DEBUG_BREAK }

#else

	// Assert given that the condition is NOT true. Also can be given a message like oprint.
	#define OASSERTM(condition, message, ...) if (!(condition)) { oprint(OCEAN_FILELINE(OCEAN_CONCAT(message, "\n")), ## __VA_ARGS__); OCEAN_DEBUG_BREAK }

#endif

}	// Ocean
