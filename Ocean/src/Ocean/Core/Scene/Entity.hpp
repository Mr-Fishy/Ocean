#pragma once

#include "Scene.hpp"
#include "Ocean/Utils/InternalUtils.hpp"

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
			OC_CORE_ASSERT(!HasComponent<T>(), "Entity already has component: " + GetTypeName<T>());

			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		// Gets a component of type T in the entity.
		//
		template<typename T>
		T& GetComponent() {
			OC_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: " + GetTypeName<T>());

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		// Gets a component, if component doesn't exist as expected, adds the component
		template<typename T>
		T& GetOrAddComponent() {
			if (!HasComponent<T>()) {
				AddComponent<T>();
			}

			return GetComponent<T>();
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
			OC_CORE_ASSERT(HasComponent<T>(), "Entity does not have component: " + GetTypeName<T>());

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		// Allows using (entityObject) as a booling statement. True if defined. False if undefined.
		//
		operator bool() const { return m_EntityHandle != entt::null; }
		// Allows implicit conversion to ENTT entity.
		//
		operator entt::entity() const { return m_EntityHandle; }
		// Allows implicit uint32_t conversion of the entity.
		//
		operator uint32_t() const { return (uint32_t)m_EntityHandle; };
		// 
		//
		bool operator == (const Entity& other) const {
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		};
		//
		//
		bool operator != (const Entity& other) const {
			return !(*this == other);
		};

	private:
		entt::entity m_EntityHandle{ entt::null };

		Scene* m_Scene = nullptr;
	};

}	// Ocean
