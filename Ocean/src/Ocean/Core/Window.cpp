#include "ocpch.hpp"

#include "Ocean/Core/Window.hpp"

#ifdef OC_PLATFORM_WINDOWS
	#include "Platform/WindowsWindow.hpp"
#endif // OC_PLATFORM_WINDOWS


namespace Ocean {

	Window* Window::Create(const WindowProps& props) {
	#ifdef OC_PLATFORM_WINDOWS
		return new WindowsWindow(props);
	#elif
		// TODO: ASSERT
		return nullptr;
	#endif // OC_PLATFORM_WINDOWS
	}

}	// Ocean
