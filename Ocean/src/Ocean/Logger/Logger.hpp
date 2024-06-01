#pragma once

#include "Ocean/Core/Defines.hpp"

#include "Ocean/Logger/Timer.hpp"

// std
#include <fstream>
#include <thread>
#include <mutex>
#include <iostream>
#include <queue>

namespace Ocean {

	class Logger {
	public:
		enum LogSeverity {
			L_INFO = 0,
			L_TRACE = 1,
			L_WARNING = 2,
			L_ERROR = 3,

			L_INTERNAL = 10,
		};

	private:
		enum ColorSelection {
			WHITE = 0,
			GREEN,
			YELLOW,
			RED,
			INTERNAL_MAGENTA,
			MAX_COLORS
		};

		struct Message {
			LogSeverity Severity;

			std::string Log;

			Message(LogSeverity sev, const std::string& mes) : Severity(sev), Log(mes) { }
		};

		Logger(const Logger& src) = delete;
		Logger& operator = (const Logger& rhs) = delete;

	public:
		Logger(std::string name, b8 writeToFile = true);
		~Logger();

		// template <typename ...Args> TODO: Argument support
		void Log(LogSeverity sev, const std::string& message);
		void Log(LogSeverity sev, const char* message);

		void Close();

	private:
		void ProcessEntries();
		//
		void PrintMessage(Message* message);
		//

		/* --- */

		std::string m_Name;

		std::thread m_Thread;
		std::mutex m_Mu;
		
		std::condition_variable m_Work;
		b8 m_Running = true;

		b8 m_WriteToFile;
		std::ofstream m_File;

		std::queue<Message*> m_MessageQueue;

		const char* Color[MAX_COLORS] = {
			"\x1B[37m",
			"\x1B[32m",
			"\x1B[33m",
			"\x1B[31m",
			"\x1B[35m",
		};
	};

}	// Ocean
