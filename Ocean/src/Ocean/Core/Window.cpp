
#include "ocpch.hpp"
#include "Window.hpp"

#ifdef OC_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.hpp"
#endif

namespace Ocean {

	Scope<Window> Window::Create(const WindowProps& props) {
		#ifdef OC_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);
		#else
			OC_CORE_ASSERT(false, "Unkown platform!");
			return nullptr;
		#endif
	}

}	// Ocean