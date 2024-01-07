#pragma once

// libs
#include <ctti/type_id.hpp>

namespace SECS {

	typedef unsigned int EntityId;
	constexpr EntityId INVALID_ENTITY_ID = 0;

	struct ComponentAutoCounterId {};

	typedef unsigned int ComponentID;
	#define ID_DECLARATION(COMPONENT_NAME) \
		static constexpr ComponentID ID = ctti::type_id<COMPONENT_NAME>().hash()

}	// SECS
