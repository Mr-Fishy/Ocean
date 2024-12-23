#pragma once

namespace Ocean {

    namespace Shrimp {
    
        class GraphicsContext {
        public:
            virtual ~GraphicsContext() = default;

            virtual void Init() = 0;
            
            virtual void SwapBuffers() = 0;

            static GraphicsContext* Create(void* window);

        };  // GraphicsContext

    }   // Shrimp

}   // Ocean
