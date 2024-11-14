#pragma once

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Primitives/HashMap.hpp"

namespace Ocean {

	/**
	 * @brief The base manager for all services, mainly to be able to keep reference to them if needed.
	 */
	class ServiceManager {
	public:
		static ServiceManager& Instance();

		/**
		 * @brief Initializes the Service Manager with no services.
		 * @param allocator - The Ocean memory allocator to use, by default it is the Memory Service System Allocator.
		 */
		void Init();
		/**
		 * @brief Shuts down the Service Manager. All services need to be shutdown before calling this.
		 */
		static void Shutdown();

		/**
		 * @brief Add's a service to the service manager. 
		 * @param service - Pointer of the service to add.
		 * @param name - The name of the service to add.
		 */
		void AddService(Service* service, cstring name);
		/**
		 * @brief Remove's a service from the service manager.
		 * @param name - The name of the service to remove.
		 */
		void RemoveService(cstring name);

		/**
		 * @brief Get's the service by name, if the service does not exist, get's the instance of the service.
		 * @param name - The name of the service to get.
		 * @return A pointer of the service.
		 */
		Service* GetService(cstring name);

		/**
		 * @brief Get's the service by type, if the service does not exist, get's the instance of the service.
		 * @tparam T - The type of service to get.
		 * @return A pointer of the service of type T.
		 */
		template<typename T>
		T* Get();

	private:
		ServiceManager() : m_Services(4) { }
		~ServiceManager() = default;

		ServiceManager(const ServiceManager&) = delete;
		ServiceManager operator = (const ServiceManager&) = delete;

		static inline ServiceManager* s_Instance = nullptr;

		HashMap<cstring, Service*> m_Services;

	};

	template<typename T>
	inline T* ServiceManager::Get() {
		T* service = static_cast<T*>(GetService(T::Name()));
		if (service == nullptr) {
			AddService(&T::Instance(), T::Name());
		}

		return &T::Instance();
	}

}	// Ocean
