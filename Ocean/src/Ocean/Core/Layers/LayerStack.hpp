#pragma once

#include "Ocean/Core/Base.hpp"
#include "Ocean/Core/Layers/Layer.hpp"

// std
#include <vector>
#include <iterator>

namespace Ocean {

	// The LayerStack represents and manages the layers that are rendered in the window.
	// Layers and overlays are functionaly the same, they are simply managed in different manners to ensure that overlays are always rendered on top of layers.
	//
	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack();

		// Pushes a layer to the layerstack at the end of the known layers so that it is rendered on top.
		//
		void PushLayer(Layer* layer);
		// Pushes a overlay to the end of the layerstack so that it is rendered over the known layers.
		//
		void PushOverlay(Layer* overlay);
		// Removes a layer from the layerstack, scopes to only the known layers in the layerstack.
		//
		void PopLayer(Layer* layer);
		// Removes a overlay from the layerstack, scopes to the unkown layers in the layerstack.
		//
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		// The application layerstack.
		//
		std::vector<Layer*> m_Layers;
		// Holds the index of the last known layer in the layerstack.
		//
		unsigned int m_LayersInsertIndex = 0;
	};

}	// Ocean