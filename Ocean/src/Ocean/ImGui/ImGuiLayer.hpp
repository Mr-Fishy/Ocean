#pragma once

#include "Ocean/Layer.hpp"

#include "Ocean/Events/ApplicationEvent.hpp"
#include "Ocean/Events/KeyEvent.hpp"
#include "Ocean/Events/MouseEvent.hpp"

namespace Ocean {

	class OCEAN_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}	// Ocean