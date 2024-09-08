#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/FloatingPoints.hpp"

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Array.hpp"

namespace Ocean {

	struct WindowConfig {

		u32 Width;
		u32 Height;

		cstring Name;

		Allocator* Allocator;

	};	// WindowConfig

	class Window;

	struct WindowData {
		Window* Window;

		u32 Width, Height;
		u32 WindowedWidth, WindowedHeight;

		b8 VSync = true;
		b8 Refreshed = false;
		b8 CenteredCursor = false;
		b8 Resized = false;
		b8 Fullscreen = false;
	};

	class Window : public Service {
	public:
		virtual void Init(void* config) override;
		virtual void Shutdown() override;

		void SetFullscreen(b8 enabled);

		void CenterMouse(b8 enabled) const;

		void PollEvents();

		u32 Width() const { return m_Data.Width; }
		u32 Height() const { return m_Data.Height; }
		void* Handle() const { return p_PlatformHandle; }

		b8 RequestedExit() const { return m_RequestedExit; }
		b8 Minimized() const { return m_Minimized; }
		b8 Resized() const { return m_Data.Resized; }
		void ResizeHandled() { m_Data.Resized = false; }

	private:
		void* p_PlatformHandle = nullptr;

		b8 m_RequestedExit = false;
		b8 m_Minimized = false;

		f32 m_DisplayRefresh = 1.0f / 60.0f;

		WindowData m_Data;

	};	// Window

}	// Ocean
