#pragma once

#include "Ocean/Input/Event.hpp"
#include "Ocean/Input/InputCodes.hpp"

namespace Ocean {

	class KeyEvent : public Event {
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::EVENT_KEYBOARD | EventCategory::EVENT_INPUT)

	protected:
		KeyEvent(const KeyCode keycode) : m_KeyCode(keycode) { }

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(const KeyCode keycode, b8 repeating) : KeyEvent(keycode), m_Repeating(repeating) { }

		b8 GetRepeating() const { return m_Repeating; }

		/* --- DEBUG --- */
		std::string ToString() const override {
			std::stringstream ss;

			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_Repeating << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		b8 m_Repeating;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) { }

		/* --- DEBUG --- */
		std::string ToString() const override {
			std::stringstream ss;

			ss << "KeyReleasedEvent: " << m_KeyCode;

			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) { }

		/* --- DEBUG --- */
		std::string ToString() const override {
			std::stringstream ss;

			ss << "KeyTypedEvent: " << m_KeyCode;

			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}	// Ocean
