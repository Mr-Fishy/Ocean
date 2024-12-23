#pragma once

#include "Ocean/Core/Types/SharedPtr.hpp"
#include "Ocean/Core/Types/Integers.hpp"

namespace Ocean {

    namespace Shrimp {

        class IndexBuffer {
        public:
            virtual ~IndexBuffer() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            virtual u32 GetCount() const = 0;

            static SharedPtr<IndexBuffer> Create(u32* indices, u32 count);

        };  // IndexBuffer

    }   // Shrimp

}   // Ocean