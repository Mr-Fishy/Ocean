#pragma once

#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

// std
#include <map>

namespace Ocean {

    namespace Shrimp {
        
        class Shader;
        class Texture2D;
        class Font;

    }   // Shrimp


    // TODO: GUIDs -> https://github.com/graeme-hill/crossguid


    class ResourceManager {
    public:
        ResourceManager();
        ~ResourceManager();

        /**
         * @brief Load's a Shrimp::Shader from the given file and stores it with the given name.
         * 
         * @param path The path to the file.
         * @param name The name to reference the Shader as.
         * @return Ref<Shrimp::Shader>& 
         */
        static Ref<Shrimp::Shader>& LoadShader(cstring path, cstring name);
        /**
         * @brief Get the Shrimp::Shader object from m_Shaders.
         * 
         * @param name The name of the Shader.
         * @return Ref<Shrimp::Shader>& 
         */
        static Ref<Shrimp::Shader>& GetShader(cstring name);

        /**
         * @brief Load's a Shrimp::Texture2D from the given file and stores it with the given name.
         * 
         * @param path The path to the file.
         * @param name The name to reference the Texture2D as.
         * @return Ref<Shrimp::Texture2D>& 
         */
        static Ref<Shrimp::Texture2D>& LoadTexture(cstring path, cstring name);
        /**
         * @brief Get the Shrimp::Texture2D object from m_Textures.
         * 
         * @param name The name of the Texture2D.
         * @return Ref<Shrimp::Texture2D>& 
         */
        static Ref<Shrimp::Texture2D>& GetTexture(cstring name);

        /**
         * @brief Load's a Shrimp::Font from the given file and stores it with the given name.
         * 
         * @param path The path to the file.
         * @param name The name to reference the Font as.
         * @return Ref<Shrimp::Font>& 
         */
        static Ref<Shrimp::Font>& LoadFont(cstring path, cstring name);
        /**
         * @brief Get the Shrimp::Font object from m_Fonts.
         * 
         * @param name The name of the Font.
         * @return Ref<Shrimp::Font>& 
         */
        static Ref<Shrimp::Font>& GetFont(cstring name);

        // Audio Files

        /**
         * @brief Clears all of the loaded items.
         */
        static void Clear();

    private:
        /**
         * @brief Gets the ResourceManager's instance as there should only be one.
         * 
         * @return ResourceManager* 
         */
        static ResourceManager* Instance();

        /**
         * @brief Internal function to load a Shrimp::Shader from the given file.
         * 
         * @param path The path to the file.
         * @return Ref<Shrimp::Shader> 
         */
        Ref<Shrimp::Shader> LoadShaderFile(cstring path);

        /**
         * @brief Internal function to load a Shrimp::Texture2D from the given file.
         * 
         * @param path The path to the file.
         * @return Ref<Shrimp::Texture2D> 
         */
        Ref<Shrimp::Texture2D> LoadTextureFile(cstring path);

        /**
         * @brief Internal function to load a Shrimp::Font from the given file.
         * 
         * @param path The path to the file.
         * @return Ref<Shrimp::Font> 
         */
        Ref<Shrimp::Font> LoadFontFile(cstring path);

        /* --- */

        inline static Scope<ResourceManager> s_Instance = MakeScope<ResourceManager>(); /** @brief The ResourceManager's singleton instance. */

        std::map<cstring, Ref<Shrimp::Shader>> m_Shaders; /** @brief The Shrimp::Shader objects stored. */
        std::map<cstring, Ref<Shrimp::Texture2D>> m_Textures; /** @brief The Shrimp::Texture2D objects stored. */
        std::map<cstring, Ref<Shrimp::Font>> m_Fonts; /** @brief The Shrimp::Font objects stored. */

    };

}   // Ocean
