#include "Time.hpp"

#include "Ocean/Types/FloatingPoints.hpp"

#include "Ocean/Primitives/Macros.hpp"

#if defined(_MSC_VER)

	#define WIN32_LEAN_AND_MEAN 1
	#include <Windows.h>

#else

	#include <time.h>

#endif

#if defined(_MSC_VER)

	// Cached frequency.
	// From Microsoft Docs: (https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency)
	// "The frequency of the performance counter is fixed at system boot and is consistent across all processors. 
	// Therefore, the frequency need only be queried upon application initialization, and the result can be cached."
	static LARGE_INTEGER s_frequency;

#endif

void oTimeServiceInit() {
#if defined(_MSC_VER)

	// Cache this value - by Microsoft Docs it will not change during process lifetime.
	QueryPerformanceFrequency(&s_frequency);

#endif

}

void oTimeServiceShutdown() {
	// Nothing yet...
}

// Adapted from the Rust code base: https://github.com/rust-lang/rust/blob/3809bbf47c8557bd149b3e52ceb47434ca8378d5/src/libstd/sys_common/mod.rs#L124
// Computes (value*numer) / denom without overflow, as long as both
// (numer * denom) and the overall result fit into i64 (which is the case
// for our time conversions).
OC_UNUSED static i64 int64_mul_div(i64 value, i64 numer, i64 denom) {
	const i64 q = value / denom;
	const i64 r = value % denom;
	// Decompose value as (value / denom * denom + value % denom),
	// substitute into (value * numer) / denom and simplify.
	// r < denom, so (denom * numer) is the upper bound of (r * numer)
	return q * numer + r * numer / denom;
}

Time oTimeNow() {
#if defined(_MSC_VER)

	// Get current time
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	// Convert to microseconds
	// const i64 microseconds_per_second = 1000000LL;
	const i64 microseconds = int64_mul_div(time.QuadPart, 1000000LL, s_frequency.QuadPart);

#else

	clock_t time = clock();

	const u64 microseconds = 1000000ULL * time / CLOCKS_PER_SEC;

#endif

	return microseconds;
}

f64 oTimeMicroSec(Time t) {
	return static_cast<f64>(t);
}

f64 oTimeMilliSec(Time t) {
	return static_cast<f64>(t) / 1000.0;
}

f64 oTimeRealiSec(Time t) {
	return static_cast<f64>(t) / 1000000.0;
}

Time oTimeFrom(Time start) {
	return oTimeNow() - start;
}

f64 oTimeFromMicroSec(Time start) {
	return oTimeMicroSec(oTimeFrom(start));
}

f64 oTimeFromMilliSec(Time start) {
	return oTimeMilliSec(oTimeFrom(start));
}

f64 oTimeFromRealiSec(Time start) {
	return oTimeRealiSec(oTimeFrom(start));
}

f64 oTimeDeltaMilliSec(Time start, Time end) {
	return oTimeMilliSec(end - start);
}

f64 oTimeDeltaRealiSec(Time start, Time end) {
	return oTimeRealiSec(end - start);
}
