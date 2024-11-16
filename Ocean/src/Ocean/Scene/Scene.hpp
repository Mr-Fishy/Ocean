#pragma once

#include "Ocean/Core/Types/Strings.hpp"
#include "Ocean/Core/Types/Timestep.hpp"

#include "Renderer/Components/Camera.hpp"

#include "Ocean/Scene/ECS/Registry.hpp"

namespace Ocean {

    namespace Shrimp {
    
        class Scene {
        public:
            Scene();
            ~Scene();

            Entity CreateEntity(const string& name = string());
            void DestroyEntity(const Entity& entity);

            void UpdateRuntime(Timestep delta);
            void UpdateEditor(Timestep delta, Camera& camera);

            Entity GetPrimaryCamera();

        private:
            template<typename T>
            void OnComponentAdded(const Entity& entity, const T& component);

            /* --- */

            Registry m_Registry;

        };

    }   // Shrimp

}   // Ocean