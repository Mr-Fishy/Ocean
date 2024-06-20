#include "ocpch.hpp"

#include "Ocean/Layer/LayerStack.hpp"

namespace Ocean {

	LayerStack::~LayerStack() {
		for (Layer* layer : v_Layers) {
			layer->OnDetach();

			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		v_Layers.emplace(v_Layers.begin() + m_LayerInsertIndex, layer);

		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		v_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(v_Layers.begin(), v_Layers.begin() + m_LayerInsertIndex, layer);

		if (it != v_Layers.end()) {
			layer->OnDetach();
			v_Layers.erase(it);

			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(v_Layers.begin() + m_LayerInsertIndex, v_Layers.end(), overlay);

		if (it != v_Layers.end()) {
			overlay->OnDetach();
			v_Layers.erase(it);
		}
	}

}	// Ocean
