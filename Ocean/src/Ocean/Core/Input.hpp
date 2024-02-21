#pragma once

#include "Ocean/Core/KeyCodes.hpp"
#include "Ocean/Core/MouseCodes.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

	class Input {
	public:
		// Checks if key is currently pressed through GLFW.
		// @param key: The keycode for the key that is being checked (uses Ocean keycodes).
		// @returns True or False
		//
		static bool IsKeyPressed(KeyCode key);
		// Checks if the mouse-button is currently being pressed through GLFW.
		// @param button: The mousecode for the button that is being checked (uses Ocean mousecodes).
		// @returns True or False
		//
		static bool IsMouseButtonPressed(MouseCode button);
		// Gets the x and y mouse position as a paired float.
		// @returns pair<float, float> (a struct of two floats as x and y)
		//
		static glm::vec2 GetMousePosition();
		// Gets the x position of the mouse.
		// @returns float
		//
		static float GetMouseX();
		// Gets the y position of the mouse.
		// @returns float
		//
		static float GetMouseY();

	private:
		// A vector of the currently pressed keys.
		//
		std::vector<KeyCode> m_PressedKeys;
	};

}	// Ocean
