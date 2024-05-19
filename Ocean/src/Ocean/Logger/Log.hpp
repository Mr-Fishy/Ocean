#pragma once

#include "Ocean/Logger/Logger.hpp"

namespace Ocean {

	class Log {
	public:
		static void Init();
		static void Shutdown();

		static Logger* GetCoreLogger() { return s_CoreLogger; }
		static Logger* GetClientLogger() { return s_ClientLogger; }

	private:
		static Logger* s_CoreLogger;
		static Logger* s_ClientLogger;
	};

}	// Ocean

/* --- Client Log Macros --- */
#define OC_INFO(msg)			::Ocean::Log::GetClientLogger()->Log(::Ocean::Logger::LogSeverity::L_INFO, msg)
#define OC_TRACE(msg)			::Ocean::Log::GetClientLogger()->Log(::Ocean::Logger::LogSeverity::L_TRACE, msg)
#define OC_WARN(msg)			::Ocean::Log::GetClientLogger()->Log(::Ocean::Logger::LogSeverity::L_WARNING, msg)
#define OC_ERROR(msg)			::Ocean::Log::GetClientLogger()->Log(::Ocean::Logger::LogSeverity::L_ERROR, msg)

/* --- Core Log Macros --- */
#define OC_CORE_INFO(msg)		::Ocean::Log::GetCoreLogger()->Log(::Ocean::Logger::LogSeverity::L_INFO, msg)
#define OC_CORE_TRACE(msg)		::Ocean::Log::GetCoreLogger()->Log(::Ocean::Logger::LogSeverity::L_TRACE, msg)
#define OC_CORE_WARN(msg)		::Ocean::Log::GetCoreLogger()->Log(::Ocean::Logger::LogSeverity::L_WARNING, msg)
#define OC_CORE_ERROR(msg)		::Ocean::Log::GetCoreLogger()->Log(::Ocean::Logger::LogSeverity::L_ERROR, msg)
