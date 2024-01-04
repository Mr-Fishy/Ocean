#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/Timestep.hpp"
#include "Ocean/Events/Event.hpp"

namespace Ocean {

	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		// TODO: Enable / Disable Layers

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}	// Ocean

