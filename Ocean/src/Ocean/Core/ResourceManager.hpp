#pragma once

#include "Ocean/Types/SmartPtrs.hpp"
#include "Ocean/Types/Strings.hpp"

#include "Ocean/Primitives/Macros.hpp"

// std
#include <map>

namespace Ocean {

    namespace Splash {
        
        class Shader;
        class Texture2D;
        class Font;

    }   // Splash


    // TODO: GUIDs -> https://github.com/graeme-hill/crossguid


    class ResourceManager {
    public:
        ResourceManager();
        ~ResourceManager();

        /**
         * @brief Load's a Splash::Shader from the given file and stores it with the given name.
         * 
         * @param path The path to the file.
         * @param name The name to reference the Shader as.
         * @return Ref<Splash::Shader>& 
         */
        OC_STATIC Ref<Splash::Shader>& LoadShader(cstring path, cstring name);
        /**
         * @brief Get the Splash::Shader object from m_Shaders.
         * 
         * @param name The name of the Shader.
         * @return Ref<Splash::Shader>& 
         */
        OC_STATIC Ref<Splash::Shader>& GetShader(cstring name);

        /**
         * @brief Load's a Splash::Texture2D from the given file and stores it with the given name.
         * 
         * @param path The path to the file.
         * @param name The name to reference the Texture2D as.
         * @return Ref<Splash::Texture2D>& 
         */
        OC_STATIC Ref<Splash::Texture2D>& LoadTexture(cstring path, cstring name);
        /**
         * @brief Get the Splash::Texture2D object from m_Textures.
         * 
         * @param name The name of the Texture2D.
         * @return Ref<Splash::Texture2D>& 
         */
        OC_STATIC Ref<Splash::Texture2D>& GetTexture(cstring name);

        /**
         * @brief Load's a Splash::Font from the given file and stores it with the given name.
         * 
         * @param path The path to the file.
         * @param name The name to reference the Font as.
         * @return Ref<Splash::Font>& 
         */
        OC_STATIC Ref<Splash::Font>& LoadFont(cstring path, cstring name);
        /**
         * @brief Get the Splash::Font object from m_Fonts.
         * 
         * @param name The name of the Font.
         * @return Ref<Splash::Font>& 
         */
        OC_STATIC Ref<Splash::Font>& GetFont(cstring name);

        // Audio Files

        /**
         * @brief Clears all of the loaded items.
         */
        OC_STATIC void Clear();

    private:
        /**
         * @brief Gets the ResourceManager's instance as there should only be one.
         * 
         * @return ResourceManager* 
         */
        OC_STATIC ResourceManager* Instance();

        /**
         * @brief Internal function to load a Splash::Shader from the given file.
         * 
         * @param path The path to the file.
         * @return Ref<Splash::Shader> 
         */
        Ref<Splash::Shader> LoadShaderFile(cstring path);

        /**
         * @brief Internal function to load a Splash::Texture2D from the given file.
         * 
         * @param path The path to the file.
         * @return Ref<Splash::Texture2D> 
         */
        Ref<Splash::Texture2D> LoadTextureFile(cstring path);

        /**
         * @brief Internal function to load a Splash::Font from the given file.
         * 
         * @param path The path to the file.
         * @return Ref<Splash::Font> 
         */
        Ref<Splash::Font> LoadFontFile(cstring path);

        /* --- */

        OC_STATIC_INLINE Scope<ResourceManager> s_Instance = MakeScope<ResourceManager>(); /** @brief The ResourceManager's singleton instance. */

        std::map<cstring, Ref<Splash::Shader>> m_Shaders; /** @brief The Splash::Shader objects stored. */
        std::map<cstring, Ref<Splash::Texture2D>> m_Textures; /** @brief The Splash::Texture2D objects stored. */
        std::map<cstring, Ref<Splash::Font>> m_Fonts; /** @brief The Splash::Font objects stored. */

    };

}   // Ocean
