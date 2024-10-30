#pragma once

namespace Ocean {

	/**
	 * @brief A service class to allow services to be made for the Service Manager.
	 */
	class Service {
	public:
		virtual ~Service() = default;

		virtual void Init(void* config) = 0;
		virtual void Shutdown() = 0;

	};	// Service

	/** @brief Defines a simple Instance retrieval function. */
	#define OCEAN_DECLARE_SERVICE(Type) static Type* Instance()

}	// Ocean
