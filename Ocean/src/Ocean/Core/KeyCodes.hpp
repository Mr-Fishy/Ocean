#pragma once

namespace Ocean {

	// A 16 bit integer code that represents a keyboard input.
	// Keycodes adapted from glfw3.h
	//
	typedef enum class KeyCode : uint16_t {
		// Based Upon Keycodes From glfw3.h
		Unkown = 0,

		Space = 32,
		Apostrophe = 39,   /* ' */
		Comma = 44,        /* , */
		Minus = 45,        /* - */
		Period = 46,       /* . */
		Slash = 47,        /* / */

		D0 = 48,           /* 0 */
		D1 = 49,           /* 1 */
		D2 = 50,           /* 2 */
		D3 = 51,           /* 3 */
		D4 = 52,           /* 4 */
		D5 = 53,           /* 5 */
		D6 = 54,           /* 6 */
		D7 = 55,           /* 7 */
		D8 = 56,           /* 8 */
		D9 = 57,           /* 9 */

		Semicolon = 59,    /* ; */
		Equal = 61,        /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,    /* \ */
		RightBracket = 93, /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161,      /* non-US #1 */
		World2 = 162,      /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode) {
		os << static_cast<int32_t>(keyCode);
		return os;
	}

}	// Ocean

// Based Upon Keycodes From glfw3.h
/* Printable Keys */

#define OC_KEY_SPACE           ::Ocean::Key::Space
#define OC_KEY_APOSTROPHE      ::Ocean::Key::Apostrophe    /* ' */
#define OC_KEY_COMMA           ::Ocean::Key::Comma         /* , */
#define OC_KEY_MINUS           ::Ocean::Key::Minus         /* - */
#define OC_KEY_PERIOD          ::Ocean::Key::Period        /* . */
#define OC_KEY_SLASH           ::Ocean::Key::Slash         /* / */
#define OC_KEY_0               ::Ocean::Key::D0
#define OC_KEY_1               ::Ocean::Key::D1
#define OC_KEY_2               ::Ocean::Key::D2
#define OC_KEY_3               ::Ocean::Key::D3
#define OC_KEY_4               ::Ocean::Key::D4
#define OC_KEY_5               ::Ocean::Key::D5
#define OC_KEY_6               ::Ocean::Key::D6
#define OC_KEY_7               ::Ocean::Key::D7
#define OC_KEY_8               ::Ocean::Key::D8
#define OC_KEY_9               ::Ocean::Key::D9
#define OC_KEY_SEMICOLON       ::Ocean::Key::Semicolon     /* ; */
#define OC_KEY_EQUAL           ::Ocean::Key::Equal         /* = */
#define OC_KEY_A               ::Ocean::Key::A
#define OC_KEY_B               ::Ocean::Key::B
#define OC_KEY_C               ::Ocean::Key::C
#define OC_KEY_D               ::Ocean::Key::D
#define OC_KEY_E               ::Ocean::Key::E
#define OC_KEY_F               ::Ocean::Key::F
#define OC_KEY_G               ::Ocean::Key::G
#define OC_KEY_H               ::Ocean::Key::H
#define OC_KEY_I               ::Ocean::Key::I
#define OC_KEY_J               ::Ocean::Key::J
#define OC_KEY_K               ::Ocean::Key::K
#define OC_KEY_L               ::Ocean::Key::L
#define OC_KEY_M               ::Ocean::Key::M
#define OC_KEY_N               ::Ocean::Key::N
#define OC_KEY_O               ::Ocean::Key::O
#define OC_KEY_P               ::Ocean::Key::P
#define OC_KEY_Q               ::Ocean::Key::Q
#define OC_KEY_R               ::Ocean::Key::R
#define OC_KEY_S               ::Ocean::Key::S
#define OC_KEY_T               ::Ocean::Key::T
#define OC_KEY_U               ::Ocean::Key::U
#define OC_KEY_V               ::Ocean::Key::V
#define OC_KEY_W               ::Ocean::Key::W
#define OC_KEY_X               ::Ocean::Key::X
#define OC_KEY_Y               ::Ocean::Key::Y
#define OC_KEY_Z               ::Ocean::Key::Z
#define OC_KEY_LEFT_BRACKET    ::Ocean::Key::LeftBracket   /* [ */
#define OC_KEY_BACKSLASH       ::Ocean::Key::Backslash     /* \ */
#define OC_KEY_RIGHT_BRACKET   ::Ocean::Key::RightBracket  /* ] */
#define OC_KEY_GRAVE_ACCENT    ::Ocean::Key::GraveAccent   /* ` */
#define OC_KEY_WORLD_1         ::Ocean::Key::World1        /* non-US #1 */
#define OC_KEY_WORLD_2         ::Ocean::Key::World2        /* non-US #2 */

