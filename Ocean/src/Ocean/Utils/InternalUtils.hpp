#pragma once

// std
#include <string>

namespace Ocean {

	namespace Internal {

		// Sample: "Ocean::Internal::GetTypeNameHelper<struct Ocean::CameraComponent>::GetTypeName"

		// TODO: The Following Needs GCC Compiler Support To Give the Same Results As MSVN & Clang
		template <typename T>
		struct GetTypeNameHelper {
			static const std::string GetTypeName(void) {
				const std::string typeStr = std::string(__FUNCTION__).substr();
				const size_t BACK_SIZE = std::string(__FUNCTION__).rfind('>');
				const size_t FRONT_SIZE = std::string(__FUNCTION__).rfind('<') + 1u + 14u;

				return std::string(__FUNCTION__).substr(FRONT_SIZE, BACK_SIZE - FRONT_SIZE);
			}
		};

	}	// Internal

	template <typename T>
	const std::string GetTypeName(void) {
		return Internal::GetTypeNameHelper<T>::GetTypeName();
	}

}	// Ocean
