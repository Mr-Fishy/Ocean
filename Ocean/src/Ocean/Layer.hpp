#pragma once

#include "Ocean/Core.hpp"
#include "Ocean/Events/Event.hpp"

namespace Ocean {

	class OCEAN_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		// TODO: Enable / Disable Layers

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}	// Ocean

