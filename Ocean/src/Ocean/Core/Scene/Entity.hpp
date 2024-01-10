#pragma once

#include "Scene.hpp"

// libs
#include <entt/entt.hpp>

namespace Ocean {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		// Adds a component of type T to the entity.
		//
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			OC_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");

			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		// Gets a component of type T in the entity.
		//
		template<typename T>
		T& GetComponent() {
			OC_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		// Checks if the entity has a component of type T.
		// 
		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		// Removes a component of type T from the entity.
		//
		template<typename T>
		void RemoveComponent() {
			OC_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		// Allows using (entityObject) as a booling statement. True if defined. False if undefined.
		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle{ entt::null };

		Scene* m_Scene = nullptr;
	};

}	// Ocean
