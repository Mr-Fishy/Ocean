#pragma once

#include "SECS/EntityComponent.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

	// Some "arcane" counter code to increment

	class TransformComponent : SECS::EntityComponent {
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent() : m_Position(1.0f), m_Rotation(0), m_UniformScale(1.0f) { }
		// ~TransformComponent() override;

		virtual bool Init(void* pDescription) override final {
			return true;
		}

		// @returns A vec3 position.
		//
		glm::vec3 GetPosition() const { return m_Position; }
		// Sets the position
		// @param transform: A vec3 position
		//
		void SetPosition(glm::vec3 position) { m_Position = position; }

		// @returns A float rotation in degrees.
		//
		float GetRotation() const { return m_Rotation; }
		// Sets the rotation
		// @param rotation: A float rotation in degrees.
		//
		void SetRotation(float rotation) { m_Rotation = rotation; }

		// @returns A float scale.
		//
		float GetScale() const { return m_UniformScale; }
		// Sets the uniform scale
		// @param scale: A float scale.
		//
		void SetScale(float scale) { m_UniformScale = scale; }

	private:
		static constexpr SECS::ComponentID ID = ID_DECLARATION(TransformComponent);

		glm::vec3 m_Position;
		float m_Rotation;
		float m_UniformScale;
	};

}	// Ocean
