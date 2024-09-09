#pragma once

namespace Ocean {

	class Service {
	public:
		virtual void Init(void* config) { }
		virtual void Shutdown() { }

	};	// Service

	#define OCEAN_DECLARE_SERVICE(Type) static Type* Instance();

}	// Ocean