#pragma once

#include "Ocean/Core/Window.hpp"

#include "Ocean/Renderer/GraphicsContext.hpp"

// libs
#include <GLFW/glfw3.h>

namespace Ocean {

	class WindowsWindow : public Window {
	private:
		struct WindowData {
			std::string Title{};

			ui32 Width{}, Height{};

			b8 VSync{};

			EventCallbackFn EventCallback;
		};

	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void Update() override;

		virtual ui32 GetWidth() const override { return m_Data.Width; }
		virtual ui32 GetHeight() const override { return m_Data.Height; }

		virtual void SetVSync(b8 enabled) override;
		virtual b8 IsVSync() const override;

		virtual void* GetNativeWindow() const override { return m_Window; }

		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		/* --- */

		GLFWwindow* m_Window;

		Scope<GraphicsContext> m_Context;

		WindowData m_Data;
	};

}	// Ocean
