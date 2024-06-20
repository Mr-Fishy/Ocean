#pragma once

#include "Ocean/Core/Base.hpp"
#include "Ocean/Core/Defines.hpp"

// std
#include <functional>
#include <sstream>

namespace Ocean {

	/*
	 * Events are handled in a block method, meaing that when an event occurs,
	 * it immediately gets dispatched and must be dealt with.
	 * 
	 * For the future, a better strategy would be to have a block and buffer event system,
	 * which would allow a event buffer to exist that can be handled each frame,
	 * rather than holding the frame until the event is handled.
	*/

	enum class EventType {
		None = 0,

		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,

		AppTick, AppUpdate, AppRender,

		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	enum EventCategory {
		NONE = 0,
		EVENT_APPLICATION	= 1 << 0,
		EVENT_INPUT			= 1 << 1,
		EVENT_KEYBOARD		= 1 << 2,
		EVENT_MOUSE			= 1 << 3,
		EVENT_MOUSEBUTON	= 1 << 4,
	};



	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								   virtual EventType GetEventType() const override { return GetStaticType(); }\
								   virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual ui32 GetCategoryFlags() const override { return category; }



	class Event {
	public:
		virtual ~Event() = default;

		b8 Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual ui32 GetCategoryFlags() const = 0;

		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		b8 IsInCategory(EventCategory category) const { return GetCategoryFlags() & category; }

	private:
		friend class EventDispatcher;
	};

	inline std::ostream& operator << (std::ostream& os, const Event& e) {
		return os << e.ToString();
	}



	class EventDispatcher {
	public:
		EventDispatcher(Event& event) : m_Event(event) { }

		template<typename T, typename F>
		b8 Dispatch(const F& func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled |= func(static_cast<T&>(m_Event));

				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

}	// Ocean
