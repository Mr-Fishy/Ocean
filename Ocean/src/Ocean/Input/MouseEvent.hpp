#pragma once

#include "Ocean/Input/Event.hpp"
#include "Ocean/Input/InputCodes.hpp"

namespace Ocean {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(const f32 x, const f32 y) : m_MouseX(x), m_MouseY(y) { }

		f32 GetX() const { return m_MouseX; }
		f32 GetY() const { return m_MouseY; }

		/* --- DEBUG --- */
		std::string ToString() const override {
			std::stringstream ss;

			ss << "MouseMovedEvent: (" << m_MouseX << ", " << m_MouseY << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EVENT_MOUSE | EVENT_INPUT)

	private:
		f32 m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(const f32 xOffset, const f32 yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) { }

		f32 GetXOffset() const { return m_XOffset; }
		f32 GetYOffset() const { return m_YOffset; }

		/* --- DEBUG --- */
		std::string ToString() const override {
			std::stringstream ss;

			ss << "MouseScrolledEvent: (" << m_XOffset << ", " << m_YOffset << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EVENT_MOUSE | EVENT_INPUT)

	private:
		f32 m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EVENT_MOUSE | EVENT_INPUT | EVENT_MOUSEBUTON)

	protected:
		MouseButtonEvent(const MouseCode  button) : m_Button(button) { }

		MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) { }

		/* --- DEBUG --- */
		std::string ToString() const override {
			std::stringstream ss;

			ss << "MouseButtonPressedEvent: (" << m_Button << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) { }

		/* --- DEBUG --- */
		std::string ToString() const override {
			std::stringstream ss;

			ss << "MouseButtonReleasedEvent: (" << m_Button << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}	// Ocean
