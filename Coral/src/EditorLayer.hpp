#pragma once

#include "Panels/SceneHierarchyPanel.hpp"
#include "Panels/ContentBrowserPanel.hpp"

// Ocean
#include <Ocean.hpp>
#include <Ocean/Renderer/Camera/EditorCamera.h>

namespace Ocean {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender(Timestep ts) override;
		void OnEvent(Event& e) override;

		void OnScenePlay();
		void OnSceneStop();

		// UI Panels
		void ToolbarUI();

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

		/* --- */

		OrthographicCameraController m_CameraController;

		// TEMP
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_HoveredEntity;

		Entity m_CameraEntity;
		Entity m_SecondCamera;
		Entity m_SquareEntity;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		Ref<Texture2D> m_CheckerboardTexture;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GizmoType = -1;

		enum class SceneState {
			Edit = 0,
			Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2] = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };

		// Panels
		// TODO: Make Dynamic "Panels" To Enable / Disable Panels To Render

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		// Editor Resources (Likely Should Be In A Ordered List)

		Ref<Texture2D> m_IconPlay, m_IconStop;
	};

}	// Ocean
