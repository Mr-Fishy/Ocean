#pragma once

#include "Ocean/Layer/Layer.hpp"

namespace Ocean {

	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return v_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return v_Layers.end(); }
		std::vector<Layer*>::const_iterator begin() const { return v_Layers.begin(); }
		std::vector<Layer*>::const_iterator end() const { return v_Layers.end(); }

		std::vector<Layer*>::reverse_iterator rbegin() { return v_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return v_Layers.rend(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return v_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return v_Layers.rend(); }

	private:
		std::vector<Layer*> v_Layers;

		ui32 m_LayerInsertIndex = 0;
	};

}	// Ocean
