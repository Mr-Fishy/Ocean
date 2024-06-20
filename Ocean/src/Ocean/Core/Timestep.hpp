#pragma once

#include "Ocean/Core/Types.hpp"

namespace Ocean {

	class Timestep {
	public:
		Timestep(f32 time = 0.0f) : m_Time(time) { }

		operator float() const { return m_Time; }

		f32 GetSeconds() const { return m_Time; }
		f32 GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		f32 m_Time;
	};

}	// Ocean
