#include "ocpch.hpp"

#include "Ocean/Core/Input.hpp"

#include "Ocean/Core/Application.hpp"
#include "Ocean/Input/KeyEvent.hpp"

// libs
#include <GLFW/glfw3.h>

namespace Ocean {

	b8 Input::IsKeyPressed(KeyCode key) {
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<i32>(key));

		return state == GLFW_PRESS;
	}

	b8 Input::IsMouseButtonPressed(MouseCode button) {
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}

	vec2 Input::GetMousePosition() {
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { static_cast<f32>(xpos), static_cast<f32>(ypos) };
	}

}	// Ocean
