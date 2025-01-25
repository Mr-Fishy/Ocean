#pragma once

#include "Ocean/Scene/ECS/Entity.hpp"

namespace Ocean {

    namespace Reef {
    
        /*
         * The Registry is the actual list of entities and their components.
         * 
         * The underlaying structure will have to be either a unordered map,
         * or a dynamic set of lists set at a maximum of 1 kilobyte of memory
         * per list. 
         * 
         * The lists would each be a array with the entities children as a child.
         * 
         * The difficult part is for the sake of speed it must be continuous memory.
         * 
         * But the primary feature that will make it best is to have the index relate
         * to the actual entity at the index.
         * 
         * Alternatively it could be a hash table that has optimized buckets
         * for the components. Regardless, there are multiple ways to implement
         * the registry.
         * 
         * Good examples for ECS systems / registries are Entt or Mustache,
         * both are on Github and open source.
         * 
         * The following article may be helpful as well:
         * https://ajmmertens.medium.com/building-an-ecs-1-where-are-my-entities-and-components-63d07c7da742
         *
         * ECS systems are pretty well documented and have a mostly standard
         * implementation style, but it would be really cool to find a
         * competitive alternative implementation.
         */

        class Registry {
        public:
            Registry();
            ~Registry();

        private:
            

        };

    }   // Reef

}   // Ocean