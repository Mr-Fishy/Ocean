#pragma once

#include "Event.hpp"

namespace Ocean {

	class OCEAN_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keyCode) : m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class OCEAN_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, bool repeating) : KeyEvent(keyCode), m_Repeating(repeating) {}

		inline int GetRepeating() const { return m_Repeating; }

		// DEBUG //
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_Repeating << " repeating)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool m_Repeating;
	};

	class OCEAN_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		// DEBUG //
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class OCEAN_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}

		// DEBUG //
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}	// Ocean