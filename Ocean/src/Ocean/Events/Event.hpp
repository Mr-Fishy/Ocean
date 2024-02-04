#pragma once

#include "Ocean/Core/Base.hpp"
#include "Ocean/Debug/Instrumentor.hpp"

// std
#include <functional>

namespace Ocean {

	/*
		Events are currently blocked, meaning when an event occurs, 
		it immediately gets dispatched and must be dealt with right then and there.
		For the future, a better strategy might be to buffer events in an event bus,
		and then process them during the "event" part of the update stage.
	*/

	// Enum To Define Types Of Expected Events //
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	// Enum To Sort Event Types Into Grouped Event Types (Uses Bit Field Map) //
	enum EventCategory {
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event {
		friend class EventDispatcher;

	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;					// DEBUG Only
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }	// DEBUG Only

		// Utility Filter Check (Returns 0 If False) //
		bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		// F is deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled |= func(static_cast<T&>(m_Event));

				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator << (std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}	// Ocean
