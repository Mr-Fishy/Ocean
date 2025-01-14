#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/FloatingPoints.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Renderer/GraphicsContext.hpp"

namespace Ocean {

	class Window;

	/**
	 * @brief The window data to handle in GLFW or other window handling library.
	 */
	struct WindowData {
		WindowData() = default;
		/**
		 * @brief Construct a new WindowData object with the given width and height.
		 * 
		 * @param width The initial width.
		 * @param height The initial height.
		 */
		WindowData(u32 width, u32 height) : width(width), height(height) { }

		Window* window = nullptr; /** @brief A reference to the owner Window for callback referencing. */

		u16 width = 0; /** @brief The width of the window. */
		u16 height = 0; /** @brief The height of the window. */
		/**
		 * @brief The width of the window from when it was not fullscreen.
		 * @details This value is only used when the window is fullscreen to make sure that it returns to the correct width when it exits fullscreen.
		 */
		u16 windowedWidth = 0;
		/**
		 * @brief The height of the window from when it was not fullscreen.
		 * @details This value is only used when the window is fullscreen to make sure that it returns to the correct height when it exits fullscreen.
		 */
		u16 windowedHeight = 0;

		b8 vSync = true; /** @brief Records if the Window has VSYNC on. */
		b8 refreshed = false; /** @brief Records if the Window has been refreshed. AKA it is being resized or moved. */
		b8 centeredCursor = false; /** @brief Records if the Window has the cursor locked to the center. */
		b8 resized = false; /** @brief Records if the Window has been resized. */
		b8 fullscreen = false; /** @brief Records if the Window is in fullscreen mode. */

	};	// WindowData

	/**
	 * @brief The Ocean window class.
	 */
	class Window {
	public:
		/**
		 * @brief Construct a new Window object with default values.
		 */
		Window() : m_Context(), p_PlatformHandle(nullptr), m_Name("Ocean Window"), m_Data() { }
		/**
		 * @brief Construct a new Window object using a given width, height, and name.
		 * 
		 * @param width The initial width of the window.
		 * @param height The initial height of the window.
		 * @param name The name of the window.
		 */
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
		 * @brief Calls the update functions for the Window. This includes polling events and checking for close request.
		 */
		void OnUpdate();

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

		/**
		 * @brief Gets the name of the window.
		 * 
		 * @return cstring
		 */
		virtual cstring GetName() const { return this->m_Name; }

		/**
		 * @brief Creates a Window object.
		 * 
		 * @param width The initial window width.
		 * @param height The initial window height.
		 * @param name The window name.
		 * @return Scope<Window> 
		 */
		static Scope<Window> Create(u32 width, u32 height, cstring name);

	private:
		Window(Window&) = delete;
		Window operator = (Window&) = delete;

		/* --- */

		Scope<Shrimp::GraphicsContext> m_Context; /** @brief The graphics context of the window. AKA the OpenGL context or Vulkan instance. */
		void* p_PlatformHandle; /** @brief The platform handle from the underlying window library. */

		cstring m_Name; /** @brief The name of the window. The name is reflected in the window title. */

		f32 m_DisplayRefresh = 1.0f / 60.0f; /** @brief The refresh rate of the display the window is on. */

		b8 m_RequestedExit = false; /** @brief Records if the window is requesting to close. */
		b8 m_Minimized = false; /** @brief Records if the window is minimized. */

		WindowData m_Data; /** @brief The WindowData for the window that is used for window callbacks. */

	};	// Window

}	// Ocean
