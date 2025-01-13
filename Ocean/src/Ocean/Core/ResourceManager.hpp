#pragma once

#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Renderer/Texture.hpp"

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

        static Ref<Shrimp::Shader>& LoadShader(cstring path, cstring name);
        static Ref<Shrimp::Shader>& GetShader(cstring name);

        static Ref<Shrimp::Texture2D>& LoadTexture(cstring path, cstring name);
        static Ref<Shrimp::Texture2D>& GetTexture(cstring name);

        static Ref<Shrimp::Font>& LoadFont(cstring path, cstring name);
        static Ref<Shrimp::Font>& GetFont(cstring name);

        // Audio Files

        static void Clear();

    private:
        ResourceManager();

        static ResourceManager& Instance();

        Ref<Shrimp::Shader> LoadShaderFile(cstring path);

        Ref<Shrimp::Texture2D> LoadTextureFile(cstring path);

        Ref<Shrimp::Font> LoadFontFile(cstring path);

        /* --- */

        inline static ResourceManager* s_Instance = nullptr;

        std::map<cstring, Ref<Shrimp::Shader>> m_Shaders;
        std::map<cstring, Ref<Shrimp::Texture2D>> m_Textures;
        std::map<cstring, Ref<Shrimp::Font>> m_Fonts;

    };

}   // Ocean
