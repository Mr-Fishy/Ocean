#pragma once

#include "Ocean/Primitives/Time.hpp"

/* 
 * The following is a good reference for C++ operator overloading practices.
 * https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading/4421719#4421719
 */

/**
 * @brief The Timestep is a class wrapper to handle an amount of time as a float.
 */
class Timestep {
public:
	/**
	 * @brief Construct a new Timestep object with the given time.
	 * 
	 * @param time The amount of time the Timestep is in seconds.
	 */
	Timestep(f32 time = 0.0f) : m_Time(time) { }
	/**
	 * @brief Construct a new Timestep object with the given Time.
	 * 
	 * @param time The Time to set, automatically converts to seconds.
	 */
	Timestep(Time time = 0) : m_Time(oTimeRealiSec(time)) { }

	/**
	 * @brief Get the Seconds as a floating point value.
	 * 
	 * @return f32 
	 */
	f32 GetSeconds() const { return m_Time; }
	/**
	 * @brief Get the Milliseconds object as a floatting point value.
	 * 
	 * @return f32 
	 */
	f32 GetMilliseconds() const { return m_Time * 1000.0f; }

	/**
	 * @brief f32 conversion operator. 
	 * 
	 * @return f32 in seconds.
	 */
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
	f32 m_Time; /** @brief The amount of time, in seconds, of the Timestep. */

};

inline Timestep operator + (const Timestep& lhs, const Timestep& rhs) {
	Timestep result(lhs); result += rhs;
	return result;
}

inline Timestep operator - (const Timestep &lhs, const Timestep& rhs) {
	Timestep result(lhs); result -= rhs;
	return result;
}
