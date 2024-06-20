#pragma once

#include "Ocean/Input/InputCodes.hpp"
#include "Ocean/Core/Types.hpp"

namespace Ocean {

	class Input {
	public:
		static b8 IsKeyPressed(KeyCode key);

		static b8 IsMouseButtonPressed(MouseCode button);

		static vec2 GetMousePosition();

		static f32 GetMouseX() { return GetMousePosition().x; }

		static f32 GetMouseY() { return GetMousePosition().y; }
	};

}	// Ocean
