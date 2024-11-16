#pragma once

namespace Ocean {

	/**
	 * @brief A service class to allow services to be made for the Service Manager.
	 */
	class Service {
	public:
		virtual ~Service() = default;

		virtual void Init(void* config = nullptr) { }

	};	// Service

}	// Ocean
