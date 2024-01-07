#pragma once

#include "EntitySystemTypes.hpp"

namespace SECS {

	class EntityComponent;

	class Entity {
	public:
		explicit Entity(EntityId id) : m_Id(id) {}

		EntityId GetId() const { return m_Id; }

	private:
		EntityId m_Id;

		std::unordered_map<ComponentID, EntityComponent*> m_Components;
	};

}	// SECS
