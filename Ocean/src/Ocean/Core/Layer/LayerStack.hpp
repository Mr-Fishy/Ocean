#pragma once

#include "Ocean/Primitives/Array.hpp"

#include "Ocean/Core/Layer/Layer.hpp"

namespace Ocean {

    /**
     * @brief A LayerStack manages pushing and popping Layers in a simple manner.
     * 
     */
    class LayerStack {
    public:
        LayerStack() : m_Layers(), m_InsertIndex(0) { }
        ~LayerStack();

        /**
         * @brief Pushes a Layer onto m_Layers.
         * 
         * @param layer The Layer to push.
         */
        void PushLayer(Layer* layer);
        /**
         * @brief Pushes a Layer onto m_Layers after m_InsertIndex.
         * 
         * @param overlay The Layer to push.
         */
        void PushOverlay(Layer* overlay);

        /**
         * @brief Pops the given Layer if it is found in m_Layers.
         * 
         * @param layer The Layer to remove from the layerstack.
         */
        void PopLayer(Layer* layer);
        /**
         * @brief Pops the given Layer if it is found in m_Layers after m_InsertIndex.
         * 
         * @param overlay The Layer to remove from the layerstack.
         */
        void PopOverlay(Layer* overlay);

        OC_INLINE DynamicArray<Layer*>::iterator begin() { return m_Layers.begin(); }
        OC_INLINE DynamicArray<Layer*>::const_iterator begin() const { return m_Layers.begin(); }

        OC_INLINE DynamicArray<Layer*>::iterator end() { return m_Layers.end(); }
        OC_INLINE DynamicArray<Layer*>::const_iterator end() const { return m_Layers.end(); }

    private:
        DynamicArray<Layer*> m_Layers; /** @brief An array of Layer pointers representing a layerstack. */

        u32 m_InsertIndex; /** @brief Records the index of the last non-overlay Layer in the layerstack. */

    };  // LayerStack

}   // Ocean
