
#include "ocpch.hpp"
#include "Window.hpp"

#ifdef OC_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.hpp"

	namespace Ocean {

		void* Window::wAllocate(uint64_t size, char aligned) { return malloc(size); }

		void Window::wFree(void* block, char aligned) { free(block); }

		void* Window::wZeroMemory(void* block, uint64_t size) { return memset(block, 0, size); }

		void* Window::wCopyMemory(void* dest, const void* source, uint64_t size) { return memcpy(dest, source, size); }

		void* Window::wSetMemory(void* dest, int32_t value, uint64_t size) { return memset(dest, value, size); }

	}	// Ocean
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