#pragma once

#include "Ocean/Core/Window.hpp"

#include "Vk/VulkanContext.hpp"

// libs
#include <GLFW/glfw3.h>

// std
#include <cstring>

namespace Ocean {

	class WindowsWindow : public Window {
	private:
		struct WindowData {
			char* Title{};

			ui32 Width{}, Height{};

			b8 VSync{};

			VulkanContext* Context = nullptr;

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
		virtual void EndCommands() const override { vkDeviceWaitIdle(m_Data.Context->GetDevice()); }

		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

	private:
		static void FramebufferResizeCallback(GLFWwindow* window, i32 width, i32 height);

		/* --- */

		GLFWwindow* m_Window;

		WindowData m_Data;
	};

}	// Ocean
