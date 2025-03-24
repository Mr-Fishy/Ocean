#pragma once

#include "Ocean/Types/Integers.hpp"
#include "Ocean/Primitives/Array.hpp"

namespace Ocean {

    namespace Reef {

       typedef u64 entityID;
    
        class Entity {
        public:
            Entity() = default;
            Entity(const Entity&) = default;
            ~Entity() = default;

        private:
            entityID m_Entity = 0;

        };

    }   // Reef

}   // Ocean