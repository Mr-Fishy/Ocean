#pragma once

#include "Ocean/Core/Primitives/Macros.hpp"
#include "Ocean/Core/Primitives/Log.hpp"

namespace Ocean {

	/** @brief Assert given that the condition is NOT true. */
	#define OASSERT(condition) if (!(condition)) { oprint(OCEAN_FUNCTIONLINE("FALSE\n")); OCEAN_DEBUG_BREAK; }

	/** @brief Asserts when the index is outside the length from 0. */
	#define OASSERT_LENGTH(i, max) OASSERT((i) < (max))

	/** @brief Assert given that the condition is NOT true. Also can be given a message like oprint. */
	#define OASSERTM(condition, message, ...) if (!(condition)) { oprintret(OCEAN_FUNCTIONLINE(message), ## __VA_ARGS__); OCEAN_DEBUG_BREAK; }

}	// Ocean
