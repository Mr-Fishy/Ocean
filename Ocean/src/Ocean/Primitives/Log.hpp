#pragma once

#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Macros.hpp"
#include "Ocean/Primitives/Service.hpp"

namespace Ocean {

	/** @brief Additional callback for printing. */
	typedef void (*PrintCallback)(const char*);

	/**
	 * @brief The Logging Service of Ocean, prints to a console.
	 */
	class LogService : public Service {
	public:
		LogService() = default;
		virtual ~LogService() = default;

		OC_STATIC LogService& Instance();

        /**
         * @brief Print's a given format to the console.
         * 
         * @param format The string format to print.
         * @param ... Arguments to pass into the format.
         */
		void PrintFormat(cstring format, ...) const;

        /**
         * @brief Set the PrintCallback for the Log Service.
         * 
         * @param callback The PrintCallback to use.
         */
		void SetCallback(PrintCallback callback);

        /**
         * @brief Get's the name of the Log Service.
         * 
         * @return cstring 
         */
		OC_INLINE virtual cstring GetName() const override { return "OCEAN_Log_Service"; }

	private:
		OC_STATIC_INLINE Scope<LogService> m_Instance = MakeScope<LogService>();

		PrintCallback m_PrintCallback = nullptr;

	};	// LogService

	// Macros

	/** @brief Print's the given string and arguments to the console. */
	#define oprint(format, ...)    Ocean::LogService::Instance().PrintFormat(format, ## __VA_ARGS__)

	/** @brief Print's the given string and arguments to the console. Add's a new line after the output. */
	#define oprintret(format, ...) Ocean::LogService::Instance().PrintFormat(format, ## __VA_ARGS__); Ocean::LogService::Instance().PrintFormat("\n")

}	// Ocean
