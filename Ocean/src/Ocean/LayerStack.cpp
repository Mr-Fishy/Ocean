
#include "ocpch.hpp"
#include "LayerStack.hpp"

namespace Ocean {

	LayerStack::LayerStack() {}

	LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, layer);

		layer->OnAttach();

		m_LayersInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		m_Layers.emplace_back(overlay);

		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (it != m_Layers.end()) {
			m_Layers.erase(it);

			layer->OnDetach();

			m_LayersInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);

		if (it != m_Layers.end()) {
			m_Layers.erase(it);

			overlay->OnDetach();
		}
	}

}	// Ocean