#pragma once

#include "Ocean/Core/Layers/Layer.hpp"

#include "Ocean/Events/ApplicationEvent.hpp"
#include "Ocean/Events/KeyEvent.hpp"
#include "Ocean/Events/MouseEvent.hpp"

namespace Ocean {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}	// Ocean