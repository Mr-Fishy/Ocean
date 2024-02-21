#pragma once

#include "Ocean/Core/Base.hpp"
#include "Ocean/Core/Scene/Scene.hpp"
#include "Ocean/Core/Scene/Entity.hpp"

namespace Ocean {

	// The class that manages the Scene Hierarchy Panel within Coral Editor.
	//
	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		/* --- */

		Ref<Scene> m_Context;

		Entity m_SelectionContext;
	};

}	// Ocean
