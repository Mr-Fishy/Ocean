#pragma once

#include "EntitySystemTypes.hpp"

// std
#include <limits>

namespace SECS {

	class EntityComponent {
	public:
		EntityComponent() : m_EntityId(INVALID_ENTITY_ID), m_PoolIndex(INVALID_POOL_INDEX) { }
		virtual ~EntityComponent() = default;

		virtual bool Init(void* pDescription) = 0; // TODO

		void SetEntityId(EntityId id) { m_EntityId = id; } // TODO: Make Private
		EntityId GetEntityId() const { return m_EntityId; }

		void SetPoolIndex(size_t index) { m_PoolIndex = index; }
		size_t GetPoolIndex() const { return m_PoolIndex; }

	private:
		static constexpr size_t INVALID_POOL_INDEX = UINT_MAX;

		EntityId m_EntityId;
		size_t m_PoolIndex;
	};

}	// SECS
