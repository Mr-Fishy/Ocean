#pragma once

#include "Ocean/Core/Timestep.hpp"
#include "Ocean/Input/Event.hpp"

namespace Ocean {

	class Layer {
	public:
		Layer(const std::string& name = "Layer", b8 enabled = true) : m_DebugName(name), m_Enabled(enabled) { }
		virtual ~Layer() = default;

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		
		virtual void OnUpdate(Timestep ts) { }

		virtual void OnEvent(Event& event) { }

		b8 IsEnabled() const { return m_Enabled; }
		void ToggleEnabled() { m_Enabled = !m_Enabled; }

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;

		b8 m_Enabled;
	};

}	// Ocean
