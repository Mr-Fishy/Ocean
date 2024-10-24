#pragma once

namespace Ocean {

	/**
	 * @brief A service class to allow services to be made for the Service Manager.
	 */
	class Service {
	public:
		virtual void Init(void* config) { }
		virtual void Shutdown() { }

	};	// Service

	#define OCEAN_DECLARE_SERVICE(Type) static Type* Instance();

}	// Ocean
