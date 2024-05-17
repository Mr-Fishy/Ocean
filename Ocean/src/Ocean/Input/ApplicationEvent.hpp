#pragma once

#include "Ocean/Input/Event.hpp"

namespace Ocean {

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(ui32 width, ui32 height) : m_Width(width), m_Height(height) { }

		ui32 GetWidth() const { return m_Width; }
		ui32 GetHeight() const { return m_Height; }

		#ifdef OC_DEBUG

		std::string ToString() const override {
			std::stringstream ss;

			ss << "WindowResizeEvent {\'Width\': " << m_Width << ", \'Height\': " << m_Height << "}";

			return ss.str();
		}

		#endif // OC_DEBUG

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EVENT_APPLICATION)

	private:
		ui32 m_Width, m_Height;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EVENT_APPLICATION)
	};

	class AppTickEvent : public Event {
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EVENT_APPLICATION)
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EVENT_APPLICATION)
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EVENT_APPLICATION)
	};

}	// Ocean
