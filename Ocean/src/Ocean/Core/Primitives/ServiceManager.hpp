#pragma once

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"
#include "Ocean/Core/Primitives/HashMap.hpp"

// std
#include <unordered_map>

namespace Ocean {

	class ServiceManager {
	public:
		ServiceManager() = default;
		~ServiceManager() = default;

		OCEAN_DECLARE_SERVICE(ServiceManager);

		void Init(Allocator* allocator);
		void Shutdown();

		void AddService(Service* service, cstring name);
		void RemoveService(cstring name);

		Service* GetService(cstring name);

		template<typename T>
		T* Get();

	private:
		Allocator* p_Allocator = nullptr;

		HashMap<cstring, Service*> m_Services;

	};

	template<typename T>
	inline T* ServiceManager::Get() {
		T* service = (T*)GetService(T::Name());
		if (service == nullptr) {
			AddService(T::Instance(), T::Name());
		}

		return T::Instance();
	}

}	// Ocean
