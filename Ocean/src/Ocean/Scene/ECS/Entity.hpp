#pragma once

#include "Ocean/Core/Types/Integers.hpp"

namespace Ocean {

    namespace Reef {
        
        typedef u64 ID;
    
        class Entity {
        public:
            Entity() = default;
            Entity(const Entity&) = default;
            ~Entity() = default;

        private:
            ID m_Entity = 0;

        };

    }   // Reef

}   // Ocean