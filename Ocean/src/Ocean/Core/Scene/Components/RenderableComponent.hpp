#pragma once

#include "SECS/EntityComponent.hpp"

// libs
#include <glm/glm.hpp>

namespace Ocean {

	// Some "arcane" counter code to increment
	class RenderableColorComponent : SECS::EntityComponent {
	public:
		RenderableColorComponent() = default;
		RenderableColorComponent(const RenderableColorComponent&) = default;
		RenderableColorComponent(const glm::vec4& color) : m_Color(color) {}
		// ~RenderableColorComponent() override;

		virtual bool Init(void* pDescription) override final {
			return true;
		}

		static constexpr SECS::ComponentID ID = ID_DECLARATION(RenderableColorComponent);

	private:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

}	// Ocean
