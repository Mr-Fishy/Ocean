#pragma once

#include "Ocean/Core/Base.hpp"

// libs
// Disabled warnings due to primarly C++ version deprication warnings.
// This will not be needed once a Ocean developed log system is made and spdlog is no longer needed.
#pragma warning(push)
#pragma warning(disable : 6285 26437 26450 26495 26498 26800)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Ocean {

	class Log {
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

/* --- Client Log Macros --- */
#define OC_TRACE(...)         ::Ocean::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OC_INFO(...)          ::Ocean::Log::GetClientLogger()->info(__VA_ARGS__)
#define OC_WARN(...)          ::Ocean::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OC_ERROR(...)         ::Ocean::Log::GetClientLogger()->error(__VA_ARGS__)
#define OC_CRITICAL(...)      ::Ocean::Log::GetClientLogger()->critical(__VA_ARGS__)


/* --- Core Log Macros --- */
#define OC_CORE_TRACE(...)    ::Ocean::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OC_CORE_INFO(...)     ::Ocean::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OC_CORE_WARN(...)     ::Ocean::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OC_CORE_ERROR(...)    ::Ocean::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OC_CORE_CRITICAL(...) ::Ocean::Log::GetCoreLogger()->critical(__VA_ARGS__)
