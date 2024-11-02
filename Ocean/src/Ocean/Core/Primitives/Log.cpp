#include "Log.hpp"

#include "Ocean/Core/Types/Integers.hpp"

#include "Ocean/Core/Primitives/Macros.hpp"

// std
#include <stdio.h>
#include <stdarg.h>

namespace Ocean {

	static constexpr u32 k_StringBufferSize = 1024 * 1024;
	static char LogBuffer[k_StringBufferSize];

	static void OutputConsole(char* logBuffer) {
		printf("%s", logBuffer);
	}

	
	
	// TODO: Change to static instance on heap rather than on stack.
	static LogService* s_LogService = nullptr;
	
	LogService& LogService::Instance() {
		if (s_LogService == nullptr)
			s_LogService = new LogService();

		return *s_LogService;
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
