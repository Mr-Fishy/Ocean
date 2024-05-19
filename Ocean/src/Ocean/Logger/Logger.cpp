#include "ocpch.hpp"

#include "Ocean/Logger/Logger.hpp"

namespace Ocean {

	Logger::Logger(std::string name, b8 writeToFile) : m_Name(name), m_WriteToFile(writeToFile) {
		m_Thread = std::thread{ &Logger::ProcessEntries, this };

		Log(LogSeverity::L_INTERNAL, "Starting " + m_Name + " Logger");
	}

	Logger::~Logger() {
		Log(LogSeverity::L_INTERNAL, "Closing " + m_Name + " Logger"); // Alternatively can notify all

		m_Thread.join();
	}

	void Logger::Log(LogSeverity sev, const std::string& message) {
		std::unique_lock lock(m_Mu);

		m_MessageQueue.push(new Message(sev, message));

		m_Work.notify_all();
	}

	void Logger::Close() {
		m_Running = false;
	}

	void Logger::ProcessEntries() {
		std::unique_lock lock(m_Mu);

		// Open Log File
		if (m_WriteToFile) {
			m_File.open(m_Name + ".log");

			if (m_File.fail())
				std::cerr << Color[RED] << "[ LOGGER  ] : Failed to open logfile.\033[0m\n" << std::endl;
		}

		while (m_Running || !m_MessageQueue.empty()) {
			m_Work.wait(lock, [this] { return !m_MessageQueue.empty(); });

			PrintMessage(m_MessageQueue.front());

			delete m_MessageQueue.front();
			m_MessageQueue.pop();
		}

		if (m_WriteToFile)
			m_File.close();
	}

	void Logger::PrintMessage(Message* message) {
		switch (message->Severity) {
			case LogSeverity::L_INFO:
				std::cout << Color[WHITE]  << "[ INFO    ] : " << message->Log << "\033[0m\n";
				m_File << "[ INFO    ] : " << message->Log << "\n";
				break;

			case LogSeverity::L_TRACE:
				std::cout << Color[GREEN]  << "[ TRACE   ] : " << message->Log << "\033[0m\n";
				m_File << "[ TRACE   ] : " << message->Log << "\n";
				break;

			case LogSeverity::L_WARNING:
				std::cout << Color[YELLOW] << "[ WARNING ] : " << message->Log << "\033[0m\n";
				m_File << "[ WARNING ] : " << message->Log << "\n";
				break;

			case LogSeverity::L_ERROR:
				std::cout << Color[RED]    << "[ ERROR   ] : " << message->Log << "\033[0m\n";
				m_File << "[ ERROR   ] : " << message->Log << "\n";
				break;

			case LogSeverity::L_INTERNAL:
				std::cout << Color[INTERNAL_MAGENTA] << "[ INTERNAL] : " << message->Log << "\033[0m\n";
				m_File << "[ INTERNAL] : " << message->Log << "\n";
				break;

			default:
				std::cout << Color[WHITE]  << "[ UNKNOWN ] : " << message->Log << "\033[0m\n";
				m_File << "[ UNKNOWN ] : " << message->Log << "\n";
				break;
		}
	}

}	// Ocean

