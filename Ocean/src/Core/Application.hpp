#pragma once

// std
#include <string>

namespace Ocean {

	class Application {
	public:
		Application();
		~Application();

		void Run();

	private:
		std::string m_Name;

		bool m_Running;
	};

}	// Ocean
