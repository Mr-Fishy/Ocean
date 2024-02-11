#pragma once

// std
#include <string>
#include <optional>

namespace Ocean {

	class FileDialogs {
	public:
		static std::optional<std::string> OpenFile(const char* filter); // Returns Empty Strings If Cancelled
		static std::optional<std::string> SaveFile(const char* filter); // Returns Empty Strings If Cancelled
	};

}	// Ocean
