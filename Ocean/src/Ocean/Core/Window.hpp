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

	typedef void (*OSMessageCallback)(void* osEvent, void* userData);

	class Window : public Service {
	public:
		void Init(void* config) override;
		void Shutdown() override;

		void SetFullscreen(b8 value);

		void CenterMouse(b8 dragging) const;

		void PollEvents() const;

		b8 RequestedExit() const { return m_RequestedExit; }
		b8 Minimized() const { return m_Minimized; }

	private:
		void* p_PlatformHandle = nullptr;

		b8 m_RequestedExit = false;
		b8 m_Resized = false;
		b8 m_Minimized = false;

		u32 m_Width = 0;
		u32 m_Height = 0;

		f32 m_DisplayRefresh = 1.0f / 60.0f;

		static constexpr cstring k_Name = "OCEAN_Window_Service";

	};	// Window

}	// Ocean
