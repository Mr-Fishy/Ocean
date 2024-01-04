
#include "ocpch.hpp"
#include "LayerStack.hpp"

namespace Ocean {

	LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers) {
			layer->OnDetach();

			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, layer);

		m_LayersInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayersInsertIndex, layer);

		if (it != m_Layers.end()) {
			layer->OnDetach();
			m_Layers.erase(it);

			m_LayersInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_Layers.begin() + m_LayersInsertIndex, m_Layers.end(), overlay);

		if (it != m_Layers.end()) {
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}

}	// Ocean