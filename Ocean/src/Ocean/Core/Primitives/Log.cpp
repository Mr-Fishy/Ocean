#include "Log.hpp"

#include "Ocean/Core/Types/Integers.hpp"

#include "Ocean/Core/Primitives/Macros.hpp"

#if defined (_MSC_VER)

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

#endif

#include <stdio.h>
#include <stdarg.h>

namespace Ocean {

	LogService s_LogService;

	static constexpr u32 k_StringBufferSize = 1024 * 1024;
	static char LogBuffer[k_StringBufferSize];

	static void OutputConsole(char* logBuffer) {
		printf("%s", logBuffer);
	}

#if defined (_MSC_VER)

	static void OutputVisualStudio(char* logBuffer) {
		OutputDebugStringA(logBuffer);
	}

#endif

	LogService* LogService::Instance() {
		return &s_LogService;
	}

	void LogService::PrintFormat(cstring format, ...) const {
		va_list args;

		va_start(args, format);

	#if defined(_MSC_VER)

		vsnprintf_s(LogBuffer, ArraySize(LogBuffer), format, args);

	#else

		vsnprintf(LogBuffer, ArraySize(LogBuffer), format, args);

	#endif

		LogBuffer[ArraySize(LogBuffer) - 1] = '\0';
		va_end(args);

		OutputConsole(LogBuffer);

	#if defined(_MSC_VER)

		OutputVisualStudio(LogBuffer);

	#endif

		if (m_PrintCallback)
			m_PrintCallback(LogBuffer);
	}

	void LogService::SetCallback(PrintCallback callback) {
		m_PrintCallback = callback;
	}

}	// Ocean
