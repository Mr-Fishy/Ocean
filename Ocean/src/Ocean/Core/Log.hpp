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
#define OC_INFO(...)			::Ocean::Log::GetClientLogger()->Log(::Ocean::Logger::LogSeverity::L_INFO, __VA_ARGS__)
#define OC_TRACE(...)			::Ocean::Log::GetClientLogger()->Log(::Ocean::Logger::LogSeverity::L_TRACE, __VA_ARGS__)
#define OC_WARN(...)			::Ocean::Log::GetClientLogger()->Log(::Ocean::Logger::LogSeverity::L_WARNING, __VA_ARGS__)
#define OC_ERROR(...)			::Ocean::Log::GetClientLogger()->Log(::Ocean::Logger::LogSeverity::L_ERROR, __VA_ARGS__)

/* --- Core Log Macros --- */
#define OC_CORE_INFO(...)		::Ocean::Log::GetCoreLogger()->Log(::Ocean::Logger::LogSeverity::L_INFO, __VA_ARGS__)
#define OC_CORE_TRACE(...)		::Ocean::Log::GetCoreLogger()->Log(::Ocean::Logger::LogSeverity::L_TRACE, __VA_ARGS__)
#define OC_CORE_WARN(...)		::Ocean::Log::GetCoreLogger()->Log(::Ocean::Logger::LogSeverity::L_WARNING, __VA_ARGS__)
#define OC_CORE_ERROR(...)		::Ocean::Log::GetCoreLogger()->Log(::Ocean::Logger::LogSeverity::L_ERROR, __VA_ARGS__)
