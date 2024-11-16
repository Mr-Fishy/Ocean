#pragma once

#include "Ocean/Core/Types/Integers.hpp"

namespace Ocean {

    namespace Shrimp {
        
        typedef u64 ID;
    
        class Entity {
        public:
            Entity() = default;
            Entity(const Entity&) = default;

        private:
            ID m_Entity = 0;

        };

    }   // Shrimp

}   // Ocean