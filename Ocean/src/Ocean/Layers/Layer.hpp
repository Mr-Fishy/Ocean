#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/Timestep.hpp"
#include "Ocean/Events/Event.hpp"

namespace Ocean {

	// A layer represents content that is rendered and is co-existing. Separate layers are rendered as given.
	//
	class Layer {
	public:
		// Creates a layer with a name, assigned to m_DebugName.
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnUpdate(Timestep ts) { }
		virtual void OnImGuiRender(Timestep ts) { }
		virtual void OnEvent(Event& event) { }

		// TODO: Enable / Disable Layers

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}	// Ocean

