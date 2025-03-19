#pragma once

#include "Ocean/Primitives/DynamicArray.hpp"

#include "Ocean/Core/Layer/Layer.hpp"

namespace Ocean {

    /**
     * @brief A LayerStack manages pushing and popping Layers in a simple manner.
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

        /**
         * @brief Get's the first layer as an iterator.
         * 
         * @return DynamicArray<Layer*>::Iterator 
         */
        OC_INLINE DynamicArray<Layer*>::Iterator begin() { return m_Layers.Begin(); }
        /**
         * @brief Get's the first layer as a const-iterator.
         * 
         * @return DynamicArray<Layer*>::ConstIterator 
         */
         OC_INLINE DynamicArray<Layer*>::ConstIterator begin() const { return m_Layers.Begin(); }

        /**
         * @brief Get's the last layer as an iterator.
         * 
         * @return DynamicArray<Layer*>::Iterator 
         */
         OC_INLINE DynamicArray<Layer*>::Iterator end() { return m_Layers.End(); }
        /**
         * @brief Get's the last layer as a const-iterator.
         * 
         * @return DynamicArray<Layer*>::ConstIterator 
         */
         OC_INLINE DynamicArray<Layer*>::ConstIterator end() const { return m_Layers.End(); }

    private:
        /** @brief An array of Layer pointers representing a layerstack. */
        DynamicArray<Layer*> m_Layers;

        /** @brief Records the index of the last non-overlay Layer in the layerstack. */
        u32 m_InsertIndex;

    };  // LayerStack

}   // Ocean
