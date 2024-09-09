#pragma once

#include "Ocean/Core/Types/Strings.hpp"

#include "Ocean/Core/Primitives/Service.hpp"

namespace Ocean {

	typedef void (*PrintCallback)(const char*); // Additional callback for printing.

	class LogService : public Service {
	public:
		OCEAN_DECLARE_SERVICE(LogService);

		void PrintFormat(cstring format, ...) const;

		void SetCallback(PrintCallback callback);

		static cstring Name() { return "OCEAN_Log_Service"; }

	private:
		PrintCallback m_PrintCallback = nullptr;

	};	// LogService

	// Macros

#if defined (_MSC_VER)
	#define oprint(format, ...)    Ocean::LogService::Instance()->PrintFormat(format, __VA_ARGS__)
	#define oprintret(format, ...) Ocean::LogService::Instance()->PrintFormat(format, __VA_ARGS__); Ocean::LogService::Instance()->PrintFormat("\n")
#else
	#define oprint(format, ...)    Ocean::LogService::Instance()->PrintFormat(format, ## __VA_ARGS__)
	#define oprintret(format, ...) Ocean::LogService::Instance()->PrintFormat(format, ## __VA_ARGS__); Ocean::LogService::Instance()->PrintFormat("\n")
#endif

}	// Ocean
