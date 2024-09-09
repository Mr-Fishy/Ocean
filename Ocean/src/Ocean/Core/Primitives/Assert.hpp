#pragma once

#include "Ocean/Core/Primitives/Macros.hpp"
#include "Ocean/Core/Primitives/Log.hpp"

namespace Ocean {

	#define OASSERT(condition) if (!(condition)) { oprint(OCEAN_FILELINE("FALSE\n")); OCEAN_DEBUG_BREAK }

#if defined (_MSC_VER)

	#define OASSERTM(condition, message, ...) if (!(condition)) { oprint(OCEAN_FILELINE(OCEAN_CONCAT(message, "\n")), __VA_ARGS__); OCEAN_DEBUG_BREAK }

#else

	#define OASSERTM(condition, message, ...) if (!(condition)) { oprint(OCEAN_FILELINE(OCEAN_CONCAT(message, "\n")), ## __VA_ARGS__); OCEAN_DEBUG_BREAK }

#endif

}	// Ocean
