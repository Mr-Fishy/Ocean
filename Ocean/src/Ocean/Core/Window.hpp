#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/FloatingPoints.hpp"

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

namespace Ocean {

	/**
	 * @brief The window configuration when creating a window.
	 */
	struct WindowConfig {

		u32 Width;
		u32 Height;

		cstring Name;

		Allocator* Allocator;

	};	// WindowConfig

	class Window;

	/**
	 * @brief The window data to handle in GLFW.
	 */
	struct WindowData {
		Window* Window;

		u16 Width, Height;
		u16 WindowedWidth, WindowedHeight;

		b8 VSync = true;
		b8 Refreshed = false;
		b8 CenteredCursor = false;
		b8 Resized = false;
		b8 Fullscreen = false;
	};

	/**
	 * @brief The Ocean window class.
	 */
	class Window : public Service {
	public:
		/**
		 * @brief Initializes the window.
		 * @param config - The configuration of the window using WindowConfig.
		 */
		virtual void Init(void* config) override;
		/**
		 * @brief Shuts down the window.
		 */
		virtual void Shutdown() override;

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
		u32 Width() const { return m_Data.Width; }
		/**
		 * @return The window height in screen coordinates.
		 */
		u32 Height() const { return m_Data.Height; }
		/**
		 * @return The window handle from glfw.
		 */
		void* Handle() const { return p_PlatformHandle; }

		/**
		 * @return True if there is a requested exit, False otherwise.
		 */
		b8 RequestedExit() const { return m_RequestedExit; }

		/**
		 * @return True if the window is minimized, False otherwise.
		 */
		b8 Minimized() const { return m_Minimized; }

		/**
		 * @return True if the window has been resized, False otherwise.
		 */
		b8 Resized() const { return m_Data.Resized; }
		/**
		 * @brief Tells the window that the resize has been handled.
		 */
		void ResizeHandled() { m_Data.Resized = false; }

	private:
		void* p_PlatformHandle = nullptr;

		b8 m_RequestedExit = false;
		b8 m_Minimized = false;

		f32 m_DisplayRefresh = 1.0f / 60.0f;

		WindowData m_Data;

	};	// Window

}	// Ocean
