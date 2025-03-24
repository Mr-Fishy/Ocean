#pragma once

#include "Ocean/Types/FloatingPoints.hpp"
#include "Ocean/Types/Integers.hpp"

using Time = i64;

/**
	* @brief Begins application time tracking, needs to be called once at startup.
	*/
void oTimeServiceInit();
/**
	* @brief Ends application time tracking, needs to be called once at shutdown.
	*/
void oTimeServiceShutdown();

/**
	* @brief Gets the current time.
	* @return The current time in ticks.
	*/
Time oTimeNow();

/**
	* @brief Gets microseconds from time ticks.
	* @param t - The time in ticks.
	* @return The time in microseconds.
	*/
f64  oTimeMicroSec(Time t);
/**
	* @brief Gets milliseconds from time ticks.
	* @param t - The time in ticks.
	* @return The time in milliseconds.
	*/
f64  oTimeMilliSec(Time t);
/**
	* @brief Gets seconds from time ticks.
	* @param t - The time in ticks.
	* @return The time in seconds.
	*/
f64  oTimeRealiSec(Time t);

/**
	* @brief Gets the difference in time from start to current time.
	* @param start - The time to start from.
	* @return The time in ticks.
	*/
Time oTimeFrom(Time start);

/**
	* @brief Gets the difference in time from start to current time.
	* @param start - The time to start from.
	* @return The time in microseconds.
	*/
f64  oTimeFromMicroSec(Time start);
/**
	* @brief Gets the difference in time from start to current time.
	* @param start - The time to start from.
	* @return The time in milliseconds.
	*/
f64  oTimeFromMilliSec(Time start);
/**
	* @brief Gets the difference in time from start to current time.
	* @param start - The time to start from.
	* @return The time in seconds.
	*/
f64  oTimeFromRealiSec(Time start);

/**
	* @brief Gets the difference in time from start to end.
	* @param start - The time to start from.
	* @param end - The time to end at.
	* @return The time in milliseconds.
	*/
f64  oTimeDeltaMilliSec(Time start, Time end);
/**
	* @brief Gets the difference in time from start to end.
	* @param start - The time to start from.
	* @param end - The time to end at.
	* @return The time in seconds.
	*/
f64  oTimeDeltaRealiSec(Time start, Time end);
