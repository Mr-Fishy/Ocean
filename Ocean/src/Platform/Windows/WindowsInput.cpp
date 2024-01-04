
#include "ocpch.hpp"
#include "Ocean/Core/Input.hpp"

#include "Ocean/Core/Application.hpp"
#include "Ocean/Events/KeyEvent.hpp"

// libs
#include <GLFW/glfw3.h>

namespace Ocean {

	bool Input::IsKeyPressed(KeyCode key) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	int Input::GetKeyPressed() {
		return 0;
	}

	bool Input::IsMouseButtonPressed(MouseCode button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(button));

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX() {
		auto [x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMouseY() {
		auto [x, y] = GetMousePosition();

		return y;
	}

}	// Ocean