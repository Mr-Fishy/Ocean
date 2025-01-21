#include "LayerStack.hpp"

// std
#include <algorithm>

namespace Ocean {

        LayerStack::~LayerStack() {
            for (Layer* layer : this->m_Layers) {
                layer->OnDetach();

                delete layer;
            }
        }

        void LayerStack::PushLayer(Layer* layer) {
            this->m_Layers.Emplace(this->m_InsertIndex, layer);

            this->m_InsertIndex++;
        }

        void LayerStack::PushOverlay(Layer* overlay) {
            this->m_Layers.EmplaceBack(overlay);
        }

        void LayerStack::PopLayer(Layer* layer) {
            const auto it = std::find(this->m_Layers.begin(), this->m_Layers.begin() + this->m_InsertIndex, layer);

            if (it != this->m_Layers.end()) {
                layer->OnDetach();
                
                this->m_Layers.Erase(this->m_InsertIndex--);
            }
        }

        void LayerStack::PopOverlay(Layer* overlay) {
            const auto it = std::find(this->m_Layers.begin() + this->m_InsertIndex, this->m_Layers.end(), overlay);

            if (it != this->m_Layers.end()) {
                overlay->OnDetach();

                this->m_Layers.Erase(this->m_Layers.Size() - 1);
            }
        }

}   // Ocean
