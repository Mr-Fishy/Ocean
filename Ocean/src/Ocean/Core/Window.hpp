#pragma once

#include "ocpch.hpp"

#include "Ocean/Core/Base.hpp"
#include "Ocean/Events/Event.hpp"

namespace Ocean {

	// Window Properties
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(
			const std::string& title = "Ocean Engine",
			unsigned int width = 1280,
			unsigned int height = 720
		) : Title(title), Width(width), Height(height) {}
	};

	// Interface Represents Desktop System Window
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());

		// Window Memory
		static void* wAllocate(uint64_t size, char aligned);
		static void wFree(void* block, char aligned);

		static void* wZeroMemory(void* block, uint64_t size);

		static void* wCopyMemory(void* dest, const void* source, uint64_t size);
		static void* wSetMemory(void* dest, int32_t value, uint64_t size);
	};

}	// Ocean