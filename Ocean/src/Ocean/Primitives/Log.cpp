#include "Log.hpp"

#include "Ocean/Types/Integers.hpp"

#include "Ocean/Primitives/Macros.hpp"

// std
#include <stdio.h>
#include <stdarg.h>

namespace Ocean {

	OC_STATIC constexpr u32 k_StringBufferSize = 1024 * 1024;
	OC_STATIC char LogBuffer[k_StringBufferSize];

	OC_STATIC void OutputConsole(char* logBuffer) {
		printf("%s", logBuffer);
	}



	LogService& LogService::Instance() {
		return *m_Instance.get();
	}

	void LogService::PrintFormat(cstring format, ...) const {
		va_list args;

		va_start(args, format);

		vsnprintf(LogBuffer, ArraySize(LogBuffer), format, args);

		LogBuffer[ArraySize(LogBuffer) - 1] = '\0';
		va_end(args);

		OutputConsole(LogBuffer);

		if (m_PrintCallback)
			m_PrintCallback(LogBuffer);
	}

	void LogService::SetCallback(PrintCallback callback) {
		m_PrintCallback = callback;
	}

}	// Ocean
