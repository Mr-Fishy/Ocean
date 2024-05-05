#pragma once

#include "Ocean/Core/Timestep.hpp"

#include "Ocean/Renderer/Camera/EditorCamera.h"

// libs
#include <entt/entt.hpp>

namespace Ocean {

	class Entity;

	// The scene will hold the object list and communicate with the ECS.
	//
	class Scene {
	public:
		Scene();
		~Scene();

		// Creates an Entity with a TagComponent and a TransformComponent.
		// @param name: The name to tag the entity with through TagComponent.
		// @returns A new entity.
		//
		Entity CreateEntity(const std::string& name = std::string());
		// Removes an Entity from the registry.
		// @param entity: The entity to remove.
		//
		void DestoryEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		/* --- */

		// The object registry.
		//
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		/* --- */

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}	// Ocean
