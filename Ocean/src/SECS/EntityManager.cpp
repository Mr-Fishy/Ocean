
#include "ocpch.hpp"

#include "EntityManager.hpp"

namespace SECS {

	EntityManager::EntityManager() {
		// TODO: Temp -> Create some validation tests.
	}

	EntityManager::~EntityManager() {}

	void EntityManager::Update(double deltaTime) {}

	void EntityManager::CreateSystems() {}

	EntityId EntityManager::CreateEntity() {
		return EntityId();
	}

	void EntityManager::DestroyEntity(EntityId entityId) {}

}	// SECS