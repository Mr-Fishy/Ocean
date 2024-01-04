#pragma once

namespace Ocean {

	typedef enum class MouseCode : uint16_t {
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode) {
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

}	// Ocean

#define OC_MOUSE_BUTTON_0      ::Ocean::Mouse::Button0
#define OC_MOUSE_BUTTON_1      ::Ocean::Mouse::Button1
#define OC_MOUSE_BUTTON_2      ::Ocean::Mouse::Button2
#define OC_MOUSE_BUTTON_3      ::Ocean::Mouse::Button3
#define OC_MOUSE_BUTTON_4      ::Ocean::Mouse::Button4
#define OC_MOUSE_BUTTON_5      ::Ocean::Mouse::Button5
#define OC_MOUSE_BUTTON_6      ::Ocean::Mouse::Button6
#define OC_MOUSE_BUTTON_7      ::Ocean::Mouse::Button7
#define OC_MOUSE_BUTTON_LAST   ::Ocean::Mouse::ButtonLast
#define OC_MOUSE_BUTTON_LEFT   ::Ocean::Mouse::ButtonLeft
#define OC_MOUSE_BUTTON_RIGHT  ::Ocean::Mouse::ButtonRight
#define OC_MOUSE_BUTTON_MIDDLE ::Ocean::Mouse::ButtonMiddle
