#pragma once

#include "Ocean/Events/Event.hpp"
#include "Ocean/Core/KeyCodes.hpp"

namespace Ocean {

	class KeyEvent : public Event {
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(const KeyCode keyCode) : m_KeyCode(keyCode) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(const KeyCode keyCode, bool repeating) : KeyEvent(keyCode), m_Repeating(repeating) {}

		int GetRepeating() const { return m_Repeating; }

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

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(const KeyCode keyCode) : KeyEvent(keyCode) {}

		// DEBUG //
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(const KeyCode keyCode) : KeyEvent(keyCode) {}

		// DEBUG //
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}	// Ocean
