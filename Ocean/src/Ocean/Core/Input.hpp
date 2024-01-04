#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/KeyCodes.hpp"
#include "Ocean/Core/MouseCodes.hpp"

namespace Ocean {

	// TODO: Ideally for switch case based key handling, while a key is pressed at a frame, a variable like "isKeyPressed" would be set to true and a vector of the keys pressed would be used to keep track of the current keys pressed.
	// This way a switch case can "GetPressedKeys()" and map movement / functionality based on the keys pressed. Character keys should always be sorted to the front of the vector. Or "GetPressedCharKeys()" and "GetPressedFuncKeys()" can be done to sort them upfront to make things easier for keybinding.
	// This should allow more straightforward / slightly faster keymapping, minimizing the time spent determining input.
	//
	class Input {
	public:
		static bool IsKeyPressed(KeyCode key);
		static int GetKeyPressed();

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}	// Ocean
