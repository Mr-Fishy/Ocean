#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/FloatingPoints.hpp"
#include "Ocean/Core/Types/UniquePtr.hpp"
#include "Ocean/Core/Types/Strings.hpp"

#include "Ocean/Core/Primitives/Service.hpp"

namespace Ocean {

	namespace Shrimp {
		
		class GraphicsContext;

	}	// Shrimp

	class Window;

	/**
	 * @brief The window data to handle in GLFW.
	 */
	struct WindowData {
		WindowData() = default;
		WindowData(u32 width, u32 height) : width(width), height(height) { }

		Window* window = nullptr;

		u16 width = 0, height = 0;
		u16 windowedWidth = 0, windowedHeight = 0;

		b8 vSync = true;
		b8 refreshed = false;
		b8 centeredCursor = false;
		b8 resized = false;
		b8 fullscreen = false;
	};

	/**
	 * @brief The Ocean window class.
	 */
	class Window : public Service {
	public:
		Window() : m_Context(), p_PlatformHandle(nullptr), m_Name("Ocean Window"), m_Data() { }
		Window(u32 width, u32 height, cstring name);
		virtual ~Window() = default;

		/**
		 * @brief Initializes the window.
		 * @param config - The configuration of the window using WindowConfig.
		 */
		void Init();
		/**
		 * @brief Shuts down the window.
		 */
		void Shutdown();

		/**
		 * @param enabled - Set's fullscreen on or off according to True or False.
		 */
		void SetFullscreen(b8 enabled);

		/**
		 * @param enabled - Locks the mouse to the center of the window according to True or False.
		 */
		void CenterMouse(b8 enabled) const;

		/**
		 * @brief Polls the queued window events.
		 */
		void PollEvents();

		/**
		 * @return The window width in screen coordinates.
		 */
		u32 Width() const { return this->m_Data.width; }
		/**
		 * @return The window height in screen coordinates.
		 */
		u32 Height() const { return this->m_Data.height; }
		/**
		 * @return The window handle from glfw.
		 */
		void* Handle() const { return this->p_PlatformHandle; }

		/**
		 * @return True if there is a requested exit, False otherwise.
		 */
		b8 RequestedExit() const { return this->m_RequestedExit; }

		/**
		 * @return True if the window is minimized, False otherwise.
		 */
		b8 Minimized() const { return this->m_Minimized; }

		/**
		 * @return True if the window has been resized, False otherwise.
		 */
		b8 Resized() const { return this->m_Data.resized; }
		/**
		 * @brief Tells the window that the resize has been handled.
		 */
		void ResizeHandled() { this->m_Data.resized = false; }

		virtual cstring GetName() const { return this->m_Name; }

		static UniquePtr<Window> Create(u32 width, u32 height, cstring name);

	private:
		Window(Window&) = delete;
		Window operator = (Window&) = delete;

		/* --- */

		UniquePtr<Shrimp::GraphicsContext> m_Context;
		void* p_PlatformHandle;

		cstring m_Name;

		f32 m_DisplayRefresh = 1.0f / 60.0f;

		b8 m_RequestedExit = false;
		b8 m_Minimized = false;

		WindowData m_Data;

	};	// Window

}	// Ocean
