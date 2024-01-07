#pragma once

// https://github.com/electronicarts/EASTL/tree/master <-- Is faster than STD, requires labeled / stated use

#include "EntitySystemTypes.hpp"
#include "ComponentSystem.hpp"
#include "Entity.hpp"

// std
#include <vector>
#include <unordered_map>

namespace SECS {

	using std::vector;
	using std::unordered_map;

	class Component;

	class EntityManager {
	public:
		EntityManager();
		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(const EntityManager&) = delete;
		EntityManager(const EntityManager&&) = delete;
		EntityManager& operator=(const EntityManager&&) = delete;
		~EntityManager();

		void Update(double deltaTime);

		void CreateSystems();

		EntityId CreateEntity();
		void DestroyEntity(EntityId entityId);

	private:
		// An unordered map of EntityId's and their corresponding Entities.
		//
		using Entities = unordered_map<EntityId, Entity>;
		// A vector of vectors with Component pointers.
		//
		using Components = vector<vector<Component*>>;
		// A vector of ComponentSystems.
		// 
		using Systems = vector<int>;

		Entities m_Entities;
		Components m_Components;
		Systems m_Systems;
	};

} // SECS
