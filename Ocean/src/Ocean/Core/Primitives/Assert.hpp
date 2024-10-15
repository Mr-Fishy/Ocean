#pragma once

#include "Ocean/Core/Primitives/Macros.hpp"
#include "Ocean/Core/Primitives/Log.hpp"

namespace Ocean {

	// Assert given that the condition is NOT true. 
	#define OASSERT(condition) if (!(condition)) { oprint(OCEAN_FUNCTIONLINE(__FUNCTION__, "FALSE\n")); OCEAN_DEBUG_BREAK }

	// Asserts when the index is outside the length from 0.
	#define OASSERT_LENGTH(i, max) OASSERT((i) >= 0 && (i) < (max))

#if defined (_MSC_VER)

	// Assert given that the condition is NOT true. Also can be given a message like oprint.
	#define OASSERTM(condition, message, ...) if (!(condition)) { oprint(OCEAN_FUNCTIONLINE(__FUNCTION__, OCEAN_CONCAT(message, "\n")), __VA_ARGS__); OCEAN_DEBUG_BREAK }

#else

	// Assert given that the condition is NOT true. Also can be given a message like oprint.
	#define OASSERTM(condition, message, ...) if (!(condition)) { oprint(OCEAN_FUNCTIONLINE(__FUNCTION__, OCEAN_CONCAT(message, "\n")), ## __VA_ARGS__); OCEAN_DEBUG_BREAK }

#endif

}	// Ocean
