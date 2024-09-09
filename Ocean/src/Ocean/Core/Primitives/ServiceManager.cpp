#include "ServiceManager.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

#include <iterator>

namespace Ocean {

	static ServiceManager s_ServiceManager;

	ServiceManager* ServiceManager::Instance() {
		return &s_ServiceManager;
	}

	void ServiceManager::Init(Allocator* allocator) {
		oprint("ServiceManager Initializing\n");

		p_Allocator = allocator;
	}

	void ServiceManager::Shutdown() {
		m_Services.Clear();

		oprint("ServiceManager Shutting Down\n");
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
