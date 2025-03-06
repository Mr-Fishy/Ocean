#pragma once

#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Strings.hpp"
#include "Ocean/Types/Timestep.hpp"

#include "Ocean/Primitives/Macros.hpp"

namespace Ocean {

    /**
     * @brief A layer to be rendered in the Application.
     */
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
        OC_INLINE virtual void OnAttach() { }
        /** @brief Occurs when the layer is dettached from a LayerStack. */
        OC_INLINE virtual void OnDetach() { }

        /** @brief Occurs when the layer is updated by the Application. */
        OC_INLINE virtual void OnUpdate(OC_UNUSED Timestep ts) { }

        /** @brief Occurs when there is an event submitted to the layer. */
        // virtual void OnEvent(Event& e) { }

        /**
         * @brief Returns true if the layer is enabled, false otherwise.
         * 
         * @return b8 
         */
        OC_INLINE b8 IsEnabled() const { return this->m_Enabled; }
        /**
         * @brief Sets m_Enabled to the given value.
         * 
         * @param enabled True to enable the layer, false to disable the layer.
         */
        OC_INLINE void SetEnabled(b8 enabled) { this->m_Enabled = enabled; }

    #ifdef OC_DEBUG

        /**
         * @brief Gets m_DebugName from the layer. DEBUG ONLY
         * 
         * @return const string&
         */
        OC_INLINE const string& GetName() const { return this->m_DebugName; }

    #endif

    protected:
        /** @brief Records if the layer is enabled, if not then updates are blocked. */
        b8 m_Enabled;

    #ifdef OC_DEBUG

    /** @brief A name for use in debug builds / engine debugging. */
        string m_DebugName;
        
    #endif

    };  // Layer

}   // Ocean
