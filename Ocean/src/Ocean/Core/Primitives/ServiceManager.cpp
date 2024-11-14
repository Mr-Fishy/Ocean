#include "ServiceManager.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

namespace Ocean {

	ServiceManager& ServiceManager::Instance() {
		if (!s_Instance) {
			s_Instance = oallocat(ServiceManager, 1, MemoryService::Instance().SystemAllocator());
		}

		return *s_Instance;
	}

	void ServiceManager::Init() {
		
	}

	void ServiceManager::Shutdown() {
		ofree(&Instance(), MemoryService::Instance().SystemAllocator());
	}

	void ServiceManager::AddService(Service* service, cstring name) {
		OASSERTM(!m_Services.Find(name), "\t%s Already Exists In ServiceManager!", name);

		m_Services.Set(name, service);
	}

	void ServiceManager::RemoveService(cstring name) {
		m_Services.Remove(name);
	}

	Service* ServiceManager::GetService(cstring name) {
		return m_Services.Find(name) ? *m_Services.Get(name) : nullptr;
	}

}	// Ocean
