
#include "ocpch.hpp"
#include "Scene.hpp"

#include "Components.hpp"
#include "Ocean/Renderer/Renderer2D.hpp"

#include "Entity.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

	Scene::Scene() { }

	Scene::~Scene() { }
	
	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { m_Registry.create(), this };
		
		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestoryEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts) {
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				// TODO: Move to Scene::OnScenePlay (Begin Scene)
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };

					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		// Render Sprites
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			const auto view = m_Registry.view<CameraComponent, TransformComponent>();
			for (auto entity : view) {
				const auto [camera, transform] = view.get<CameraComponent, TransformComponent>(entity);

				if (camera.Primary) {	// If the main camera is found, assign and exit the loop.
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();

					break;
				}
			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			const auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				const auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		
		// Resize our non-FixedAspectRatio cameras
		const auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	template<typename T>
	inline void Ocean::Scene::OnComponentAdded(Entity entity, T& component) {
		assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) { }

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) { }

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) { }

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) { }

}	// Ocean
