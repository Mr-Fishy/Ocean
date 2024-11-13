#pragma once

#include "Ocean/Core/Types/FloatingPoints.hpp"

/* 
 * The following is a good reference for C++ operator overloading practices.
 * https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading/4421719#4421719
 */

class Timestep {
public:
	Timestep(f32 time = 0.0f) : m_Time(time) { }

	f32 GetSeconds() const { return m_Time; }
	f32 GetMilliseconds() const { return m_Time * 1000.0f; }

	operator f32() const { return m_Time; }

	Timestep& operator += (const Timestep& other) {
		this->m_Time += other.m_Time;
		return *this;
	}

	Timestep& operator -= (const Timestep& other) {
		this->m_Time -= other.m_Time;
		return *this;
	}

private:
	f32 m_Time;

};

inline Timestep operator + (const Timestep& lhs, const Timestep& rhs) {
	Timestep result(lhs); result += rhs;
	return result;
}

inline Timestep operator - (const Timestep &lhs, const Timestep& rhs) {
	Timestep result(lhs); result -= rhs;
	return result;
}
