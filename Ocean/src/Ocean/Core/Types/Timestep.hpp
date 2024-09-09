#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"

namespace Ocean {

	class Timestep {
	public:
		Timestep(f32 time = 0.0f) : m_Time(time) { }

		f32 GetSeconds() const { return m_Time; }
		f32 GetMilliseconds() const { return m_Time * 1000.0f; }

		operator f32() const { return m_Time; }

	private:
		f32 m_Time;

	};

}	// Ocean