/* Function keys */
#define OC_KEY_ESCAPE          ::Ocean::Key::Escape
#define OC_KEY_ENTER           ::Ocean::Key::Enter
#define OC_KEY_TAB             ::Ocean::Key::Tab
#define OC_KEY_BACKSPACE       ::Ocean::Key::Backspace
#define OC_KEY_INSERT          ::Ocean::Key::Insert
#define OC_KEY_DELETE          ::Ocean::Key::Delete
#define OC_KEY_RIGHT           ::Ocean::Key::Right
#define OC_KEY_LEFT            ::Ocean::Key::Left
#define OC_KEY_DOWN            ::Ocean::Key::Down
#define OC_KEY_UP              ::Ocean::Key::Up
#define OC_KEY_PAGE_UP         ::Ocean::Key::PageUp
#define OC_KEY_PAGE_DOWN       ::Ocean::Key::PageDown
#define OC_KEY_HOME            ::Ocean::Key::Home
#define OC_KEY_END             ::Ocean::Key::End
#define OC_KEY_CAPS_LOCK       ::Ocean::Key::CapsLock
#define OC_KEY_SCROLL_LOCK     ::Ocean::Key::ScrollLock
#define OC_KEY_NUM_LOCK        ::Ocean::Key::NumLock
#define OC_KEY_PRINT_SCREEN    ::Ocean::Key::PrintScreen
#define OC_KEY_PAUSE           ::Ocean::Key::Pause
#define OC_KEY_F1              ::Ocean::Key::F1
#define OC_KEY_F2              ::Ocean::Key::F2
#define OC_KEY_F3              ::Ocean::Key::F3
#define OC_KEY_F4              ::Ocean::Key::F4
#define OC_KEY_F5              ::Ocean::Key::F5
#define OC_KEY_F6              ::Ocean::Key::F6
#define OC_KEY_F7              ::Ocean::Key::F7
#define OC_KEY_F8              ::Ocean::Key::F8
#define OC_KEY_F9              ::Ocean::Key::F9
#define OC_KEY_F10             ::Ocean::Key::F10
#define OC_KEY_F11             ::Ocean::Key::F11
#define OC_KEY_F12             ::Ocean::Key::F12
#define OC_KEY_F13             ::Ocean::Key::F13
#define OC_KEY_F14             ::Ocean::Key::F14
#define OC_KEY_F15             ::Ocean::Key::F15
#define OC_KEY_F16             ::Ocean::Key::F16
#define OC_KEY_F17             ::Ocean::Key::F17
#define OC_KEY_F18             ::Ocean::Key::F18
#define OC_KEY_F19             ::Ocean::Key::F19
#define OC_KEY_F20             ::Ocean::Key::F20
#define OC_KEY_F21             ::Ocean::Key::F21
#define OC_KEY_F22             ::Ocean::Key::F22
#define OC_KEY_F23             ::Ocean::Key::F23
#define OC_KEY_F24             ::Ocean::Key::F24
#define OC_KEY_F25             ::Ocean::Key::F25

/* Keypad */
#define OC_KEY_KP_0            ::Ocean::Key::KP0
#define OC_KEY_KP_1            ::Ocean::Key::KP1
#define OC_KEY_KP_2            ::Ocean::Key::KP2
#define OC_KEY_KP_3            ::Ocean::Key::KP3
#define OC_KEY_KP_4            ::Ocean::Key::KP4
#define OC_KEY_KP_5            ::Ocean::Key::KP5
#define OC_KEY_KP_6            ::Ocean::Key::KP6
#define OC_KEY_KP_7            ::Ocean::Key::KP7
#define OC_KEY_KP_8            ::Ocean::Key::KP8
#define OC_KEY_KP_9            ::Ocean::Key::KP9
#define OC_KEY_KP_DECIMAL      ::Ocean::Key::KPDecimal
#define OC_KEY_KP_DIVIDE       ::Ocean::Key::KPDivide
#define OC_KEY_KP_MULTIPLY     ::Ocean::Key::KPMultiply
#define OC_KEY_KP_SUBTRACT     ::Ocean::Key::KPSubtract
#define OC_KEY_KP_ADD          ::Ocean::Key::KPAdd
#define OC_KEY_KP_ENTER        ::Ocean::Key::KPEnter
#define OC_KEY_KP_EQUAL        ::Ocean::Key::KPEqual

#define OC_KEY_LEFT_SHIFT      ::Ocean::Key::LeftShift
#define OC_KEY_LEFT_CONTROL    ::Ocean::Key::LeftControl
#define OC_KEY_LEFT_ALT        ::Ocean::Key::LeftAlt
#define OC_KEY_LEFT_SUPER      ::Ocean::Key::LeftSuper
#define OC_KEY_RIGHT_SHIFT     ::Ocean::Key::RightShift
#define OC_KEY_RIGHT_CONTROL   ::Ocean::Key::RightControl
#define OC_KEY_RIGHT_ALT       ::Ocean::Key::RightAlt
#define OC_KEY_RIGHT_SUPER     ::Ocean::Key::RightSuper
#define OC_KEY_MENU            ::Ocean::Key::Menu
