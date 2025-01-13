#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Strings.hpp"
#include "Ocean/Types/Timestep.hpp"

#include "Ocean/Primitives/Macros.hpp"

namespace Ocean {

    class Layer {
    public:
        /**
         * @brief Constructs a new Layer.
         * 
         * @param name The name of the layer, only used when OC_DEBUG is enabled.
         */
        Layer(const string& name = "Layer");
        virtual ~Layer() = default;

        /** @brief Occurs when the layer is attached to a LayerStack. */
        virtual void OnAttach() { }
        /** @brief Occurs when the layer is dettached from a LayerStack. */
        virtual void OnDetach() { }

        /** @brief Occurs when the layer is updated by the Application. */
        virtual void OnUpdate(OC_UNUSED Timestep ts) { }

        /** @brief Occurs when there is an event submitted to the layer. */
        // virtual void OnEvent(Event& e) { }

        /**
         * @brief Returns true if the layer is enabled, false otherwise.
         * 
         * @return b8 
         */
        b8 IsEnabled() const { return this->m_Enabled; }
        /**
         * @brief Sets m_Enabled to the given value.
         * 
         * @param enabled True to enable the layer, false to disable the layer.
         */
        void SetEnabled(b8 enabled) { this->m_Enabled = enabled; }

    #ifdef OC_DEBUG

        /**
         * @brief Gets m_DebugName from the layer. DEBUG ONLY
         * 
         * @return const string&
         */
        const string& GetName() const { return this->m_DebugName; }

    #endif

    protected:
        b8 m_Enabled; /** @brief Records if the layer is enabled, if not then updates are blocked. */

    #ifdef OC_DEBUG
        
        string m_DebugName; /** @brief A name for use in debug builds / engine debugging. */
        
    #endif

    };  // Layer

}   // Ocean
