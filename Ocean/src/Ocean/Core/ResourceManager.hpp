#pragma once

#include "Ocean/Core/Types/SharedPtr.hpp"
#include "Ocean/Core/Types/Strings.hpp"

#include "Renderer/Texture.hpp"

#include <map>

namespace Ocean {

    namespace Shrimp {
        
        class Shader;
        class Texture;
        class Font;

    }   // Shrimp


    // TODO: GUIDs -> https://github.com/graeme-hill/crossguid


    class ResourceManager {
    public:
        ~ResourceManager();

        // Scene Files

        static SharedPtr<Shrimp::Shader>& LoadShader(cstring path, cstring name);
        static SharedPtr<Shrimp::Shader>& GetShader(cstring name);

        static SharedPtr<Shrimp::Texture2D>& LoadTexture(cstring path, cstring name);
        static SharedPtr<Shrimp::Texture2D>& GetTexture(cstring name);

        static SharedPtr<Shrimp::Font>& LoadFont(cstring path, cstring name);
        static SharedPtr<Shrimp::Font>& GetFont(cstring name);

        // Audio Files

        static void Clear();

    private:
        ResourceManager();

        static ResourceManager& Instance();

        SharedPtr<Shrimp::Shader> LoadShaderFile(cstring path);

        SharedPtr<Shrimp::Texture2D> LoadTextureFile(cstring path);

        SharedPtr<Shrimp::Font> LoadFontFile(cstring path);

        /* --- */

        inline static ResourceManager* s_Instance = nullptr;

        std::map<cstring, SharedPtr<Shrimp::Shader>> m_Shaders;
        std::map<cstring, SharedPtr<Shrimp::Texture2D>> m_Textures;
        std::map<cstring, SharedPtr<Shrimp::Font>> m_Fonts;

    };

}   // Ocean
