#pragma once

#include "Ocean/Core/Types/Integers.hpp"
#include "Ocean/Core/Types/SharedPtr.hpp"

namespace Ocean {

    namespace Shrimp {

        enum TextureFormat {
            R    = 0,
            G    = 1,
            B    = 2,
            RGB  = 3,
            RGBA = 4,
            A    = 5,

        };  // TextureFormat



        class Texture {
        public:
            virtual ~Texture() = default;

            virtual void Bind(u32 slot = 0) const = 0;

            virtual void SetData(void* data, u32 size) = 0;
            virtual void SetFormat(TextureFormat format) = 0;

            inline u32 GetWidth() const { return this->m_Width; }
            inline u32 GetHeight() const { return this->m_Height; }

            virtual u32 GetRendererID() const = 0;

            virtual bool operator == (const Texture& other) const = 0;

        protected:
            u32 m_Width, m_Height;

        };  // Texture



        class Texture2D : public Texture {
        public:
            virtual ~Texture2D() = default;

            static SharedPtr<Texture2D> Create(u32 width, u32 height);

        };  // Texture2D

    }   // Shrimp

}   // Ocean
