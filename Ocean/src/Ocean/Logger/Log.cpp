#include "ocpch.hpp"

#include "Ocean/Logger/Log.hpp"

namespace Ocean {

	Logger* Log::s_CoreLogger = nullptr;
	Logger* Log::s_ClientLogger = nullptr;

	void Log::Init() {
		s_CoreLogger = new Logger("Ocean", true);
		s_ClientLogger = new Logger("APP", false);
	}

	void Log::Shutdown() {
		s_CoreLogger->Close();
		delete s_CoreLogger;

		s_ClientLogger->Close();
		delete s_ClientLogger;
	}

}	// Ocean
