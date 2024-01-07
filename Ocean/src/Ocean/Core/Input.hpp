#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/KeyCodes.hpp"
#include "Ocean/Core/MouseCodes.hpp"

namespace Ocean {

	// TODO: In order to efficiently keep track of currently pressed keys, managing a vector of the currently pressed keys will be best.
	// This way when a key is pressed, it will be added to the vector, but when it is released it will be removed.
	//
	class Input {
	public:
		// Checks if key is currently pressed through GLFW.
		// @param key: The keycode for the key that is being checked (uses Ocean keycodes).
		// @returns True or False
		//
		static bool IsKeyPressed(KeyCode key);
		// WIP --- Gets the current key that is being pressed.
		// @returns The keycode of the currently pressed key. Returns UNKOWN key if no known platform key is being pressed.
		//
		static KeyCode GetKeyPressed();

		// Checks if the mouse-button is currently being pressed through GLFW.
		// @param mouse-key: The mousecode for the button that is being checked (uses Ocean mousecodes).
		// @returns True or False
		//
		static bool IsMouseButtonPressed(MouseCode button);
		// Gets the x and y mouse position as a paired float.
		// @returns pair<float, float> (a struct of two floats as x and y)
		//
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	private:
		// A vector of the currently pressed keys.
		//
		std::vector<KeyCode> m_PressedKeys;
	};

}	// Ocean
