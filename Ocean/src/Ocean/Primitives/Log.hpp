#pragma once

#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Service.hpp"

namespace Ocean {

	typedef void (*PrintCallback)(const char*); /** @brief Additional callback for printing. */

	class LogService : public Service {
	public:
		LogService() = default;
		virtual ~LogService() = default;

		static LogService& Instance();

		void PrintFormat(cstring format, ...) const;

		void SetCallback(PrintCallback callback);

		inline virtual cstring GetName() const override { return "OCEAN_Log_Service"; }

	private:
		static LogService* s_Instance;

		PrintCallback m_PrintCallback = nullptr;

	};	// LogService

	// Macros

	/** @brief Print's the given string and arguments to the console. */
	#define oprint(format, ...)    Ocean::LogService::Instance().PrintFormat(format, ## __VA_ARGS__)

	/** @brief Print's the given string and arguments to the console. Add's a new line after the output. */
	#define oprintret(format, ...) Ocean::LogService::Instance().PrintFormat(format, ## __VA_ARGS__); Ocean::LogService::Instance().PrintFormat("\n")

}	// Ocean
