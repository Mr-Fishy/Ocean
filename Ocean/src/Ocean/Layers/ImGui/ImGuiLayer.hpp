#pragma once

#include "Ocean/Layers/Layer.hpp"

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

		void SetDarkThemeColors();

	private:
		bool m_BlockEvents = true;
	};

}	// Ocean
