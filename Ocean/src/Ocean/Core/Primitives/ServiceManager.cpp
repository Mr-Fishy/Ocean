#include "ServiceManager.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

namespace Ocean {

	u32 NameToIndex(cstring name);


	// TODO: Change to not be singleton based, rather is made in memory.
	static ServiceManager s_ServiceManager;

	ServiceManager* ServiceManager::Instance() {
		return &s_ServiceManager;
	}

	void ServiceManager::Init(Allocator* allocator) {
		oprint("> ServiceManager Initializing\n");

		p_Allocator = allocator;

		m_Services.Init(allocator, 8);
	}

	void ServiceManager::Shutdown() {
		m_Services.Shutdown();

		oprint("> ServiceManager Shutting Down\n");
	}

	void ServiceManager::AddService(Service* service, cstring name) {
		OASSERTM(m_Services.IndexOf(service) != -1, "Attempted to add existing service %s!", name);

		m_Services.InsertAt(service, NameToIndex(name));
	}

	void ServiceManager::RemoveService(cstring name) {
		
	}

	Service* ServiceManager::GetService(cstring name) {


		return nullptr;
	}

	u32 NameToIndex(cstring name) {
		oprint("Name is: %i", *name % 20);
		return *name % 20;
	}

}	// Ocean
