#pragma once

#include "Ocean/Core/PlatformBase.hpp"
#include "Ocean/Core/Defines.hpp"

#include "Ocean/Input/Event.hpp"

// std
#include <cstring>

namespace Ocean {

	struct WindowProps {
		char* Title;
		ui32 Width, Height;

		WindowProps(const char* title = "Ocean Window", ui32 width = 1600, ui32 height = 900)
			: Title(nullptr), Width(width), Height(height) {
			Title = new char[sizeof(title)];
			std::strcpy(Title, title);
		}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() = default;
		virtual ~Window() = default;

		virtual void Update() = 0;

		virtual ui32 GetWidth() const = 0;
		virtual ui32 GetHeight() const = 0;

		virtual void SetVSync(b8 enabled) = 0;
		virtual b8 IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}	// Ocean
